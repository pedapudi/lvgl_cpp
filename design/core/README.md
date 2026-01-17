# Core Architecture Design

## 1. Overview
The Core module is the foundation of `lvgl_cpp`. It handles:
*   Memory Management (RAII, Ownership).
*   The `Object` root class (reduced in scope).
*   The `Widget<T>` template (the new CRTP base).
*   Event Handling (`Event` class, Callbacks).

## 2. The `Object` Class (Refactored)
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
    
    // Parenting operations (Always common)
    void set_parent(Object& parent);
    Object get_parent() const;
    void delete_obj();
    
protected:
    lv_obj_t* obj_ = nullptr;
    Ownership ownership_;
};
```

## 3. The `Widget<T>` Template (New)
The new base for all concrete widgets. It uses CRTP to allow fluent chaining to return the derived type.

### Architecture
```cpp
template <typename Derived>
class Widget : public Object,
               public Positionable<Derived>,
               public Sizable<Derived>,
               public Stylable<Derived>,
               public Clickable<Derived>, // Optional? No, base widgets have events.
               public EventSource<Derived> {
public:
    using Object::Object; // Inherit constructors
    
    Derived& self() { return *static_cast<Derived*>(this); }
};
```

## 4. Mixins
Located in `core/mixins/`.

### `Positionable<T>`
*   `T& x(int32_t)` / `int32_t x()`
*   `T& y(int32_t)` / `int32_t y()`
*   `T& pos(int32_t x, int32_t y)`
*   `T& align(...)`
*   `T& center()`

### `Sizable<T>`
*   `T& width(int32_t)` / `int32_t width()`
*   `T& height(int32_t)` / `int32_t height()`
*   `T& size(int32_t w, int32_t h)`

## 5. Event System
Refactoring `Event` to be purely a safe wrapper.

### `Event` Class
Reference type (does not own event).
*   `lv_event_code_t code()`
*   `Object target()` (returns unmanaged wrapper)
*   `T target<T>()` (safe cast)

### Callback Registration
```cpp
template <typename Derived>
struct EventSource {
    Derived& on_event(lv_event_code_t code, std::function<void(Event)> cb) {
        // Internal registration logic
        return static_cast<Derived&>(*this);
    }
    
    // Semantic Helpers
    Derived& on_click(std::function<void(Event)> cb) {
        return on_event(LV_EVENT_CLICKED, cb);
    }
};
```

## 6. Memory Management Policy
*   **Stack Allocation**: `Button btn(screen);` -> Widget is deleted when `btn` goes out of scope.
*   **Smart Pointers**: `auto btn = std::make_unique<Button>(screen);` -> Lifecycle tied to pointer.
*   **Leak (Manual)**: `new Button(screen); btn->release();` -> User must manually manage C pointer or let LVGL parent cleanup. (Not recommended but supported via "Escape Hatch").
