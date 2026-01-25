# Consolidating Geometric and Event Mixins
> **Status**: Implemented (v0.3.0)

## 1. Executive Summary

This document proposes the final phase of the "Systemic Proxy" architecture by consolidating the remaining geometric (`Positionable`, `Sizable`) and event (`EventSource`) mixins into the `Object` base class. 

While heavy properties like Styles and Layouts moved to transient proxies, fundamental properties like **Position**, **Size**, and **Events** are core to every LVGL object. Moving them to the base class eliminates inheritance complexity (CRTP bloat) and ensures these methods are available when high-level widgets are treated as generic objects.

## 2. Issues with Current Mixins

1.  **Inaccessibility**: `obj.set_width()` is only available if `obj` is a `Widget`. If you have a raw `Object` handle (e.g., from `get_parent()`), you cannot set its position or listen to events easily.
2.  **Inheritance Complexity**: Every widget inherits from 4+ classes. This increases symbol size and slows compilation.
3.  **Redundancy**: `Widget` template already has to "forward" these methods to provide a fluent API.

## 3. Proposed Architecture

### 3.1. `Object` as the Truth
All raw LVGL setters will reside in `Object`.

```cpp
class Object {
 public:
  // From Sizable
  Object& set_width(int32_t w);
  Object& set_height(int32_t h);
  
  // From Positionable
  Object& set_x(int32_t x);
  Object& set_y(int32_t y);
  Object& align(Align align, int32_t x = 0, int32_t y = 0);

  // From EventSource
  Object& on_clicked(EventCallback cb);
  // ...
};
```

### 3.2. `Widget` for Ergononmics
`Widget<Derived>` remains the provider of the "Fluent Interface" that returns the concrete type.

```cpp
template <typename Derived>
class Widget : public Object {
 public:
  // Fluent overrides
  Derived& set_width(int32_t w) { 
    Object::set_width(w); 
    return self(); 
  }
  // ...
};
```

## 4. Implementation Steps

1.  **Move Code**: Copy logic from `core/mixins/*.h` to `core/object.h` / `core/object.cpp`.
2.  **Harmonize Naming**: Ensure all methods use the current project naming conventions (e.g., snake_case for properties, CamelCase for Types).
3.  **Delete Mixins**: Remove the files in `core/mixins/`.
4.  **Refactor Widget**: Update `core/widget.h` to remove inheritance from mixins and implement the fluent forwarding methods.
5.  **Verify**: Run `test_core_widgets` and `test_api_hardening` to ensure no regressions.

## 5. Benefits

- **Simpler API**: One way to do things.
- **Improved Performance**: Reduced template instantiation and flatter objects.
- **Consistent DX**: Generic `Object` handles are now fully functional.
