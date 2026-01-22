# API coverage report

This document reports the coverage of LVGL C API functions by lvgl_cpp wrappers.

## Methodology

The coverage analysis scans all lvgl_cpp source files (`.h` and `.cpp`) for actual LVGL C function calls, comparing against the complete set of public C functions extracted from LVGL headers. This approach accurately identifies which C functions are wrapped, including:

- Inline methods in headers
- Methods inherited from parent classes
- CRTP-generated methods

## Summary

| Metric | Value |
|--------|-------|
| Total LVGL C Functions | 1867 |
| Called by lvgl_cpp | 613 |
| Not Used | 1254 |
| **Overall Coverage** | **32.8%** |

## Subsystem coverage

### Complete coverage (100%)

These widgets have full API coverage:

| Widget | Functions |
|--------|-----------|
| `lv_textarea` | 37/37 |
| `lv_dropdown` | 24/24 |
| `lv_menu` | 20/20 |
| `lv_bar` | 16/16 |
| `lv_msgbox` | 12/12 |
| `lv_roller` | 11/11 |
| `lv_tabview` | 11/11 |
| `lv_line` | 9/9 |
| `lv_led` | 7/7 |
| `lv_list` | 5/5 |
| `lv_tileview` | 5/5 |
| `lv_win` | 5/5 |
| `lv_checkbox` | 4/4 |
| `lv_switch` | 3/3 |
| `lv_async` | 2/2 |
| `lv_button` | 1/1 |

### High coverage (>75%)

| Subsystem | Coverage | Notes |
|-----------|----------|-------|
| `lv_spinbox` | 95.0% (19/20) | Missing 1 function |
| `lv_arc` | 92.9% (26/28) | Missing 2 functions |
| `lv_canvas` | 92.9% (13/14) | Missing 1 function |
| `lv_keyboard` | 91.7% (11/12) | Missing 1 function |
| `lv_table` | 82.4% (14/17) | Missing 3 functions |
| `lv_display` | 75.0% (54/72) | Missing 18 functions |

### Medium coverage (25-75%)

| Subsystem | Coverage | Missing Functions |
|-----------|----------|-------------------|
| `lv_timer` | 63.6% (14/22) | `lv_timer_create_basic`, `lv_timer_get_idle`, `lv_timer_get_next`, `lv_timer_get_paused`, `lv_timer_get_time_until_next`, `lv_timer_handler_run_in_period`, `lv_timer_periodic_handler`, `lv_timer_set_cb` |
| `lv_label` | 61.9% (13/21) | `lv_label_cut_text`, `lv_label_get_letter_on`, `lv_label_get_letter_pos`, `lv_label_ins_text`, `lv_label_is_char_under_pos`, `lv_label_set_text_fmt`, `lv_label_set_text_static`, `lv_label_set_translation_tag` |
| `lv_scale` | 58.8% (20/34) | Section-related functions not wrapped |
| `lv_calendar` | 50.0% (11/22) | Chinese calendar and header functions not wrapped |
| `lv_group` | 48.1% (13/27) | Callback and wrap functions not wrapped |
| `lv_fs` | 48.6% (18/37) | Advanced filesystem functions not wrapped |
| `lv_image` | 45.0% (27/60) | Many image-specific functions not wrapped |
| `lv_anim` | 39.1% (27/69) | Advanced animation controls not wrapped |
| `lv_chart` | 38.1% (16/42) | Series manipulation functions not wrapped |
| `lv_indev` | 31.3% (21/67) | Many input device functions not wrapped |

### Low coverage (<25%)

| Subsystem | Coverage | Notes |
|-----------|----------|-------|
| `lv_slider` | 23.5% (4/17) | Uses `lv_bar_*` via inheritance |
| `lv_color` | 22.0% (11/50) | Color manipulation utilities |
| `lv_obj` | 17.6% (89/505) | Very large API surface |
| `lv_event` | 12.2% (5/41) | Event handling utilities |
| `lv_font` | 8.3% (2/24) | Font manipulation functions |
| `lv_style` | 4.1% (6/146) | Style property setters |
| `lv_draw` | 2.1% (7/338) | Low-level drawing primitives |
| `lv_span` | 0.0% (0/7) | Span widget not wrapped |
| `lv_flex` | 0.0% (0/1) | Only `lv_flex_init` (system function) |
| `lv_grid` | 0.0% (0/2) | Only system-level functions |

## Analysis

### Why some coverage appears low

1. **Inheritance**: `Slider` uses `Bar`'s C functions, so `lv_slider_*` functions show low coverage even though the widget is fully functional.

2. **System functions**: `lv_flex_init`, `lv_grid_init`, `lv_grid_fr` are internal LVGL setup functions not meant for user code.

3. **Low-level primitives**: `lv_draw_*` functions (338 total) are primarily used internally by LVGL's rendering engine.

4. **Style setters**: The 146 `lv_style_*` functions include individual property setters that could be exposed through a type-safe wrapper.

### Priority areas for improvement

1. **`lv_obj_*` (505 functions)**: Core object API. Many advanced layout, scrolling, and event functions are not wrapped.

2. **`lv_style_*` (146 functions)**: Consider creating a fluent Style builder API.

3. **`lv_event_*` (41 functions)**: Event bubbling and custom event data handling.

4. **`lv_chart_*` (42 functions)**: Chart series and cursor manipulation.

5. **`lv_span_*` (7 functions)**: Span widget has no wrapper.

## Regenerating this report

Run the coverage audit script:

```bash
cd lvgl_cpp
python3 scripts/audit_api_coverage.py ../lvgl .
```

The JSON data is saved to `docs/api_coverage.json`.
