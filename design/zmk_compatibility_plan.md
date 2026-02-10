# ZMK and Zephyr compatibility plan (LVGL v9.3 support)

This document outlines the strategy for ensuring `lvgl_cpp` remains compatible with ZMK and Zephyr-based projects, which typically utilize LVGL v9.3 (or specific minimal builds) while `lvgl_cpp` targets v9.4.

## Technical objectives
1.  **Enable optional modules**: Ensure the library compiles even when optional LVGL modules (like Observer, Matrix Transformations, or specific widgets) are disabled.
2.  **API version bridging**: Provide fallbacks or mappings for APIs that were introduced or renamed between v9.3 and v9.4, aligning with LVGL's internal `lv_api_map` layers.
3.  **Embedded best practices**: Prioritize LVGL's memory manager and avoid POSIX-specific requirements where possible.

## Compatibility architecture
We will introduce `lvgl_cpp/core/compatibility.h` to centralize version-specific logic and macro mappings. This header will be included in `lvgl_cpp.h` to provide a consistent environment.

## Detailed action plan

### 1. Robust module guarding
- **Observer module**: Wrap all `Subject`, `Observer`, and `on_subject` / `bind` method declarations and implementations in `#if LV_USE_OBSERVER`. This is critical for ZMK where the module is frequently disabled.
- **Matrix transformations**: Guard `set_matrix_rotation` and `get_matrix_rotation` in `Display` with `#if LV_DRAW_TRANSFORM_USE_MATRIX`.
- **Comprehensive widget guarding**: Every widget class in the `widgets/` directory will be wrapped in its corresponding `LV_USE_` guard (e.g., `LV_USE_LABEL`, `LV_USE_IMAGE`, `LV_USE_CHART`). This allows users to strip unused functionality and significantly reduce binary size in memory-constrained environments like Zephyr.
- **Internal subsystems**: Guard features like `LV_USE_SNAPSHOT`, `LV_USE_FILE_EXPLORER`, and `LV_USE_FS_API` to ensure the library is as modular as LVGL itself.

### 2. API bridging and renames
- **Display animations**: In LVGL 9.4, screen loading animations were renamed from `LV_SCR_LOAD_ANIM_*` to `LV_SCREEN_LOAD_ANIM_*`. Since ZMK 9.3 builds lack the new names, we will map them in `compatibility.h`:
  ```cpp
  #ifndef LV_SCREEN_LOAD_ANIM_NONE
  #define LV_SCREEN_LOAD_ANIM_NONE LV_SCR_LOAD_ANIM_NONE
  // ... maps for all anim types (Over, Move, Fade, Out)
  #endif
  ```
- **Label formatting**: `lv_label_set_text_vfmt` was introduced in v9.4. For v9.3 compatibility, we will provide a fallback in `Label::set_text_fmt` that uses a stack-allocated buffer and `vsnprintf`.
- **Image metadata**: `lv_image_get_src_width` and `lv_image_get_src_height` are v9.4 additions. We will implement robust fallbacks that manually inspect the `lv_image_dsc_t` header if the source type is a descriptor.

### 3. Memory management hygiene
- **Allocation migration**: Replace all occurrences of `std::malloc`, `std::free`, and `posix_memalign` with their LVGL equivalents (`lv_malloc` / `lv_free`).
- **Alignment constraints**: For hardware accelerated environments (DMA/SIMD), we will use `lv_draw_buf_malloc` on v9.4+ platforms to ensure proper alignment, with a manual alignment shim for v9.3.
- **Zephyr visibility**: Using `lv_malloc` ensures that memory usage is visible to Zephyr's logging and debug subsystems and adheres to standard memory partition rules.

### 4. File system fallbacks
- **High-level helpers**: Functions like `lv_fs_path_get_size`, `lv_fs_path_join`, and `lv_fs_load_to_buf` were added in v9.4. 
- **In-place reimplementation**: We will reimplement these in `misc/file_system.cpp` using basic v9.3-compliant primitives (`lv_fs_open`, `lv_fs_read`, `lv_fs_seek`, and `lv_fs_tell`). This removes the dependency on v9.4 helper functions while maintaining a consistent C++ API for the user.

### 5. Input device gestures
- **Gesture guarding**: Guard `LV_INDEV_GESTURE_TWO_FINGERS_SWIPE` and other recently added gesture types. 
- **Field existence**: Conditionally compile access to the `timestamp` field in `lv_indev_data_t` based on the LVGL version, as its availability varies across minimal builds.

## Implementation steps
### Phase 1: Core guards
Implement the comprehensive `#if LV_USE_...` wrapping for all widgets and optional core modules.

### Phase 2: Compatibility bridge
Create `lvgl_cpp/core/compatibility.h` with the constant renames and macro-based API mappings.

### Phase 3: Allocator and fallback update
Update `draw/image_descriptor.cpp` to use `lv_malloc` and provide the file system reimplementations in `misc/file_system.cpp`.

### Phase 4: Validation
Validate the build against a simulated LVGL v9.3 environment to ensure zero regressions for ZMK/Zephyr users.
