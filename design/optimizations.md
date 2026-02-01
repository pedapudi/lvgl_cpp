# Optimizations

This document details the memory and performance optimizations implemented in `lvgl_cpp` to achieve zero-overhead wrappers.

## 1. Memory Size Optimization

**Goal**: Reduce `sizeof(lvgl::Object)` to matching the size of a raw C pointer (`sizeof(void*)` = 8 bytes on 64-bit systems).

**Result**: Achieved. `sizeof(lvgl::Object)` is now **8 bytes**.

### 1.1. Tagged Pointers (Pointer Packing)

The `lvgl::Object` class previously maintained two members:
1.  `lv_obj_t* obj_`: The pointer to the LVGL C object (8 bytes).
2.  `bool owned_`: A flag indicating if the C++ wrapper owns the lifecycle of the C object (1 byte + 7 bytes padding).

This resulted in a minimum size of 16 bytes due to alignment padding.

**Optimization**:
We leverage the property that `lv_obj_t` allocations are always aligned to at least 4 bytes (and typically 8 bytes) on supported architectures. This guarantees that the Least Significant Bit (LSB) of the pointer is always `0`.

We use this LSB to store the `owned` state:
- **Bit 0 (LSB)**: `owned` flag (1 = owned, 0 = unmanaged).
- **Bits 63..1**: The actual pointer address.

```cpp
// Accessors in Object
bool owned() const { return data_ & 1; }
lv_obj_t* raw() const { return reinterpret_cast<lv_obj_t*>(data_ & ~1); }
```

### 1.2. Externalized State (Heap-Allocated Callbacks)

Previously, `lvgl::Object` contained a `std::vector<std::unique_ptr<CallbackNode>>` member to manage event callback closures.
- **Cost**: `sizeof(std::vector)` is 24 bytes.
- **Issue**: This overhead was paid by *every* object wrapper, even if no C++ callbacks were used.

**Optimization**:
We moved this state entirely out of the C++ wrapper and into the underlying LVGL object's `user_data`.
1.  We define a helper struct `ObjectData` that holds the vector.
2.  This struct is lazily allocated on the heap only when `add_event_cb` is called.
3.  The pointer to `ObjectData` is stored in `lv_obj_t->user_data`.
4.  We register a deletion hook (`LV_EVENT_DELETE`) to automatically `delete` the `ObjectData` when the LVGL object is destroyed.

**Impact**: 
- `lvgl::Object` size reduced by 24 bytes.
- Zero memory overhead for simple wrappers that don't use C++ event closures.
- The `ObjectData` lifecycle is tied to the *LVGL object*, regardless of C++ wrapper lifetime.

### 1.3. Devirtualization

Previously, `lvgl::Object` had a `virtual` destructor to allow polymorphic deletion.
- **Cost**: A `vptr` (Virtual Table Pointer) is added to every object (8 bytes).

**Optimization**:
We removed the `virtual` keyword from the destructor.
- **Trade-off**: You **cannot** delete a derived class instance (e.g., `Button*`) through a base class pointer (`Object*`) if the derived class has its own data members or destructor logic that needs to run.
- **Justification**: 
    1.  LVGL widgets are typically used as value types or destroyed via the C API (`lv_obj_del`).
    2.  `lvgl_cpp` wrappers are designed to be lightweight handles, not heavy polymorphic hierarchies.
    3.  Most derived widgets (`Button`, `Label`) add no data members, so slicing is not an issue for data, though destructor behavior is locally important.
    4.  The documentation explicitly warns against polymorphic deletion via `Object*`.

## 2. Benchmark Results

Verified via `tests/test_size.cpp`:

```text
sizeof(void*): 8
sizeof(lvgl::Object): 8
sizeof(lvgl::Button): 8
sizeof(lvgl::Label): 8
Is Object pointer-sized? YES
```

## 3. Performance Impact

- **Construction**: Slightly faster (no vptr initialization, no vector initialization).
- **Access**: Negligible overhead for bit-masking the pointer (single instruction).
- **Events**: Slightly slower first-time event registration (heap allocation of `ObjectData`), but standard speed afterwards.
- **Memory**: Significant reduction in RAM usage for large UI trees managed by C++. A tree of 100 widgets saves ~3.2KB of RAM on stack/heap overhead.
