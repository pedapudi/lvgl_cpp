# Observer Bindings: Type Safety & Lifecycle Management

## 1. Executive Summary

This document details the modernization of the `Subject` and `Observer` systems in `lvgl_cpp` (Issues #201, #203). The goal was to transform the powerful but low-level LVGL observer C API into a type-safe, fluent, and lifecycle-aware C++ framework that integrates seamlessly with the existing widget hierarchy.

**Key Improvements:**
1.  **Observer RAII Wrapper**: All binding methods now return `Observer` objects with clear ownership semantics.
2.  **Lifecycle Hardening**: Integrated widget-bound observers with the widget lifecycle using "unmanaged" ownership defaults.
3.  **Fluent Widget Proxies**: Enabling code like `checkbox.bind_checked(settings_subject)`.
4.  **Type-Safe Bindings**: Support for scoped enums (`ObjFlag`, `State`) in binding conditions.
5.  **Formatted String Subjects**: Correct utilization of `lv_subject_snprintf` for dynamic text synchronization.

---

## 2. Problem Analysis

### 2.1 Raw Pointer Management
The LVGL C API for observers returns `lv_observer_t*`. In C++, this forces the user to choose between holding a raw pointer or creating a complex management structure. If an observer is attached to an object, LVGL deletes it automatically when the object is deleted. If not, the user must manualy delete it.

### 2.2 Lack of Contextual Binding
Previously, bindings were defined solely on the `Subject` class. This reversed the natural flow of UI definition:
```cpp
// Old (Subject-centric)
my_subject.bind_checked(my_checkbox);

// Desired (Widget-centric/Fluent)
my_checkbox.bind_checked(my_subject);
```

### 2.3 Condition Type Safety
Conditional bindings (e.g., "set flag if subject == X") used raw `lv_obj_flag_t` and `int32_t`. This bridged poorly with the `lvgl_cpp` scoped enum system (`ObjFlag`, `State`).

---

## 3. Technical Approach

### 3.1 The `Observer` Managed Wrapper
The `Observer` class encapsulates an `lv_observer_t*` with a boolean `owned_` flag.

```cpp
class Observer {
public:
    explicit Observer(lv_observer_t* obs, bool owned = true);
    ~Observer() {
        if (obs_ && owned_) lv_observer_delete(obs_);
    }
    // ... move semantics ...
};
```

### 3.2 Ownership Strategy: "Unmanaged by Default"
For bindings that attach a subject to a widget (e.g., `lv_obj_bind_checked`), LVGL takes internal ownership of the observer. Therefore, the `lvgl_cpp` wrapper defaults to `owned = false` in these contexts.

This allows the user to:
1.  **Discard the return value**: The binding survives as long as the widget does.
2.  **Capture the return value**: To manually delete the binding early if needed via `Observer::delete_observer()`.

### 3.3 Fluent Widget Proxies
We implemented the `Widget<Derived>` template mixin to provide proxy methods that delegate to the subject's binding logic.

```cpp
// core/widget.h
Observer bind_checked(Subject& subject) {
    return subject.bind_checked(*this);
}
```

### 3.4 String Subject Formatting
The `StringSubject::set_formatted` method was corrected to use `lv_subject_snprintf`. This is critical because `lv_subject_t` manages its own internal buffer for formatted strings, and manual `vsprintf` into a local buffer would lead to memory corruption or truncated displays.

---

## 4. Usage Guide

### 4.1 Simple Boolean Binding
Automatically sync a checkbox state with a hardware setting.

```cpp
lvgl::IntSubject wifi_enabled(0);
lvgl::Checkbox wifi_cb(screen);

// Fluent style
wifi_cb.bind_checked(wifi_enabled);

// Later...
wifi_enabled.set(1); // Checkbox checks automatically
```

### 4.2 Conditional Flag Binding
Hide a "Loading" spinner based on a status subject.

```cpp
lvgl::IntSubject status(STATUS_IDLE);
lvgl::Spinner loader(screen);

// Hide if status is NOT LOADING
loader.bind_flag_if_eq(status, ObjFlag::Hidden, STATUS_IDLE);
```

### 4.3 Formatted Label Binding
Sync a label with an integer value using a template.

```cpp
lvgl::IntSubject battery_lvl(85);
lvgl::Label bat_label(screen);

// Sync text: "Battery: 85%"
bat_label.bind_text(battery_lvl, "Battery: %d%%");
```

---

## 5. Implementation Summary

| Component | Change Description |
| :--- | :--- |
| `Observer` | Added RAII safety with `owned` flag. |
| `Subject` | All `bind_*` methods updated to return `Observer`. |
| `Subject` | Added `bind_style` for conditional styling. |
| `Widget` | Added `bind_flag_if_eq`, `bind_state_if_eq`, `bind_checked`. |
| `StateProxy` | Added `is_checked()`, `set_checked()` convenience helpers. |
| `StringSubject`| Corrected `set_formatted` to use `lv_subject_snprintf`. |

---

## 6. Future Steps

1.  **Lambda Observers**: Implement `Subject::observe(std::function<void(T)>)` to allow arbitrary C++ code to react to subject changes without a widget.
2.  **Grouped Subjects**: Explore `SubjectGroup` for atomic updates to multiple related subjects.
3.  **Complex Conditionals**: Add support for range-based or bitmask-based conditional bindings if requested.
4.  **Observer Vectors**: Provide a helper to track multiple observers in a class (`std::vector<Observer>`) for complex UI controllers.
