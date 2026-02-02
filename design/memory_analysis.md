# Memory overhead analysis: lvgl_cpp vs lvgl

**Tracking Issue:** #67
**Implementation Details**: See [optimizations.md](optimizations.md).

## Executive Summary

The `lvgl_cpp` wrapper introduces a **Fixed Overhead** of ~24 bytes per widget (for the `callback_nodes_` vector) and a **Variable Overhead** of ~40 bytes per callback.
**STATUS: RESOLVED**. The previously identified "Systemic Memory Leak" (Issue #67) has been fixed in v0.3.0. The `Object` class now fully owns all registered event callbacks using a `std::vector<std::unique_ptr<CallbackNode>>` container, ensuring RAII compliance.

## 1. Static analysis: Base overhead

### Per-object fixed costs
Every widget in C++ (e.g., `lvgl::Button`) pairs an underlying C `lv_obj_t` with a C++ wrapper.

| Component | Size (32-bit arch) | Description |
| :--- | :--- | :--- |
| `vptr` | 4 bytes | Virtual table pointer (`Object` is virtual). |
| `lv_obj_t* obj_` | 4 bytes | Pointer to LVGL core object. |
| `bool owned_` | 1 byte (+3 padding) | Ownership flag. |
| `std::vector` | 12 bytes | `callback_nodes_` vector header (in `Object`). |
| **Total** | **~24 bytes** | **Stack/Members only. Does not include heap.** |

*Note: If `lvgl::Object` is allocated on the heap (e.g., `new Button()`), add ~8-16 bytes allocator overhead.*

### Per-callback variable costs
The event system relies on `std::function` and heap-allocated wrappers.

| Component | Size | Description |
| :--- | :--- | :--- |
| `CallbackNode` allocation | 4 bytes + overhead | The pointer/wrapper storage. |
| `std::function` | ~32 bytes | Stores the callable/lambda. |
| Lambda Capures | Variable | Context captured by the lambda. |
| **Total** | **~40-60 bytes** | **Per event handler registered.** |

---

## 2. Component-level analysis

Detailed audit of `widgets/` headers reveals that **No Standard Widgets add extra member variables**.
All state is stored in the underlying C `lv_obj_t`. The C++ class is purely a behavior wrapper.

| Widget | C++ Members | Extra Overhead | Notes |
| :--- | :--- | :--- | :--- |
| `Button` | 0 | 0 bytes | Inherits `Object`. Pure wrapper. |
| `Label` | 0 | 0 bytes | Pure wrapper. |
| `Table` | 0 | 0 bytes | Helper methods only. |
| `Chart` | 0 | 0 bytes | Helper methods only. |
| `TextArea` | 0 | 0 bytes | Buffer managed by C core. |
| `Canvas` | 0 | 0 bytes | Buffer pointer managed by C core. |
| `Menu` | 0 | 0 bytes | Complex helpers, but no instance state. |

**Conclusion**: The overhead is uniform. You do not pay "extra C++ tax" for using complex widgets, other than the base `Object` tax.

---

3. Resolved: Ownership Transfer

### The Fix (v0.3.0)
The standard `Object` class now includes a `std::vector<std::unique_ptr<CallbackNode>> callback_nodes_` member.
- **Ownership**: The C++ `Object` wrapper owns the closures.
- **Lifecycle**: When the C++ wrapper is destroyed, the vector is cleared, destroying all lambdas.
- **Safety**: The C callback wrapper (`event_proxy`) handles the bridging.

**Result:** "Orphaned State" is eliminated.

**Result:** we have "Orphaned State"—memory that is owned by no one.

### Systemic Failure Mode
```cpp
// core/mixins/event_source.h
Derived& add_event_cb(EventCallback cb, lv_event_code_t filter) {
    auto wrapper = new EventCallbackWrapper{cb}; // <--- Allocation without Owner
    lv_obj_add_event_cb(..., wrapper); 
    // Wrapper pointer is lost to the C++ side immediately.
    return *static_cast<Derived*>(this);
}
```

### Impact Analysis
- **Leak Rate**: ~40-64 bytes per event registration.
- **Severity**: Critical. Dynamic UI creation/destruction cycles will inevitably exhaust heap.
- **Pattern Violation**: Violates RAII (Resource Acquisition Is Initialization). Resources are acquired but never tied to an object lifetime.

---

## 4. Hardware context and constraints

Analysis for Espressif chips (commonly used with LVGL).

### Scenario: "Rich UI"
1 Screen, 20 Buttons, 5 Labels, 2 Charts. Total 27 Widgets. 30 Callbacks.

### Memory map impact

| Board | SRAM | PSRAM | C++ Overhead (Est.) | Impact Assessment |
| :--- | :--- | :--- | :--- | :--- |
| **ESP32-WROOM** | 320KB~ | None | ~1.5 KB | **Negligible**. (0.5% of RAM). |
| **ESP32-S3 (WROOM)** | 512KB | 2MB+ | ~1.5 KB | **None**. |
| **ESP32-C3** | 400KB | None | ~1.5 KB | **Negligible**. |
| **Seeed Xiao C3** | 400KB | None | ~1.5 KB | **Negligible**. |
| **Seeed Xiao S3** | 512KB | 2-8MB | ~1.5 KB | **None**. |

### The "Danger zone"
The overhead becomes critical only on extremely constrained devices (e.g., STM32F103 with 20KB RAM) where 1.5KB is >5% of system memory. For ESP32 family, the performance cost of dynamic allocation (fragmentation) is higher risk than the raw usage.

---

## 5. Systemic Optimizations & Architectural Proposals

### 5.1 Pattern: Lazy Initialization (The "Pay-for-Play" Model)
**Problem**: Every `Object` pays a 12-byte tax for a `std::vector` that is empty 90% of the time.
**Solution**: Move state to the heap, allocated only on demand.

- **Current**: `std::vector<...> callback_nodes_;` (Stack: 12-24 bytes)
- **Proposed**: `std::unique_ptr<CallbackRegistry> registry_;` (Stack: 4 bytes)
  - `CallbackRegistry` holds the vector.
  - Accessors check for `nullptr` before push/pop.
- **Trade-off**: Adds a branch instruction to every event add/remove operation.
- **Verdict**: **APPROVED**. The memory limitation on MCUs is strictly harder than the nanosecond branch cost.

### 5.2 Pattern: Stateless Views (Zero-Overhead Interop)
**Problem**: Temporary wrappers (e.g., `Button btn(lv_btn_create(...))`) incur construction/destruction costs and ambiguity about "who owns the C object?".
**Concept**: Explicitly separate **Owners** from **Views**.

```cpp
// The Owner (owns the C life-cycle)
class Button : public Widget<Button> { ... }; 

// The View (Borrowed reference)
template <typename T>
class View {
    lv_obj_t* ptr_;
    public:
        View(lv_obj_t* p) : ptr_(p) {}
        // API mirrors Widget<T> but holds NO state and deletes NOTHING.
        void set_text(const char* txt) { lv_label_set_text(ptr_, txt); }
};
```
- **Use Case**: Traversal APIs. `View<Object> parent = btn.get_parent();`
- **Benefit**: Zero allocations. Pure syntactic sugar.

### 5.3 Allocator Strategy: Combatting Fragmentation
**Problem**: `std::function` performs small, variable-sized allocations. This causes heap fragmentation, which is more fatal than raw usage on long-running embedded systems.
**Strategy**: `std::pmr` (Polymorphic Memory Resource) or Slab Allocation.
- Implement a `CallbackAllocator` that draws from a fixed-size block pool (e.g., 64-byte blocks).
- Ensures that freeing a callback always returns a perfectly reusable slot.

### 5.4 Resolution Plan for Event Leak
1.  **Refactor `EventSource`**: It must not allocate directly.
2.  **Protocol**: `EventSource` must request storage from the host `Object`.
    ```cpp
    // In Object
    virtual void register_callback(std::unique_ptr<CallbackNode> node);
    
    // In EventSource
    Derived& add_event(...) {
        auto node = std::make_unique<...>(...);
        static_cast<Derived*>(this)->register_callback(std::move(node));
        // ...
    }
    ```
3.  **Lifecycle**: `Object` destructor clears the registry, automatically deleting all closures.
