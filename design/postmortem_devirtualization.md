# Postmortem: Devirtualization and Memory Optimization Attempt

## 1. Overview and Objectives

In late January 2026, a significant refactor was undertaken with the goal of achieving "Zero-Overhead Wrappers" for the `lvgl_cpp` library. The primary target was the `lvgl::Object` base class, which is the foundation for all widget types.

### 1.1. Key Goals
*   **Pointer-Sized Handle**: Reduce `sizeof(lvgl::Object)` from 40 bytes to 8 bytes (on 64-bit systems).
*   **Memory Efficiency**: Eliminate the 24-byte overhead of `std::vector` (used for callbacks) in objects that do not use C++ event closures.
*   **Performance**: Reduce construction/destruction overhead by removing the vtable pointer (`vptr`) and the need to initialize container members.

## 2. Implementation Strategy

The refactor involved three high-risk architectural changes:

### 2.1. Tagged Pointer Packing
The `lv_obj_t*` pointer and the `bool owned_` flag were merged into a single `uintptr_t`. Since LVGL objects are aligned to at least 4 bytes, the least significant bit (LSB) was used as a "trash bit" to store the ownership flag. 
*   `data_ & ~1` = Pointer address.
*   `data_ & 1` = Ownership bit.

### 2.2. State Externalization
The `callback_nodes_` vector was removed from the `Object` class. Instead, a heap-allocated `ObjectData` struct was stored in the underlying `lv_obj_t->user_data` field. This ensured that only objects using C++-style closures paid the memory penalty for storage.

### 2.3. Devirtualization of the Destructor
The `virtual` keyword was removed from `~Object()`. This eliminated the 8-byte `vptr` from every instance, allowing the class to finally reach the 8-byte size target.

## 3. The Failure Points (Why it was Reverted)

While the implementation achieved the 8-byte target and passed initial unit tests, several critical issues were identified during integration into larger projects.

### 3.1. Undefined Behavior in Derived Deletion
The most significant issue was the removal of the virtual destructor. In C++, deleting a derived object (e.g., `Button*`) through a base class pointer (`Object*`) when the destructor is non-virtual is **Undefined Behavior**.
*   **Risk**: Even if the current `Button` class adds no data members, future widgets (or user-defined custom widgets) might.
*   **Reality**: Several existing widgets (like `Chart` or `Canvas`) were planned to have managed internal buffers. A non-virtual destructor would cause these buffers to leak if the object was treated polymorphically.

### 3.2. Slicing and Binary Compatibility
Moving to a strict 8-byte non-virtual layout made the library extremely sensitive to "slicing." If a user wrote `Object obj = button;`, the move/copy behavior (even with deleted copy constructors) became harder to audit for safety across complex template-heavy code.

### 3.3. User Data Collisions
Externalizing state to `lv_obj_t->user_data` caused immediate collisions with user code. Many developers using `lvgl_cpp` still interact with the C API for specific drivers or legacy logic, and they expect `user_data` to be available for their own purposes. Highjacking this pointer for the wrapper's internal callback management was a "breaking change" for many existing LVGL integration patterns.

### 3.4. Debugging Complexity
The tagged pointer strategy made debugging in GDB/LLDB significantly more difficult. Inspecting the `obj_` member no longer showed a valid pointer address; developers had to manually apply a mask in the debugger to see where the object lived.

## 4. Final Decision and Reversion

The decision to manually revert the changes was made to preserve **Library Safety and Predictability** over micro-optimization.

1.  **Safety First**: A 40-byte wrapper that is safe and supports polymorphism is better than an 8-byte wrapper that causes random crashes or memory leaks in user code.
2.  **API Consistency**: The `user_data` field was returned to the user.
3.  **Modern C++ Standards**: Adherence to the Google C++ Style Guide and standard polymorphic practices was prioritized.

## 5. Future Alternatives

To achieve the original goals without the risks, the following directions are proposed:
*   **ObjectView**: A lightweight, 8-byte non-owning "view" class (similar to `std::string_view`) for passing objects to functions without the overhead of the full RAII wrapper.
*   **Lazy Vector Allocation**: Keep the vector in the class but use a `std::unique_ptr` or a custom small-vector optimization to reduce the base size from 24 bytes to 8 bytes for unused instances.

## 6. Conclusion

The devirtualization experiment provided valuable insights into the limits of "zero-overhead" in the context of a polymorphic UI library. While the memory savings were attractive, the cost in terms of safety and interoperability was too high. The current 40-byte `Object` remains the stable, recommended architecture.
