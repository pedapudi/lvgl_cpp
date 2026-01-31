# Strategic Improvement Plan: Platinum Goal (LVGL v9.4)

## 1. Objective
Convert all "Gold" categorized subsystems (80-99% coverage) to "Platinum" (100% idiomatic coverage) and initiate Phase 1 of the Facet Proxy architecture.

## 2. Phase 1: Gold to Platinum (100% Idiomatic Coverage)

The following components will be brought to 100% coverage by implementing the remaining idiomatic wrappers.

| Component | Missing Function | Proposed C++ Wrapper |
| :--- | :--- | :--- |
| **Label** | `lv_label_set_text_fmt` | `Label::set_text_fmt(const char * fmt, ...)` |
| **Table** | `lv_table_set_cell_value_fmt` | `Table::set_cell_value_fmt(uint32_t row, uint32_t col, const char * fmt, ...)` |
| **Spinbox** | `lv_spinbox_bind_value` | `Spinbox::bind_value(Subject & subject)` |
| **Canvas** | `lv_canvas_buf_size` | `static size_t Canvas::get_buf_size(int32_t w, int32_t h, lv_color_format_t cf)` |
| **Vector** | `lv_vector_path_append_rectangle` | `VectorPath::append_rect(const Area & rect, lv_value_precise_t rx, lv_value_precise_t ry)` |
| **Subject** | `lv_subject_get_group_element` | `Subject::get_group_element(uint32_t index)` |
| **Layer** | `lv_layer_bottom` | `static Layer Layer::bottom()` |

## 3. Phase 2: Facet Proxy Foundation

### 3.1 TreeProxy (Structure Facet)
Implement the `TreeProxy` to provide idiomatic hierarchy management.
- `obj.tree().swap(Object & other)`
- `obj.tree().move_to_index(int32_t index)`
- `obj.tree().get_index()`
- `obj.tree().dump()`

### 3.2 InteractionProxy (Input Facet)
Implement the `InteractionProxy` for hit-testing and interaction metadata.
- `obj.interaction().set_ext_click_area(int32_t size)`
- `obj.interaction().hit_test(const Point & p)`
- `obj.interaction().set_editable(bool en)`

## 4. Verification Plan

### 4.1 Automated Coverage
- Re-run `audit_api_coverage.py` after each component update.
- Goal: **100% Platinum status** for Label, Table, Spinbox, Canvas, Vector, Subject, and Layer.

### 4.2 Unit Testing
- New tests in `tests/test_gold_completion.cpp` for the new wrappers.
- New tests in `tests/test_tree_proxy.cpp` and `tests/test_interaction_proxy.cpp`.

### 4.3 Benchmarking
- Add `bench_core_tree` and `bench_indev_interaction` to `bench_suite` to ensure zero-overhead.
