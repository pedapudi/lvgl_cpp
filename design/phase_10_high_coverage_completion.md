# Phase 10: High Coverage Subsystem Completion

## Objective
The goal of this phase is to bring subsystems with "High Coverage" (>80%) to 100% API coverage. This involves identifying the few missing functions in each subsystem and implementing idiomatic C++ wrappers for them.

## Targets

The following subsystems have been identified as high-priority targets for completion:

1.  **`lv_anim_timeline`** (89.5% -> 100%)
2.  **`lv_label`** (85.7% -> 100%)
3.  **`lv_arc`** (92.9% -> 100%)
4.  **`lv_table`** (82.4% -> 100%)
5.  **`lv_canvas`** (92.9% -> 100%)
6.  **`lv_vector`** (93.8% -> 100%)
7.  **`lv_subject`** (90.6% -> 100%)

---

## Detailed Implementation Plan

### 1. Animation Timeline (`lv_anim_timeline`)
**Missing:**
- `lv_anim_timeline_get_user_data`
- `lv_anim_timeline_set_user_data`

**Implementation:**
Add `get_user_data` and `set_user_data` to the `AnimationTimeline` class in `misc/animation_timeline.h`.
```cpp
void* get_user_data() const;
void set_user_data(void* user_data);
```

### 2. Label (`lv_label`)
**Missing:**
- `lv_label_set_text_static`
- `lv_label_set_translation_tag`

**Implementation:**
Add these methods to `Label` class in `widgets/label.h`.
- **`set_text_static`**: `Label& set_text_static(const char* text);`
- **`set_translation_tag`**: `Label& set_translation_tag(uint32_t state_id, const char* txt);`

### 3. Arc (`lv_arc`)
**Missing:**
- `lv_arc_align_obj_to_angle`
- `lv_arc_rotate_obj_to_angle`

**Implementation:**
Add methods to `Arc` class in `widgets/arc.h` using idiomatic `Object&` references.
```cpp
void align_obj_to_angle(const Object& obj, int32_t r_offset) const;
void rotate_obj_to_angle(const Object& obj, int32_t r_offset) const;
```

### 4. Table (`lv_table`)
**Missing:**
- `lv_table_get_cell_value`
- `lv_table_set_cell_value_fmt`
- `lv_table_set_selected_cell`

**Implementation:**
Add methods to `Table` class in `widgets/table.h`.
- **`get_cell_value`**: `const char* get_cell_value(uint16_t row, uint16_t col) const;`
- **`set_cell_value_fmt`**: Variadic template wrapper.
  ```cpp
  Table& set_cell_value_fmt(uint16_t row, uint16_t col, const char* fmt, ...);
  ```
- **`set_selected_cell`**: `Table& set_selected_cell(uint16_t row, uint16_t col);`

### 5. Canvas (`lv_canvas`)
**Missing:**
- `lv_canvas_buf_size`

**Implementation:**
Add static utility to `Canvas` class in `widgets/canvas.h`.
```cpp
static size_t get_buf_size(int32_t w, int32_t h, uint8_t bpp, uint8_t stride);
```

### 6. Vector (`lv_vector`)
**Missing:**
- `lv_vector_path_append_rect`

**Implementation:**
The method `lv_vector_path_append_rect` actually takes an `lv_area_t` pointer. We will use our existing `Area` wrapper.
Additionally, we should wrap other descriptors used in `VectorDraw` methods for consistency (e.g. `GradientStop`).

**New Type Wrappers:**
- **`GradientStop`**: Wrapper for `lv_grad_stop_t`.
- **`ImageDescriptor`**: Wrapper for `lv_draw_image_dsc_t`.

**Updates to `VectorPath`:**
```cpp
// In misc/vector.h
void append_rect(const Area& area, float rx, float ry);
```

**Updates to `VectorDraw`:**
Use C++ wrappers for properties.
```cpp
void set_fill_gradient_stops(const std::vector<GradientStop>& stops);
void set_stroke_gradient_stops(const std::vector<GradientStop>& stops);
void set_fill_image(const ImageDescriptor& dsc);
```

### 7. Subject (`lv_subject`)
**Missing:**
- `lv_subject_add_observer_obj`
- `lv_subject_get_group_element`
- `lv_subject_snprintf`

**Implementation:**
Add methods to `Subject` and subgroups in `core/observer.h`.
- **`add_observer_obj`**: Use `ObserverCallback` (std::function) for the C++ wrapper.
  ```cpp
  Observer* add_observer_obj(Object& obj, ObserverCallback cb);
  ```
- **`get_group_element`**: Add to `GroupSubject`.
  ```cpp
  Subject* get_element(int32_t index); // Assuming wrapping raw element back to Subject*
  ```
- **`snprintf`**: Add to `StringSubject`.
  ```cpp
  void set_formatted(const char* fmt, ...);
  ```

## Verification
- Usage of `audit_api_coverage.py` to confirm 100% coverage for these subsystems.
- Compilation checks to ensure template methods (variadic) work correctly.
