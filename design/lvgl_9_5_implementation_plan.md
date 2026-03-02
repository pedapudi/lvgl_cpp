# LVGL 9.5 Migration Implementation Plan

This document breaks down the migration strategy detailed in `design/lvgl_9_5_migration_plan.md` into concrete, step-by-step implementation tasks. It covers backward compatibility, the new Property Interface, structural fixes to SIMD and rendering layers, and exhaustive testing requirements.

## User Review Required

> [!WARNING]
> This plan touches **every single widget** in `lvgl_cpp` via the Property Interface migration. It is highly invasive. We are using preprocessor directives (`#if LVGL_VERSION_MAJOR > 9 || ...`) to maintain ABI and API compatibility with existing ZMK/v9.3 builds. Please review the matrix approach thoroughly.

---

## Proposed Changes

### Core Engine & Compatibility Layer

#### [MODIFY] [compatibility.h](../core/compatibility.h)
- Introduce explicit `#define LVGL_CPP_HAS_PROPERTIES 1` if LVGL >= 9.5.
- Add macros mapping `LV_STATE_ALT` and fallback stubs if missing.
- Define feature macros for `LVGL_CPP_HAS_NATIVE_BLUR` and `LV_CHART_TYPE_CURVE`.

#### [MODIFY] [object.h](../core/object.h)
- Inject the `PropertySetters<WidgetT>` templated mixin that wraps `lv_obj_set_property`.
- Add `.flag_radio_button()` to the fluent builder.
- Add `State::Alt` to the overarching C++ state enums, ensuring it maps to `LV_STATE_ALT`.

#### [MODIFY] [style.h](../misc/style.h)
- Add `.blur()` and `.drop_shadow()` using the generic properties if underlying LVGL >= 9.5.
- Wrap `lv_obj_bind_style_prop` for reactive property binding.

### Display & Render Drivers

#### [MODIFY] [esp32_spi.cpp](../display/drivers/esp32_spi.cpp)
- Validate byte-swapping logic (`flush_swap`) against the new explicit `RGB565_SWAPPED` software operations in v9.5.

#### [MODIFY] [CMakeLists.txt](../CMakeLists.txt)
- Audit the injected SIMD header `LV_DRAW_SW_ASM_CUSTOM_INCLUDE` against the new RISC-V V-extension integration to ensure ESP32-S3 Xtensa assembly routines continue to effectively override default C loops.
- Update the Github Actions CI matrix mappings (done in YAML, but reflected in CMake configurable defaults) to enforce the triple-epoch testing (ZMK v9.3, v9.4 stable, v9.5 edge).

### The Widget Property Interface Migration
### 🏁 Completed & Merged

- [x] SIMD Architecture Refactor (9.5 Native registration)
- [x] Widget Property Interface Migration (Complete)
    - [x] Arc
    - [x] Bar
    - [x] Slider
    - [x] Switch
    - [x] Roller
    - [x] Keyboard
The following 15 files will follow the exact same structural redesign: replacing hard-coded `lv_widget_set_x(obj, val)` with the property macro `LV_PROPERTY_WIDGET_X` wrapped internally via `#if LVGL_CPP_HAS_PROPERTIES`, guaranteeing C++ return types `WidgetT&` are preserved.

#### [x] [arc.cpp](../widgets/arc.cpp)
#### [x] [bar.cpp](../widgets/bar.cpp)
#### [x] [switch.cpp](../widgets/switch.cpp)
#### [x] [checkbox.cpp](../widgets/checkbox.cpp)
#### [x] [led.cpp](../widgets/led.cpp)
#### [x] [line.cpp](../widgets/line.cpp)
#### [x] [scale.cpp](../widgets/scale.cpp)
#### [x] [spinbox.cpp](../widgets/spinbox.cpp)
#### [x] [spinner.cpp](../widgets/spinner.cpp)
#### [x] [table.cpp](../widgets/table.cpp)
#### [x] [tabview.cpp](../widgets/tabview.cpp)
#### [x] [button_matrix.cpp](../widgets/button_matrix.cpp)
#### [x] [span.cpp](../widgets/span.cpp)
#### [x] [menu.cpp](../widgets/menu.cpp)
#### [x] [chart.cpp](../widgets/chart.cpp)
- Chart specific: Implement the new `LV_CHART_TYPE_CURVE` Bézier enum fallback.

### New Features & Miscellaneous Updates

#### [NEW] [arclabel.h](../widgets/arclabel.h)
#### [NEW] [arclabel.cpp](../widgets/arclabel.cpp)
- Wrapper for the newly stabilized `lv_arclabel` widget.

#### [MODIFY] [image_descriptor.cpp](../draw/image_descriptor.cpp)
- Add native recognition of `.webp` files utilizing the new decoder plugin hook.

#### [MODIFY] [input_device.cpp](../indev/input_device.cpp)
- Add `.set_key_remap(...)` and `.set_gesture_threshold(...)` wrappers for the new pointer/keyboard abstractions.

### Testing & Benchmarking Suite

#### [NEW] [test_api_properties.cpp](../tests/test_api_properties.cpp)
- Exhaustive reflections testing of the 15 refactored widgets using the generic property interface.

#### [MODIFY] [test_visual_widgets.cpp](../tests/test_visual_widgets.cpp)
- Add visual screenshots for Bézier curves (`LV_CHART_TYPE_CURVE`) and `State::Alt` theme rendering.

#### [MODIFY] [bench_suite](../bench/bench.cpp)
- Update layout churn profiles to ensure the new Flex + `LV_SIZE_CONTENT` layouts do not regress frametimes.

### Documentation

#### [MODIFY] [MIGRATION_GUIDE.md](../docs/MIGRATION_GUIDE.md)
- Extend the public migration guide for `lvgl_cpp` users.
- Outline the transparent nature of the property interface (no C++ changes required).
- Detail instructions for enabling NanoVG in Linux deployments and Wayland client chrome adjustments.
- Highlight the new `State::Alt`, Blur/Shadow APIs, and new `ArcLabel` widgets available via the fluent interface.

---

### 9. Core Native Enhancements (9.5 Specific)

#### 9.1. 3D Rendering & math
* [x] Implement `misc/math_3d.h` wrapping `lv_3dpoint_t`, `lv_quaternion_t`, etc.
* [x] Implement `widgets/gltf.h` wrapper for `lv_gltf` widget.
* [x] Expose 3D raycasting and coordinate transformation APIs in `Gltf`.

#### 9.2. SIMD Architecture Refactor (9.5+)
* [x] Replace assembly shim hooks with the new `lv_draw_sw_register_blend_handler` API.
* [x] Create `utility/esp32/simd/simd_plugin.cpp` to register handlers at runtime.
* [x] Remove legacy linker `-u` flags from `CMakeLists.txt`.

#### 9.3. Style & Theme Management
* [x] Add `Object::remove_theme()` and `Object::bind_style_prop()`.
* [x] Add `StyleProxy::bind()` for data-driven styling.

### 10. Verification & Benchmarking

#### 10.1. Integration Tests
* [x] Create `tests/test_gltf.cpp` for 3D model interaction.
* [x] Create `tests/test_style_binding.cpp` for observer-based styling.

#### 10.2. Performance Analysis
* [x] Profile SIMD performance using the new blend handler registration method.
* [x] Compare `lv_obj_set_property` vs legacy setters for high-frequency updates.

## Verification Plan

### Automated Tests
- Run `ctest -R test_api_properties` under forced LVGL 9.4 and 9.5 compile definitions.
- Execute `test_visual_widgets` and run `git diff` on the generated hash maps to detect rendering regressions.

### Manual Verification
- Deploy the updated `lvgl_cpp` artifact to an ESP32-S3 dev board. Use `bench_churn` with `LV_USE_PERF_MONITOR` active to confirm the SIMD cache-invalidate logic remains intact and FPS remains stable compared to baseline v9.4 measurements.
