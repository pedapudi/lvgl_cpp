# LVGL 9.5 Migration & Backward Compatibility Architecture

## 1. Executive Summary
The transition from LVGL v9.4 (and v9.3 for ZMK environments) to LVGL v9.5 requires a profound architectural shift within `lvgl_cpp`. LVGL v9.5 represents a major maturation of the library, introducing a unified Property Interface, dropping legacy Wayland client decorations, integrating NanoVG for vector graphics, and standardizing properties across 15 core widgets.

This plan details how to systematically upgrade `lvgl_cpp` to leverage v9.5 capabilities natively, while strictly preserving compilation and behavioral correctness against v9.4 and minimal v9.3 configurations.

---

## 2. Global Architectural Strategy

### 2.1 The Unified Property Interface
LVGL 9.5 rolls out a unified `lv_obj_set_property(obj, property_id, value)` interface for 15 core widgets. This standardizes property assignments, replacing hundreds of disparate setter functions.

**Impact on `lvgl_cpp`:**
Currently, `lvgl_cpp` wraps each discrete C-function (e.g., `lv_slider_set_value`) in a type-safe C++ method. We must adopt a hybrid approach:
1. **Core Property Mixin:** Implement a templated mixin `PropertySetters<WidgetT>` that defines `.set_property(...)`.
2. **Version Checks and Fluent Builders:** For native LVGL 9.5 compilation, the widget's existing fluent builders (like `.value(x)`) will strictly preserve their C++ return types (returning `WidgetT&`). Internally, they will bridge to the property interface. For LVGL < 9.5, they will branch to legacy functions. This guarantees that **no downstream `lvgl_cpp` applications will break**; the idiomatic chaining and strong typing remain untouched.

```cpp
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
// LVGL 9.5+ Implementation via Properties
template <typename T>
inline T& set_widget_property(T& widget, lv_property_id_t id, const lv_property_value_t& val) {
    lv_obj_set_property(widget.raw(), id, &val);
    return widget;
}
#else
// Legacy API mappings fallback handled per-widget
#endif
```

### 2.2 Style and Theme Enhancements
LVGL 9.5 introduces `LV_STATE_ALT` for streamlined dark/light mode toggles without duplicate theme trees, and native Blur and Drop Shadow APIs.

**Action Items:**
- **State Enum Extension:** Add `State::Alt` to `lvgl_cpp::style::State` mapped to `LV_STATE_ALT`.
- **Blur & Shadow Setters:** Inject `.blur()` and `.drop_shadow()` to the `lvgl_cpp::style::Style` class, guarded by `#if LVGL_VERSION_MINOR >= 5`.
- **Dynamic Themes:** Introduce a wrapper for `lv_obj_bind_style_prop` to allow localized reactive styling beyond the global theme context.

### 2.3 Rendering Engines: NanoVG & Wayland
- **NanoVG:** Complete OpenGL draw unit providing GPU-accelerated vector rendering. If compiling `lvgl_cpp` for Linux/OpenGL, the draw initialization sequence must now query NanoVG availability and skip legacy software masking.
- **Wayland:** The driver was rewritten. Client-side window decorations are gone. If `lvgl_cpp` implemented Wayland window chromes, we must migrate to drawing `lv_win` widgets manually.

---

## 3. Widget-by-Widget Migration Plan

The following section defines the exact modernization pipeline for widgets affected by the Property Interface rollout in LVGL 9.5.

### 3.1 Arc (`lv_arc`)
**Overview:** Angles, rotation, and configurable backgrounds.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::Arc` mapped calls like `.set_value(val)` to `lv_arc_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `Arc` implements `LV_PROPERTY_ARC_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_ARC_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_arc_...` function bounds.

**Code Example for Arc:**
```cpp
Arc& Arc::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_ARC_VALUE, &pv);
#else
    lv_arc_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```

### 3.2 Bar (`lv_bar`)
**Overview:** Value indicators, min/max ranges, and progress tracking.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::Bar` mapped calls like `.set_value(val)` to `lv_bar_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `Bar` implements `LV_PROPERTY_BAR_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_BAR_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_bar_...` function bounds.

**Code Example for Bar:**
```cpp
Bar& Bar::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_BAR_VALUE, &pv);
#else
    lv_bar_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```

### 3.3 Switch (`lv_switch`)
**Overview:** Boolean toggles with animations.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::Switch` mapped calls like `.set_value(val)` to `lv_switch_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `Switch` implements `LV_PROPERTY_SWITCH_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_SWITCH_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_switch_...` function bounds.

**Code Example for Switch:**
```cpp
Switch& Switch::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_SWITCH_VALUE, &pv);
#else
    lv_switch_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```

### 3.4 Checkbox (`lv_checkbox`)
**Overview:** State toggles with text labels.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::Checkbox` mapped calls like `.set_value(val)` to `lv_checkbox_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `Checkbox` implements `LV_PROPERTY_CHECKBOX_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_CHECKBOX_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_checkbox_...` function bounds.

**Code Example for Checkbox:**
```cpp
Checkbox& Checkbox::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_CHECKBOX_VALUE, &pv);
#else
    lv_checkbox_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```

### 3.5 LED (`lv_led`)
**Overview:** Brightness and color-based indicators.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::LED` mapped calls like `.set_value(val)` to `lv_led_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `LED` implements `LV_PROPERTY_LED_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_LED_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_led_...` function bounds.

**Code Example for LED:**
```cpp
LED& LED::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_LED_VALUE, &pv);
#else
    lv_led_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```

### 3.6 Line (`lv_line`)
**Overview:** Vector graphics with configurable point arrays.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::Line` mapped calls like `.set_value(val)` to `lv_line_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `Line` implements `LV_PROPERTY_LINE_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_LINE_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_line_...` function bounds.

**Code Example for Line:**
```cpp
Line& Line::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_LINE_VALUE, &pv);
#else
    lv_line_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```

### 3.7 Scale (`lv_scale`)
**Overview:** Tick marks, labels, and needle indicators for meters/gauges.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::Scale` mapped calls like `.set_value(val)` to `lv_scale_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `Scale` implements `LV_PROPERTY_SCALE_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_SCALE_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_scale_...` function bounds.

**Code Example for Scale:**
```cpp
Scale& Scale::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_SCALE_VALUE, &pv);
#else
    lv_scale_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```

### 3.8 Spinbox (`lv_spinbox`)
**Overview:** Digit-by-digit numerical input.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::Spinbox` mapped calls like `.set_value(val)` to `lv_spinbox_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `Spinbox` implements `LV_PROPERTY_SPINBOX_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_SPINBOX_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_spinbox_...` function bounds.

**Code Example for Spinbox:**
```cpp
Spinbox& Spinbox::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_SPINBOX_VALUE, &pv);
#else
    lv_spinbox_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```

### 3.9 Spinner (`lv_spinner`)
**Overview:** Indeterminate progress arcs.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::Spinner` mapped calls like `.set_value(val)` to `lv_spinner_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `Spinner` implements `LV_PROPERTY_SPINNER_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_SPINNER_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_spinner_...` function bounds.

**Code Example for Spinner:**
```cpp
Spinner& Spinner::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_SPINNER_VALUE, &pv);
#else
    lv_spinner_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```

### 3.10 Table (`lv_table`)
**Overview:** Cell-based grids with text formatting and spans.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::Table` mapped calls like `.set_value(val)` to `lv_table_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `Table` implements `LV_PROPERTY_TABLE_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_TABLE_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_table_...` function bounds.

**Code Example for Table:**
```cpp
Table& Table::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_TABLE_VALUE, &pv);
#else
    lv_table_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```

### 3.11 Tabview (`lv_tabview`)
**Overview:** Multi-page containers with scrolling headers.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::Tabview` mapped calls like `.set_value(val)` to `lv_tabview_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `Tabview` implements `LV_PROPERTY_TABVIEW_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_TABVIEW_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_tabview_...` function bounds.

**Code Example for Tabview:**
```cpp
Tabview& Tabview::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_TABVIEW_VALUE, &pv);
#else
    lv_tabview_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```

### 3.12 ButtonMatrix (`lv_buttonmatrix`)
**Overview:** Grid of interactive mapped buttons.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::ButtonMatrix` mapped calls like `.set_value(val)` to `lv_buttonmatrix_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `ButtonMatrix` implements `LV_PROPERTY_BUTTONMATRIX_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_BUTTONMATRIX_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_buttonmatrix_...` function bounds.

**Code Example for ButtonMatrix:**
```cpp
ButtonMatrix& ButtonMatrix::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_BUTTONMATRIX_VALUE, &pv);
#else
    lv_buttonmatrix_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```

### 3.13 Span (`lv_span`)
**Overview:** Rich-text contiguous layouts.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::Span` mapped calls like `.set_value(val)` to `lv_span_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `Span` implements `LV_PROPERTY_SPAN_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_SPAN_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_span_...` function bounds.

**Code Example for Span:**
```cpp
Span& Span::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_SPAN_VALUE, &pv);
#else
    lv_span_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```

### 3.14 Menu (`lv_menu`)
**Overview:** Hierarchical navigation containers.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::Menu` mapped calls like `.set_value(val)` to `lv_menu_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `Menu` implements `LV_PROPERTY_MENU_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_MENU_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_menu_...` function bounds.

**Code Example for Menu:**
```cpp
Menu& Menu::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_MENU_VALUE, &pv);
#else
    lv_menu_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```

### 3.15 Chart (`lv_chart`)
**Overview:** Data plotting with new Bézier curve support in v9.5.

**LVGL 9.3/9.4 Legacy Mapping:**
Historically, `lvgl_cpp::widgets::Chart` mapped calls like `.set_value(val)` to `lv_chart_set_value(obj, val)`. This required an immense amount of boilerplate in `lvgl_cpp`.

**LVGL 9.5 Property Integration:**
In 9.5, `Chart` implements `LV_PROPERTY_CHART_*` constants. 
- **Migration Task:** We will replace the internal `#ifdef` blocks in the C++ wrapper to route standard builder methods to `LV_PROPERTY_CHART_...` when compiled against 9.5.
- **Backward Compatibility:** If `LV_VERSION_CHECK(9, 5, 0)` fails, macro expansion must emit the legacy `lv_chart_...` function bounds.

**Code Example for Chart:**
```cpp
Chart& Chart::value(int32_t val) {
#if LVGL_VERSION_MAJOR > 9 || (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5)
    lv_property_value_t pv; pv.num = val;
    lv_obj_set_property(this->obj, LV_PROPERTY_CHART_VALUE, &pv);
#else
    lv_chart_set_value(this->obj, val, LV_ANIM_OFF); // Legacy
#endif
    return *this;
}
```


---

## 4. Advanced Subsystems Migration

### 4.1 Grid and Flex Layout Resolvers
In 9.5, Flex layouts natively support `LV_PCT` and `LV_SIZE_CONTENT` combinations previously deemed invalid. `lvgl_cpp` must remove its internal constraint assertions that previously warned users against this combination.

### 4.2 File System and Image Decoders
With the unified WebP decoder and upgraded BMP/JPEG cache in 9.5:
- `lvgl_cpp::draw::Image` must natively accept WebP payloads. The `Source` variant constructor needs an update to validate `.webp` extensions and link to the explicit cache flusher when decoding dynamic textures.

### 4.3 3D Rendering (glTF)
LVGL 9.5 introduces runtime glTF manipulation.
- **Migration Task:** `lvgl_cpp` currently has no `glTF` widget abstraction. We must create a new component: `lvgl_cpp::widgets::Model3D`. This must include raycasting coordinate translation methods to convert 2D window taps into 3D ray intersections. 

---

## 5. Verification & Testing

### 5.1 Compilation Matrix
To ensure the `#ifdef` macros operate perfectly, we will enforce a strict CI matrix:
- **ZMK Baseline (LVGL 9.3):** Compiles without the Observer module, no properties, missing modern FS APIs.
- **Standard (LVGL 9.4):** Compiles with observers but legacy setters.
- **Modern (LVGL 9.5):** Compiles utilizing the full Property Interface.

### 5.2 Behavioral Tests
We will add standard `catch2` tests for:
- Property assignment regressions on the 15 updated widgets.
- `State::Alt` bitwise resolution within styles.
- Assertion checks on `LV_SIZE_CONTENT` + Flex combinations.

---
*Migration plan generated to guide the lvgl_cpp API transition.*


---

## 6. Meta-Analysis of LVGL 9.5 Coverage

To ensure 100% architectural coverage, we must audit our planned updates against the entirety of the LVGL 9.5 changelog.

### Addressed in Above Plan:
1. **Property Interface Rollout:** Covered extensively in Section 3 for 15 widgets.
2. **Native Blur & Drop Shadow:** Addressed via `Style` property additions.
3. **NanoVG & Wayland EGL:** Addressed in Section 2.
4. **LV_STATE_ALT:** Addressed by extending `lvgl_cpp::style::State`.
5. **Image Decoders (WebP):** Abstracted through `lvgl_cpp::draw::Image` source variants.
6. **Bézier Curved Charts:** The `.type(LV_CHART_TYPE_CURVE)` must be wrapped in a version-checked method.

### Additional Required Coverages (Newly Identified):
1. **Key Remapping & Gesture Thresholds:** In v9.5, `lv_indev_t` supports new indev-level remapping and threshold configurations. Our `InputDevice` base class and specific `Keyboard`/`Pointer` wrappers must expose `.set_key_remap(...)` and `.set_gesture_threshold(...)`.
2. **GStreamer Audio-Free Sources:** If `lvgl_cpp` exposes video wrappers in the future, it must handle the new stream end events and audio-less decoder fallbacks.
3. **Timer Event Resumption:** `lv_timer` behavior changed to resume handler after attribute changes. `lvgl_cpp::misc::Timer` wrap behavior is unaffected but tests must be validated against the new cycle ticks.
4. **Widget-Specific Oddities:**
   - **ArcLabel:** A new widget `lv_arclabel` was stabilized. `lvgl_cpp::widgets::ArcLabel` must be built.
   - **Line Vectors:** Line can now take an array of points in the draw task directly.
   - **Object Flags:** `LV_OBJ_FLAG_RADIO_BUTTON` added to easily create radio groups. We must add `.flag_radio_button()` to the core `Object` fluent builder.

---

## 7. SIMD Optimization Reconciliation Strategy

### 7.1 Current Architecture (`lvgl_cpp` ESP32-S3 Shim)
`lvgl_cpp` maintains custom Xtensa assembly routines for the ESP32-S3 (e.g., `lv_color_blend_to_rgb565_esp32s3.S`). It injects these into the LVGL build via `CMakeLists.txt` using:
- `LV_USE_DRAW_SW_ASM=255`
- `LV_DRAW_SW_ASM_CUSTOM_INCLUDE="lv_draw_sw_asm_custom.h"`
This guarantees `lvgl_cpp`'s highly optimized vector instructions overwrite LVGL's default C implementations.

### 7.2 LVGL 9.5 Changes & Risks
LVGL v9.5 officially introduces the **RISC-V Vector Extension (`LV_DRAW_SW_ASM_RISCV_V`)** for software drawing. This refactored aspects of the `lv_draw_sw_asm` dispatch tables to dynamically handle SIMD capabilities. 

**Risk:** The introduction of standard RISC-V V-extension hooks may have caused the internal `LV_DRAW_SW_ASM_CUSTOM` hook (`255`) and the signatures of blending functions (e.g., `lv_color_blend_to_rgb565_shim`) to diverge or require new alignment macros.

### 7.3 Reconciliation Execution Plan
1. **Signature Audit:** Immediately compare the function prototypes in `../utility/esp32/simd/lv_draw_sw_asm_custom.h` against the 9.5 `src/draw/sw/blend/lv_draw_sw_blend_to_*.h` definitions.
2. **Custom Hook Verification:** Verify `LV_USE_DRAW_SW_ASM=255` is still honored by the LVGL 9.5 Kconfig and CMake scripts. If LVGL deprecated `255` in favor of a specific registration API, we must migrate the `CMakeLists.txt` compile definitions.
3. **RGB565 Swapped Format:** LVGL 9.5 introduces explicit fixes for `RGB565_SWAPPED` rendering in software blur and blending. Our ESP32-S3 SIMD routines currently assume standard RGB565. We must ensure the SPI drivers (`esp32_spi.cpp`) which handle byte-swapping (`flush_swap`) do not implicitly conflict with new 9.5 swapped representations when using our custom ASM.
4. **Cache Alignment:** 9.5 adds strict D-Cache flush requirements spanning `draw_buf`. We must audit `lv_draw_sw_asm_shim.c` to ensure we invoke `lv_draw_buf_invalidate_cache()` before executing Xtensa assembly loops to prevent stale artifacts, especially on ESP32-S3 with PSRAM.

---

## 8. Platinum Coverage Testing Strategy

To guarantee "platinum" coverage and assure regressions are non-existent when migrating `lvgl_cpp` to LVGL 9.5, we will deploy a multi-faceted testing implementation plan leveraging our CI/CD pipelines and the `tests/` subsystem.

### 8.1 API Surface Exhaustive Sweeps
- **Property Reflection Tests:** A new `test_api_properties.cpp` suite will iterate through every `LV_PROPERTY_*` constant for the 15 updated widgets, asserting that calling `widget.set_property(id, val)` matches the side-effects of `widget.legacy_setter(val)`. 
- **Type-Safety Asserts:** `lvgl_cpp` strongly-typed enums (like `lvgl_cpp::style::State::Alt`) will be compile-time checked (`static_assert`) against the underlying C enum values (`LV_STATE_ALT`) to catch silent breakage in headers.

### 8.2 Compilation Matrix Testing
Our CMake configurations will be extended to test all three epochs of compatibility in isolated matrix environments:
1. **ZMK Stub Environment (v9.3):** Simulates the Zephyr/ZMK restrictive environment. Asserts that missing APIs (like new FS) gracefully fallback or stub, and that the `LV_USE_OBSERVER=0` macro avoids compile errors.
2. **Current Stable (v9.4):** Compiles and runs existing visual widget verification.
3. **Modern (v9.5 Edge):** Compiles with properties enabled, enforcing NanoVG and Wayland updates.

### 8.3 Visual Regression Benchmark Tests
We will add screenshot comparison tasks to the existing `test_visual_widgets.cpp` harness:
- Render complex views involving the new Bézier curved charts and advanced arc properties.
- Hash the framebuffer outputs under 9.4 and 9.5 rendering paths.
- Tolerate anti-aliasing delta, but fail on layout regressions caused by properties or Flex `LV_SIZE_CONTENT` combinations.

### 8.4 Hardware-in-Loop Evaluation (ESP32-S3)
For the SIMD reconciliation strategy:
- Deploy the benchmarking artifact (`bench_suite`) to physical ESP32-S3 boards.
- Capture frametimes using `LV_USE_PERF_MONITOR` for standard C blending vs our custom `lv_draw_sw_asm_shim.c` under 9.5.
- Require identical or improved FPS numbers before merging the 9.5 compatibility PR.
