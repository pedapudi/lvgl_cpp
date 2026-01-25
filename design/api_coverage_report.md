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
| Category | Total | Used | Coverage |
|----------|-------|------|----------|
| **Functions** | 1,460 | 661 | **45.3%** |
| **Enums and Constants** | 423 | 264 | **62.4%** |

## Function coverage

### Complete (100%)

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

| Subsystem | Coverage |
|-----------|----------|
| `lv_spinbox` | 95.0% (19/20) |
| `lv_arc` | 92.9% (26/28) |
| `lv_canvas` | 92.9% (13/14) |
| `lv_keyboard` | 91.7% (11/12) |
| `lv_table` | 82.4% (14/17) |
| `lv_display` | 75.0% (54/72) |

### Medium coverage (25-75%)

| Subsystem | Coverage | Change |
|-----------|----------|--------|
| `lv_scale` | 67.6% (23/34) | ⬆️ (+9) |
| `lv_label` | 61.9% (13/21) | - |
| `lv_timer` | 59.1% (13/22) | - |
| `lv_chart` | 54.8% (23/42) | ⬆️ (+7) |
| `lv_group` | 51.9% (14/27) | ⬆️ (+1) |
| `lv_fs` | 51.4% (19/37) | - |
| `lv_calendar` | 50.0% (11/22) | - |
| `lv_anim` | 47.8% (33/69) | ⬆️ |
| `lv_image` | 45.0% (27/60) | - |
| `lv_style` | 36.3% (53/146)| ⬆️ (+47) |
| `lv_indev` | 31.3% (21/67) | - |

### Low coverage (<25%)

| Subsystem | Coverage | Notes |
|-----------|----------|-------|
| `lv_slider` | 23.5% (4/17) | Uses `lv_bar_*` |
| `lv_color` | 22.0% (11/50) | Color utilities |
| `lv_obj` | 17.8% (90/505) | ⬆️ (+1) |
| `lv_event` | 17.1% (7/41) | ⬆️ (+2) |
| `lv_font` | 8.3% (2/24) | - |

### Not wrapped

| Subsystem | Note |
|-----------|------|
| `lv_span` | Widget not implemented |
| `lv_flex` | Only `lv_flex_init` (system function) |
| `lv_grid` | Only system-level functions |

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
| `LV_SCR_LOAD_ANIM` | 93.8% (15/16) |
| `LV_PALETTE` | 90.9% (20/22) |
| `LV_PART` | 90.0% (9/10) |
| `LV_FLEX` | 77.8% (14/18) |
| `LV_OPA` | 76.5% (13/17) |
| `LV_DISPLAY` | 75.0% (6/8) |
| `LV_ARC` | 75.0% (3/4) |

### Medium/low coverage

| Category | Coverage |
|----------|----------|
| `LV_CHART` | 71.4% (10/14) |
| `LV_KEYBOARD` | 72.7% (8/11) |
| `LV_ROLLER` | 66.7% (2/3) |
| `LV_GRID` | 63.6% (7/11) |
| `LV_BAR` | 57.1% (4/7) |
| `LV_SLIDER` | 42.9% (3/7) |
| `LV_LABEL` | 38.9% (7/18) |
| `LV_TEXT` | 28.6% (4/14) |
| `LV_INDEV` | 25.9% (7/27) |
| `LV_SCALE` | 6.7% (1/15) |
| `LV_ANIM` | 6.1% (2/33) |

### Not used

| Category | Note |
|----------|------|
| `LV_GRAD` | Gradient constants |
| `LV_TABLE` | Table cell flags |
| `LV_DROPDOWN` | Dropdown direction |
| `LV_MENU` | Menu mode constants |

## Priority improvements

1. **`lv_obj_*`** (505 functions): Core object API - layout, scrolling, events
2. **`lv_style_*`** (146 functions): Consider fluent Style builder API
3. **`LV_ANIM_*`** (33 constants): Animation path and playback constants
4. **`lv_chart_*`** (42 functions): Series and cursor manipulation

## Regenerating this report

```bash
cd lvgl_cpp
python3 scripts/audit_api_coverage.py ../lvgl .
```

JSON data saved to `docs/api_coverage.json`.
