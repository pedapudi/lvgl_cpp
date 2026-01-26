# API Coverage & Expansion Plan (v0.4.0+)

This document outlines the roadmap for achieving near-100% API coverage of LVGL v9 features within the `lvgl_cpp` framework.

## 1. Executive Summary

Current `lvgl_cpp` provides a robust foundation with the `Object` hierarchy, `StyleProxy`, `LayoutProxy`, and major widgets. However, significant gaps remain in specialized widgets, system abstractions (Display, Input Device, File System), and advanced drawing features introduced in LVGL v9. This plan prioritizes closing these gaps to enable full-scale application development.

## 2. Coverage Audit (v9.4 Baseline)

### 2.1. Widget Inventory

| LVGL Widget | `lvgl_cpp` Wrapper | Status | Notes |
| :--- | :--- | :--- | :--- |
| `lv_animimage` | `widgets/anim_image.h` | ✅ | Basic wrapper exists. |
| `lv_arc` | `widgets/arc.h` | ✅ | |
| `lv_bar` | `widgets/bar.h` | ✅ | |
| `lv_button` | `widgets/button.h` | ✅ | Core interactive widget. |
| `lv_buttonmatrix` | `widgets/button_matrix.h` | ✅ | |
| `lv_calendar` | `widgets/calendar.h` | ⚠️ | Needs verification of header/arrow integration. |
| `lv_canvas` | `widgets/canvas.h` | ✅ | Drawing API recently expanded (Issue #144). |
| `lv_chart` | `widgets/chart.h` | ⚠️ | Complex API; likely needs cursor/series wrappers. |
| `lv_checkbox` | `widgets/checkbox.h` | ✅ | |
| `lv_dropdown` | `widgets/dropdown.h` | ✅ | |
| `lv_image` | `widgets/image.h` | ✅ | |
| `lv_imagebutton` | `widgets/image_button.h` | ✅ | |
| `lv_keyboard` | `widgets/keyboard.h` | ✅ | |
| `lv_label` | `widgets/label.h` | ✅ | |
| `lv_led` | `widgets/led.h` | ✅ | |
| `lv_line` | `widgets/line.h` | ✅ | |
| `lv_list` | `widgets/list.h` | ⚠️ | Verify button addition API vs `lv_list_add_btn`. |
| `lv_lottie` | `widgets/lottie.h` | ✅ | Implemented in Phase 1. |
| `lv_menu` | `widgets/menu.h` | ⚠️ | Complex navigation widget; needs deep review. |
| `lv_msgbox` | `widgets/msgbox.h` | ✅ | |
| `lv_roller` | `widgets/roller.h` | ✅ | |
| `lv_scale` | `widgets/scale.h` | ✅ | Implemented recently. |
| `lv_slider` | `widgets/slider.h` | ✅ | |
| `lv_span` | `widgets/span.h` | ✅ | |
| `lv_spinbox` | `widgets/spinbox.h` | ✅ | |
| `lv_spinner` | `widgets/spinner.h` | ✅ | |
| `lv_switch` | `widgets/switch.h` | ✅ | |
| `lv_table` | `widgets/table.h` | ⚠️ | Needs cell draw descriptor support. |
| `lv_tabview` | `widgets/tabview.h` | ✅ | |
| `lv_textarea` | `widgets/textarea.h` | ✅ | |
| `lv_tileview` | `widgets/tileview.h` | ✅ | |
| `lv_win` | `widgets/win.h` | ✅ | |
| `3dtexture` | **MISSING** | ❌ | Niche, low priority. |

### 2.2. System Modules

- **Display (`lv_display`)**: ✅ **DONE**. `display/display.h` provides full abstraction.
- **Input Device (`lv_indev`)**: ✅ **DONE**. `indev/input_device.h` covers inputs and cursors.
- **Group (`lv_group`)**: ✅ **DONE**. Core group management is functional.
- **File System (`lv_fs`)**: **PARTIAL**. `misc/file_system.h` exists but driver registration logic is complex.
- **Theme (`lv_theme`)**: ✅ **DONE**. `misc/theme.h` wrapper implemented.

## 3. Implementation Roadmap

### Phase 1: High-Value Widget Gaps (Completed)
**Goal**: Enable rich, animated UIs.
1.  **Lottie Wrapper**: Implemented `widgets/lottie.h` wrapping `lv_lottie`.
2.  **Chart & Series**: Enhance `Chart` with a `Series` helper class for type-safe data management.
3.  **Menu & Navigation**: Harden `Menu` widget wrapper to ensure easy nested navigation building.

### Phase 2: System Abstractions (Completed)
**Goal**: Manage hardware resources via C++.
1.  **Display Class**: Implemented `core/display.h` wrapping `lv_display_t`.
    - Methods: `set_resolution`, `set_flush_cb`, `set_buffers`, `set_default`.
2.  **Input Device Class**: Implemented `core/indev.h` wrapping `lv_indev_t`.
    - Methods: `set_read_cb`, `set_cursor`, `get_type`.
3.  **Theme Manager**: Implemented `misc/theme.h` for easy theme application.

### Phase 3: Advanced Drawing & Effects (Completed)
**Goal**: Unlock hardware acceleration and visual flair.
1.  **Vector Graphics**: Wrapped `lv_vector` API as `misc/vector.h` (VectorDraw, VectorPath).
2.  **Masking**: Implemented `misc/mask.h` for rectangle masks.

## 4. Implementation Details

### 4.1. Lottie Implementation (Done)
```cpp
class Lottie : public Widget<Lottie> {
public:
    Lottie(Object* parent);
    void set_src_data(const void* data, size_t size);
    void set_src_file(const char* path);
    lv_anim_t* get_anim();
};
```

### 4.2. Display Abstraction (Done)
```cpp
class Display {
public:
    static Display create(int32_t hor_res, int32_t ver_res);
    void set_flush_cb(lv_display_flush_cb_t cb);
    void set_buffers(void* buf1, void* buf2, uint32_t size, lv_display_render_mode_t mode);
    static void set_default(Display& disp);
};
```

### 4.3. Vector Graphics (Done)
```cpp
VectorPath path;
path.move_to(10, 10);
path.line_to(100, 100);

VectorDraw draw(layer);
draw.set_stroke_color(Color::white());
draw.add_path(path);
draw.draw();
```
