# Core architecture design

## 1. Overview
The Core module is the foundation of `lvgl_cpp`. It handles:
*   Memory Management (RAII, Ownership).
*   The `Object` root class (reduced in scope).
*   The `Widget<T>` template (the new CRTP base).
*   Event Handling (`Event` class, Callbacks).

## 2. The `Object` class (refactored)
We will strip `Object` down to its essence: managing the `lv_obj_t*` handle.

### Responsibilities
*   **Handle Storage**: `protected lv_obj_t* obj_`.
*   **Ownership Management**: `Ownership` enum (Managed, Unmanaged).
*   **Lifetime**: `~Object()` calls `lv_obj_delete` if owned.
*   **Raw Access**: `raw()` method (The "Escape Hatch").
*   **Equality**: `operator==` based on `lv_obj_t*` identity.

### API Surface
```cpp
class Object {
public:
    explicit Object(lv_obj_t* obj, Ownership ownership = Ownership::Default);
    virtual ~Object();
    
    // Core Identity
    lv_obj_t* raw() const { return obj_; }
    bool is_valid() const;
    
    // Lifecycle
    void delete_async();
    
    // Geometry (Consolidated from Positionable/Sizable)
    Object& set_pos(int32_t x, int32_t y);
    Object& set_size(int32_t w, int32_t h);
    Object& align(Align align, ...);
    
    // Events (Consolidated from EventSource)
    Object& add_event_cb(lv_event_code_t, EventCallback);
    Object& on_click(EventCallback);
    
    // Proxies (Factories)
    StyleProxy style();
    LayoutProxy layout();
    ScrollProxy scroll();
    
protected:
    lv_obj_t* obj_ = nullptr;
    Ownership ownership_;
};
```

## 3. The `Widget<T>` template (new)
The new base for all concrete widgets. It uses CRTP to allow fluent chaining to return the derived type.

### Architecture
```cpp
template <typename Derived>
class Widget : public Object {
public:
    using Object::Object; // Inherit constructors
    
    // Fluent Proxies (Forward to Object, return Derived&)
    Derived& set_pos(int32_t x, int32_t y) {
        Object::set_pos(x, y);
        return self();
    }
    
    Derived& self() { return *static_cast<Derived*>(this); }
};
```

## 4. Mixins (Consolidated)
Former mixins (`Positionable`, `Sizable`, `EventSource`) have been consolidated into `Object` to reduce inheritance complexity. `Stylable`, `Layoutable`, and `Scrollable` have been replaced by the Proxy architecture.

## 5. Event system
Refactoring `Event` to be purely a safe wrapper.

### `Event` class
Reference type (does not own event).
*   `lv_event_code_t code()`
*   `Object target()` (returns unmanaged wrapper)
*   `T target<T>()` (safe cast)

### Callback registration
### Callback registration
Event registration is now built directly into `Object`.

```cpp
Object& obj = ...;
obj.on_click([](Event& e){ ... });
```

## 6. Memory management policy
*   **Stack Allocation**: `Button btn(screen);` -> Widget is deleted when `btn` goes out of scope.
*   **Smart Pointers**: `auto btn = std::make_unique<Button>(screen);` -> Lifecycle tied to pointer.
*   **Leak (Manual)**: `new Button(screen); btn->release();` -> User must manually manage C pointer or let LVGL parent cleanup. (Not recommended but supported via "Escape Hatch").
