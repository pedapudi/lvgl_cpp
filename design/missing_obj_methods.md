# Missing LVGL Object Methods Audit

## 1. Executive Summary
This document provides a comprehensive list of user-facing `lv_obj_*` functions from LVGL v9.4 that are currently missing in `lvgl_cpp`. It maps each missing function to its recommended implementation location within the C++ wrapper library to guide Phase 9 (Core Coverage).

**Audit Date**: 2026-01-27
**Coverage Gap**: ~63% of user-facing core object API.

## 2. Missing functionality by category

### 2.1. Coordinate Systems & Transformations
**Header**: `lv_obj_pos.h`
**Wrapper Location**: `core/object.h` (for core getters) & `core/transform_proxy.h` (for advanced ops)

*Design Principle*: Use `lvgl::Area` and `lvgl::Point` wrappers (from `misc/geometry.h`) instead of raw C structs.

| Function | Description | Recommended C++ API |
| :--- | :--- | :--- |
| `lv_obj_get_coords` | Get valid absolute coordinates on screen. | `Area get_coords() const` |
| `lv_obj_get_content_coords` | Get coordinates of content area (minus padding). | `Area get_content_coords() const` |
| `lv_obj_get_click_area` | Get the area responsive to input. | `Area get_click_area() const` |
| `lv_obj_transform_point` | Transform a point based on object's zoom/angle. | `Point transform_point(const Point& p, bool recursive, bool inverse) const` |
| `lv_obj_get_transformed_area` | Get area after transformation. | `Area get_transformed_area(const Area& area, bool recursive, bool inverse) const` |
| `lv_obj_invalidate_area` | Invalidate a specific area of the object. | `void invalidate_area(const Area& area)` |
| `lv_obj_area_is_visible` | Check if an area is visible on screen. | `bool is_area_visible(const Area& area) const` |

### 2.2. Scrolling Configuration
**Header**: `lv_obj_scroll.h`
**Wrapper Location**: `core/scroll_proxy.h` & `core/object.h`

*Design Principle*: Setters belong on the existing `ScrollProxy` (fluent API). Getters belong on `Object` as direct property accessors.

| Function | Description | Recommended C++ API |
| :--- | :--- | :--- |
| `lv_obj_set_scrollbar_mode` | Set when scrollbars are visible. | `ScrollProxy& mode(ScrollbarMode mode)` (Exists) |
| `lv_obj_set_scroll_dir` | Set allowed scroll directions. | `ScrollProxy& dir(Dir dir)` (Exists) |
| `lv_obj_set_scroll_snap_x` | Set horizontal snap behavior. | `ScrollProxy& snap(ScrollSnap x, ScrollSnap y)` (Exists) |
| `lv_obj_get_scrollbar_mode` | Get current scrollbar mode. | *Object:* `ScrollbarMode get_scrollbar_mode() const` |
| `lv_obj_get_scroll_dir` | Get allowed scroll directions. | *Object:* `Dir get_scroll_dir() const` |
| `lv_obj_get_scroll_snap_x` | Get horizontal snap alignment. | *Object:* `ScrollSnap get_scroll_snap_x() const` |

### 2.3. Interaction & Hit Testing
**Header**: `lv_obj.h`, `lv_obj_draw.h`
**Wrapper Location**: `core/interaction_proxy.h` (New)

*Design Principle*: Group interaction-specific methods into a new `InteractionProxy` (`obj.interaction()`) to prevent `Object` bloat.

| Function | Description | Recommended C++ API |
| :--- | :--- | :--- |
| `lv_obj_set_ext_click_area` | Increase clickable area outside object bounds. | `InteractionProxy& set_ext_click_area(int32_t size)` |
| `lv_obj_get_ext_click_area` | Get the extended click area size. | `void get_ext_click_area(Area& area) const` |
| `lv_obj_hit_test` | Check if a point is within the object. | `bool hit_test(const Point& point) const` |
| `lv_obj_is_editable` | Check if object can be edited by encoder/keypad. | `bool is_editable() const` |
| `lv_obj_is_group_def` | Check if object is the default in its group. | `bool is_group_def() const` |

### 2.4. Drawing & Refresh
**Header**: `lv_obj_draw.h`, `lv_refr.h`
**Wrapper Location**: `core/object.h`

*Design Principle*: These are rare/expert methods. Keep on `Object` but consider a `DrawProxy` if list grows.

| Function | Description | Recommended C++ API |
| :--- | :--- | :--- |
| `lv_obj_refresh_ext_draw_size` | Notify LVGL that extra draw size (shadow/outline) changed. | `void refresh_ext_draw_size()` |
| `lv_obj_get_ext_draw_size` | Get the extra size required for drawing. | `int32_t get_ext_draw_size() const` |
| `lv_obj_get_layer_type` | Get whether object is drawn on a separate layer. | `lv_layer_type_t get_layer_type() const` |
| `lv_obj_redraw` | Force immediate redraw. | `void redraw(lv_layer_t* layer)` |

### 2.5. Tree & Hierarchy Operations
**Header**: `lv_obj_tree.h`
**Wrapper Location**: `core/tree_proxy.h` (New)

*Design Principle*: Group complex hierarchy operations into a new `TreeProxy` (`obj.tree()`). Common getters (`get_parent`, `get_child`) remain on `Object`.

| Function | Description | Recommended C++ API |
| :--- | :--- | :--- |
| `lv_obj_swap` | Swap two objects in the hierarchy. | `TreeProxy& swap(Object& other)` |
| `lv_obj_move_to_index` | Move object to a specific child index. | `TreeProxy& move_to_index(int32_t index)` |
| `lv_obj_get_child_by_type` | Find child of specific class. | `Object get_child(int32_t index) const` (Overload on Object) |
| `lv_obj_get_sibling` | Get next/prev sibling. | `Object get_sibling(int32_t offset) const` (On Object) |
| `lv_obj_get_sibling_by_type` | Get next/prev sibling of specific class. | `template<typename T> Object get_sibling(int32_t offset) const` (On Object) |
| `lv_obj_get_child_count_by_type` | Count children of specific class. | `template<typename T> uint32_t get_child_count() const` (On Object) |

### 2.6. User Data & ID
**Header**: `lv_obj.h`
**Wrapper Location**: `core/object.h`

| Function | Description | Recommended C++ API |
| :--- | :--- | :--- |
| `lv_obj_set_id` | Set custom ID generic pointer. | `void set_id(void* id)` |
| `lv_obj_get_id` | Get custom ID. | `void* get_id() const` |
| `lv_obj_assign_id` | Assign auto-generated ID. | `void assign_id()` |
| `lv_obj_stringify_id` | Get string representation of ID. | `std::string to_string_id() const` |

## 3. Implementation Priorities

1.  **Proxies & Types**: Ensure `misc/geometry.h` is available in Core and create `InteractionProxy`/`TreeProxy` structures.
2.  **Scrolling**: Getters on `Object`.
3.  **Core Coordinates**: `get_coords`, `get_content_coords` using `Area`.

## 4. References
- LVGL v9.4 Core Documentation
- `lvgl/src/core/*.h` source headers
- `lvgl_cpp` design documents
