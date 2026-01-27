# API coverage report

This document reports the coverage of LVGL's **user-facing** C API by lvgl_cpp wrappers.

## Methodology

### What is measured

The audit scans all lvgl_cpp source files (`.h` and `.cpp`) for actual usage of:
1. **Functions**: LVGL C API function calls (e.g., `lv_obj_set_pos`)
2. **Enums and Constants**: LVGL enum values (e.g., `LV_ALIGN_CENTER`)

### Internal vs user-facing APIs

| API Type | Description | lvgl_cpp Policy |
|----------|-------------|-----------------|
| **User-facing** | Public functions documented in [LVGL docs](https://docs.lvgl.io/9.4/) | ✅ Should be wrapped |
| **Internal** | Implementation details (`_lv_*`, `*_private.h`) | ❌ Must NOT be wrapped |

**Excluded from coverage**:
- Functions starting with `_lv_` (internal)
- Functions in `*_private.h` headers
- Low-level draw primitives (`lv_draw_sw_*`, `lv_draw_buf_*`, `*_init_draw_*`)
- XML parser bindings (`*_xml_*`)
- Widget class implementation details (`*_class_*`)
- Internal event and handler functions

### Internal API compliance

> [!IMPORTANT]
> **lvgl_cpp does NOT call any internal LVGL APIs.** The audit confirms zero violations.

## Summary

| Category | Total | Used | Coverage |
|----------|-------|------|----------|
| **Functions** | 1,836 | 1,003 | **54.6%** |
| **Enums and Constants** | 859 | 296 | **34.5%** |

## Function coverage

### Complete (100%)

| Widget | Functions |
|--------|-----------|
| `lv_textarea` | 37/37 |
| `lv_dropdown` | 24/24 |
| `lv_menu` | 20/20 |
| `lv_bar` | 16/16 |
| `lv_buttonmatrix` | 15/15 |
| `lv_msgbox` | 12/12 |
| `lv_roller` | 11/11 |
| `lv_tabview` | 11/11 |
| `lv_keyboard` | 11/11 |
| `lv_line` | 9/9 |
| `lv_led` | 7/7 |
| `lv_imagebutton` | 6/6 |
| `lv_lottie` | 6/6 |
| `lv_list` | 5/5 |
| `lv_tileview` | 5/5 |
| `lv_win` | 5/5 |
| `lv_checkbox` | 4/4 |
| `lv_observer` | 4/4 |
| `lv_switch` | 3/3 |
| `lv_screen` | 3/3 |
| `lv_async` | 2/2 |
| `lv_spinner` | 2/2 |
| `lv_button` | 1/1 |

### High coverage (>75%)

| Subsystem | Coverage |
|-----------|----------|
| `lv_spinbox` | 95.0% (19/20) |
| `lv_vector` | 93.8% (15/16) |
| `lv_arc` | 92.9% (26/28) |
| `lv_canvas` | 92.9% (13/14) |
| `lv_subject` | 90.6% (29/32) |
| `lv_anim_timeline` | 89.5% (17/19) |
| `lv_label` | 85.7% (18/21) |
| `lv_table` | 82.4% (14/17) |
| `lv_layer` | 80.0% (4/5) |
| `lv_display` | 75.0% (51/68) |

### Medium coverage (25-75%)

| Subsystem | Coverage | Change |
|-----------|----------|--------|
| `lv_spangroup` | 69.2% (18/26) | - |
| `lv_timer` | 68.4% (13/19) | - |
| `lv_scale` | 67.6% (23/34) | - |
| `lv_style` | 65.8% (96/146) | ⬆️ (+2) |
| `lv_group` | 60.9% (14/23) | - |
| `lv_anim` | 59.7% (37/62) | ⬆️ (+7) |
| `lv_chart` | 54.8% (23/42) | - |
| `lv_fs` | 51.4% (19/37) | - |
| `lv_indev` | 50.8% (31/61) | - |
| `lv_image` | 50.0% (28/56) | - |
| `lv_calendar` | 50.0% (11/22) | - |
| `lv_span` | 42.9% (3/7) | - |
| `lv_obj` | 37.6% (175/465) | ⬆️ (+31) |
| `lv_animimg` | 35.7% (5/14) | - |

### Low coverage (<25%)

| `lv_slider` | 23.5% (4/17) | Uses `lv_bar_*` |
| `lv_draw` | 23.2% (52/224) | High-level drawing API |
| `lv_color` | 22.0% (11/50) | - |
| `lv_event` | 17.9% (7/39) | - |
| `lv_font` | 8.3% (2/24) | - |

## Enum and constant coverage

### Complete (100%)

| Category | Values |
|----------|--------|
| `LV_STATE` | 14/14 |
| `LV_KEY` | 12/12 |
| `LV_DIR` | 8/8 |
| `LV_BORDER` | 7/7 |

### High coverage (>75%)

| Category | Coverage |
|----------|----------|
| `LV_ALIGN` | 95.7% (22/23) |
| `LV_EVENT` | 94.4% (68/72) |
| `LV_GRID` | 90.9% (10/11) |
| `LV_PALETTE` | 90.9% (20/22) |
| `LV_PART` | 90.0% (9/10) |
| `LV_FLEX` | 77.8% (14/18) |
| `LV_OPA` | 76.5% (13/17) |
| `LV_ARC` | 75.0% (3/4) |

### Medium/low coverage

| Category | Coverage |
|----------|----------|
| `LV_KEYBOARD` | 72.7% (8/11) |
| `LV_CHART` | 71.4% (10/14) |
| `LV_ROLLER` | 66.7% (2/3) |
| `LV_DISPLAY` | 62.5% (5/8) |
| `LV_BAR` | 57.1% (4/7) |
| `LV_OBJ` | 51.6% (32/62) |
| `LV_TEXT` | 50.0% (7/14) |
| `LV_SLIDER` | 42.9% (3/7) |
| `LV_INDEV` | 40.0% (10/25) |
| `LV_SCALE` | 40.0% (6/15) |
| `LV_SPAN` | 22.2% (2/9) |
| `LV_LABEL` | 7.7% (1/13) |
| `LV_ANIM` | 6.1% (2/33) |

## Priority improvements

1. **`lv_obj_*`** (465 functions): Core object API - layout, scrolling, events
2. **`lv_style_*`** (146 functions): Consider fluent Style builder API
3. **`LV_ANIM_*`** (33 constants): Animation path and playback constants
4. **`lv_chart_*`** (42 functions): Series and cursor manipulation

## References
- [Gap Analysis](gap_analysis.md): Detailed breakdown of functional gaps and missing core features.
- [Missing Object Methods](missing_obj_methods.md): Comprehensive list of user-facing `lv_obj_*` functions missing from the API.

## Regenerating this report

```bash
cd lvgl_cpp
python3 scripts/audit_api_coverage.py ../lvgl .
```

JSON data saved to `docs/api_coverage.json`.
