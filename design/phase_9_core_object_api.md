# Phase 9: Core Usage API Expansion

## 1. Objective
Refine and expand the `lv_obj` wrapper coverage in `lvgl_cpp` from ~37% to >80% by implementing missing coordinate, scrolling, interaction, and tree management features.

**Tracking Issue**: #163

## 2. Design Philosophy
- **Idiomatic Types**: Use `lvgl::Area` and `lvgl::Point` (wrappers from `misc/geometry.h`) instead of raw C structs.
- **Proxy Pattern**: Use transient proxies (`ScrollProxy`, `InteractionProxy`, `TreeProxy`) to group related functionality and prevent `Object` class bloat.
- **Fluent API**: Ensure setters returns `*Proxy&` or `Object&` for chaining.

## 3. Implementation Plan

### 3.1. Foundation Types
- Ensure `misc/geometry.h` is fully integrated and available to `core/object.h`.
- Verify `Area` and `Point` have necessary conversion operators and constructors.

### 3.2. Coordinate Systems (`core/object.h`)
Implement getters directly on `Object` as they are fundamental properties.
```cpp
// Object.h
Area get_coords() const;
Area get_content_coords() const;
Area get_click_area() const;
```

### 3.3. Scroll Proxy (`core/scroll_proxy.h`)
Finalize the existing `ScrollProxy` and add proper getters to `Object`.
```cpp
// ScrollProxy
ScrollProxy& mode(ScrollbarMode mode); // Exists
ScrollProxy& dir(Dir dir);             // Exists
ScrollProxy& snap(ScrollSnap x, ...);  // Exists

// Object.h (Getters)
ScrollbarMode get_scrollbar_mode() const;
Dir get_scroll_dir() const;
ScrollSnap get_scroll_snap_x() const;
```

### 3.4. Interaction Proxy (`core/interaction_proxy.h`)
New proxy for input handling and hit testing.
```cpp
class InteractionProxy {
 public:
  explicit InteractionProxy(lv_obj_t* obj);
  InteractionProxy& set_ext_click_area(int32_t size);
  bool hit_test(const Point& point) const;
  bool is_editable() const;
  // ...
};
```

### 3.5. Tree Proxy (`core/tree_proxy.h`)
New proxy for hierarchy manipulation.
```cpp
class TreeProxy {
 public:
  explicit TreeProxy(lv_obj_t* obj);
  TreeProxy& swap(Object& other);
  TreeProxy& move_to_index(int32_t index);
  // ...
};
```

### 3.6. Object Integration
- Add `interaction()` and `tree()` accessor methods to `Object`.
- Add simplified `get_child<T>()`, `get_sibling<T>()` overloads to `Object`.

## 4. Testing Strategy
- Create `tests/test_core_api_phase9.cpp`.
- Verify coordinate transforms using a known child-parent layout.
- Verify scroll settings stick.
- Verify tree swapping changes order (check indices).
