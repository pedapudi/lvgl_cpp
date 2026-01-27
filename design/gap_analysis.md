# Gap analysis: lvgl_cpp vs lvgl v9

## 1. Executive Summary
This document identifies critical functional and architectural gaps between the current `lvgl_cpp` implementation and the upstream LVGL v9.4+ features. It provides a remediation plan to achieve production-ready coverage for core systems and widgets.

## 2. Identified Gaps

### 2.1. Type Safety and Scoped Enums
Many LVGL constants are still accessed via raw C defines, leading to namespace pollution and weak type checking.
- **Gap**: Missing C++20 enum classes for `Flag`, `TextAlign`, `TextDecor`, `Opacity`, `Palette`, `FlexAlign`, and `GridAlign`.
- **Impact**: Increased risk of passing illegal values to widget methods; poor IDE autocompletion for style properties.

### 2.2. Complex Widget Encapsulation
Widgets with internal state or complex handle management (Charts, Tables) lack ergonomic C++ proxies.
- **Gap (Chart)**: Interactive series management requires raw `lv_chart_series_t*` pointers.
- **Gap (Table)**: Cell manipulation requires repeated row/column coordinate passing instead of fluent cell proxies.
- **Impact**: Verbose code and increased potential for use-after-free or double-free errors when managing external handles.

### 2.4. Core Object API Coverage
The automated API audit revealed low coverage for the core `lv_obj` class (~37% wrapped). A manual review against `lvgl/src/core/*.h` identifies the following specific functional gaps that must be addressed in `lvgl_cpp/core/object.h`:

- **Scrolling Configuration** (`lv_obj_scroll.h`)
    - *Missing*: `lv_obj_set_scrollbar_mode`, `lv_obj_set_scroll_dir`, `lv_obj_set_scroll_snap_x/y`.
    - *Impact*: Inability to hide scrollbars or lock scroll axes (e.g., vertical-only lists).
- **Coordinate Systems** (`lv_obj_pos.h`)
    - *Missing*: `lv_obj_get_coords` (absolute screen coordinates), `lv_obj_get_content_coords`, `lv_obj_transform_point`.
    - *Impact*: Critical for implementing custom drag-drop, collision detection, or advanced animations.
- **Interaction & Drawing** (`lv_obj_draw.h`, `lv_obj.h`)
    - *Missing*: `lv_obj_set_ext_click_area`, `lv_obj_refresh_ext_draw_size`, `lv_obj_hit_test`.
    - *Impact*: Cannot expand touch areas for small buttons; custom drawing updates may be clipped.
- **Advanced Tree Operations** (`lv_obj_tree.h`)
    - *Missing*: `lv_obj_swap` (z-order swap), `lv_obj_get_index`.
    - *Impact*: Dynamic reordering of widgets (e.g., bringing a focused card to front) is difficult.

## 3. Remediation Plan

### Phase 1: The Scoped Enumeration Framework
- **Implementation**: Expand `misc/enums.h` with `enum class` wrappers for all major LVGL categories.
- **Refactor**: Update `Object` and `Widget` base methods to prefer these types over `lv_...` integers.

### Phase 2: Deep Object Wrappers (The Proxy Pattern)
- **Chart**: Implement `ChartSeries` to encapsulate series metadata and data setting methods.
- **Table**: Implement `TableCell` proxy returning from `table.cell(r, c)` to allow `cell.set_value("...").set_ctrl(...)`.

### Phase 3: Event System Hardening
- **Refactor**: Ensure all widgets (Arc, Slider, Switch, Checkbox, Textarea) have standardized `on_value_changed`, `on_clicked` callbacks that utilize the safe `Event` wrapper.

### Phase 4: Core Coverage (In Progress)
- **Object**: Systematically wrap missing `lv_obj_*` functions.
- **Animation**: Verify RAII compliance for lambda callbacks.

## 4. Implementation Details (Remediation)

### ChartSeries Implementation
```cpp
class ChartSeries {
 public:
  ChartSeries(Chart* chart, lv_chart_series_t* series);
  void set_next_value(int32_t value);
  void set_next_value2(int32_t x_value, int32_t y_value);
  void set_all_values(int32_t value);
  void set_color(lv_color_t color);
  lv_chart_series_t* raw() const;
};
```

### TableCell Implementation
```cpp
class TableCell {
 public:
  TableCell(Table* table, uint32_t row, uint32_t col);
  TableCell& set_value(const char* txt);
  TableCell& set_ctrl(lv_table_cell_ctrl_t ctrl);
  TableCell& set_user_data(void* user_data);
};
```

## 5. Status and Tracking
| Feature | Status | Tracking |
| :--- | :--- | :--- |
| Scoped Enums | ✅ COMPLETE | issue/enums |
| ChartSeries | ✅ COMPLETE | test/chart_table |
| TableCell | ✅ COMPLETE | test/chart_table |
| Animation RAII | 🚧 IN PROGRESS | issue/animations |
| Style Proxies | ✅ COMPLETE | style_proxy.h |
| Layout Helpers | ✅ COMPLETE | misc/layout.h |
| Object Introspection | ✅ COMPLETE | core/object.h |
| Core Object API | Phase 9 | ✅ COMPLETE |
| High Coverage (<100%) | Phase 10 | 🚧 IN PROGRESS |
