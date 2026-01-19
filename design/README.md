# LVGL C++ API modernization design wiki

## 1. Executive Summary

This design repository outlines the re-architecture of `lvgl_cpp` into a **Fluent, Type-Safe, and Idiomatic C++20 Framework**. 

**Goal**: To resolve the friction between LVGL's flexible C architecture and modern C++ development patterns, making the "Pit of Success" wide for developers while maintaining zero-overhead performance for embedded constraints.

---

## 2. Design Philosophy & Rationale

To understand *why* `lvgl_cpp` is built this way, we must contrast the underlying C architecture with our C++ design goals.

### 2.1. The "God Object" vs. Composition (CRTP)

**How LVGL (C) does things:**
In C, `lv_obj_t` is a generic structure that holds *all possible properties*. Every object has X/Y coordinates, size, and styles, but specific widgets (like Sliders) "inherit" this base object via composition at the C struct level.
However, in previous C++ wrappers, this often led to a **God Object** pattern where the base `Object` class exposed *every* method (positioning, sizing, styling) to *every* widget. This corrupted the API surface; a generic `Object` suggests it can do anything, but a `Label` shouldn't necessarily expose `set_value()` unless it's a value-holding widget.

**How `lvgl_cpp` does things:**
We strictly reject the God Object. Instead, we use **Mixins** via C++20 **CRTP (Curiously Recurring Template Pattern)**.
*   **Mechanism**: The `Widget<T>` template inherits from specific traits: `Positionable`, `Sizable`, `Stylable`, `Scrollable`.
*   **Result**: You only pay for what you use. We decompose functionality into granular, reusable components.

### 2.2. Type Safety vs. Void Pointers

**How LVGL (C) does things:**
LVGL relies heavily on `void*` user data and integer-based event codes.
*   Callbacks receive a generic event struct.
*   User data must be mechanically cast back to the correct type.
*   Widgets are often created generically (`lv_obj_create`) and then specialized.

**How `lvgl_cpp` does things:**
We enforce **Strong Typing** at compile time.
*   **constructors**: `Button` creates a button. You cannot accidentally wrap a Slider as a Button without an explicit (and dangerous) cast.
*   **Callbacks**: We use `std::function` and lambdas. The framework handles the `void*` trampoline logic internally, presenting the user with a clean, typed interface.
*   **Builders**: Styles are built using method chaining (`Style().bg_color(...)`) rather than untyped property sets (`lv_style_set_prop(..., generic_value)`).

### 2.3. The "Escape Hatch" (Zero Friciton Interop)

**Why design this way?**
No wrapper can perfectly capture 100% of a shifting C API like LVGL. If the wrapper tries to hide the C layer completely, it blocks developers from using unsupported features.

**The Solution:**
*   **Transparency**: Every C++ object exposes `.raw()` returning the underlying `lv_obj_t*`.
*   **Interoperability**: Constructors accept `lv_obj_t*`, allowing you to wrap an object created by C code (e.g., from UI Gen tools) into a C++ object instantly.
*   **Unmanaged Mode**: Objects can be created in "Unmanaged" mode, where the C++ wrapper acts as a temporary view (borrowed reference) without trying to `free` the specific memory on destruction.

### 2.4. Embedded First Performance

**Rationale**:
Embedded systems have tight RAM/ROM constraints. A heavy C++ abstraction with `virtual` tables for every widget would be unacceptable.

**Implementation**:
*   **No Virtual Methods**: In the hot path of widget rendering and API calls, we use templates (static polymorphism) instead of virtual inheritance.
*   **Smart Memory**: We use `std::unique_ptr` for extra attached data (like C++ callbacks) but rely on LVGL's highly optimized memory manager (`lv_mem_alloc`) for the widgets themselves.
*   **Inlining**: Most `set_...` wrappers are `inline`, meaning they compile down to the exact same assembly as the raw C function call.

---

## 3. Architecture and modules

### 3.1. Core system
*   **[Core Architecture](core/README.md)**: `Object` base class, `Widget<T>` template, Memory Management, and the Event System.
*   **[Style System](core/STYLES.md)**: The new `Style` builder and Property proxies.

### 3.2. Widget ecosystem
*   **[Input Widgets](widgets/INPUTS.md)**: `Button`, `Checkbox`, `Slider`, `Switch`, `Textarea`, `Keyboard`, `Encoder`.
*   **[Output/Visual Widgets](widgets/OUTPUTS.md)**: `Label`, `LED`, `Bar`, `Arc`, `Line`, `Image`, `Spinner`.
*   **[Container Widgets](widgets/CONTAINERS.md)**: `Window`, `TabView`, `TileView`, `Table`, `List`.
*   **[Menus and Complex](widgets/COMPLEX.md)**: `Menu`, `Calendar`, `Chart`, `Roller`, `Dropdown`.

### 3.3. System modules
*   **[Layouts](system/LAYOUTS.md)**: Declaration helpers for Flex and Grid.
*   **[Display and Input](system/DRIVERS.md)**: `Display` and `InDev` wrappers.
*   **[Memory & Performance](memory_analysis.md)**: Analysis of overhead and callbacks.
*   **[Animations & Timelines](animations.md)**: Wrapper design for `lv_anim` and timelines.
*   **[Fonts](fonts.md)**: Font resource management (`OwnedFont`).
*   **[Input Devices](input_devices.md)**: `Pointer`, `Keypad`, `Encoder` abstractions.
*   **[Filesystem](filesystems.md)**: `lv_fs` wrappers (`File`, `Directory`).

---

## 4. Refactoring Roadmap & Status

### 4.1. Completed Phases (Verification)

| Phase | Milestone | Status | Notes |
| :--- | :--- | :--- | :--- |
| **Phase 1** | Foundation (Mixins, CRTP) | ✅ 100% | Architecture is stable and modular. |
| **Phase 2** | Core Pilot (Button, Label) | ✅ 100% | Patterns proven and fluent. |
| **Phase 3** | Global Expansion | ✅ 100% | All 60+ widgets migrated to `Widget<T>`. |
| **Phase 4** | Constructor Standardization | ✅ 100% | Standard delegation and `nullptr` handling. |
| **Phase 5** | Legacy Cleanup | ✅ 100% | Removed deprecated methods from `Object`. |

### 4.2. Current Horizon: Phase 6 & 7

The remaining work is categorized into three strategic batches.

#### Batch A: The "Idiomatic Core" (Unblocking Data Types)
Before deep widget APIs can be finalized, we need strictly typed data structures and scoped enums.
- **[x] Geometry Utilities (#22)**: Implement C++ `Point`, `Area`, and `Coord` classes to replace raw `lv_point_t`.
- **[x] Scoped Enums (#24)**: Migrate `LV_STATE_...` and `LV_PART_...` to `lvgl::State::Pressed` and `lvgl::Part::Main`.

#### Batch B: Behavioral Deepening (The Event System)
Moving from wrapping C functions to idiomatic C++ behaviors.
- **[x] Advanced Callbacks (#31)**: Support `std::function` and lambdas for widget-specific events (e.g., `on_clicked`, `on_value_changed`).
- **[x] Comprehensive Callbacks Extension (#35)**: Extend standardized callbacks to `Checkbox`, `Arc`, `Textarea`, `Spinbox`, `Calendar`, and `Table`.
- **[x] Group Management (#25)**: Wrapper for `lv_group_t` to handle focus/editing logic for input devices.

#### Batch C: High-Level Components & Systems
Handling the application lifecycle, complex UI flows, and system abstractions.
- **[x] Screen Management (#26)**: A dedicated `Screen` class for transitions, background tasks, and active screen tracking.
- **[x] Complex Widget Encapsulation (#34)**: Deep refactor of the `Menu` widget to encapsulate "Pages" as first-class C++ objects.
- **[x] TabView & TileView Refactor (#37)**: Encapsulate page/tile management for `TabView` and `TileView`, similar to `Menu`.
- **[x] Modal & Window Management (#38)**: Idiomatic wrappers for `MsgBox` (modals) and `Win` (header/content windows).
- **[ ] Animation & Timer System (#39)**: RAII wrappers for `lv_anim` and `lv_timer` to manage safe lifecycles using C++ functors.
- **[x] Filesystem & Images (#40)**: C++ wrappers for `lv_fs` and `lv_img` handling, ensuring resource safety.

## 5. Definition of Done for v0.2.0 Release
The refactoring is considered complete when:
1. All open architectural issues (#22, #24, #25, #26, #31, #34, #61) are closed.
2. The `MIGRATION_GUIDE.md` is updated for the final API surface.
3. Test coverage for non-visual logic reaches >80%.
