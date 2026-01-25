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

### 2.3. Animation and Timer RAII
The current animation system is a partial wrapper.
- **Gap**: Animation timelines and safe C++ callback (lambda) cleanup.
- **Impact**: Memory leaks if animations are deleted before callbacks are cleared.

## 3. Remediation Plan

### Phase 1: The Scoped Enumeration Framework
- **Implementation**: Expand `misc/enums.h` with `enum class` wrappers for all major LVGL categories.
- **Refactor**: Update `Object` and `Widget` base methods to prefer these types over `lv_...` integers.

### Phase 2: Deep Object Wrappers (The Proxy Pattern)
- **Chart**: Implement `ChartSeries` to encapsulate series metadata and data setting methods.
- **Table**: Implement `TableCell` proxy returning from `table.cell(r, c)` to allow `cell.set_value("...").set_ctrl(...)`.

### Phase 3: Event System Hardening
- **Refactor**: Ensure all widgets (Arc, Slider, Switch, Checkbox, Textarea) have standardized `on_value_changed`, `on_clicked` callbacks that utilize the safe `Event` wrapper.

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
| Style Proxies | 🚧 IN PROGRESS | |
