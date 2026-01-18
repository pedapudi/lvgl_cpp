# LVGL C++ API Modernization Design Wiki

## 1. Executive Summary

This design repository outlines the re-architecture of `lvgl_cpp` into a **Fluent, Type-Safe, and Idiomatic C++20 Framework**. 

**Goal**: To resolve the friction between LVGL's flexible C architecture and modern C++ development patterns, making the "Pit of Success" wide for developers while maintaining zero-overhead performance for embedded constraints.

## 2. Design Principles

### 2.1. Object-Oriented Composition
We replace the "Monolithic Inheritance" model (where the base class carries all state) with a **Mixin-based** approach using C++20 CRTP.
*   **Principle**: Only pay for what you use. A `Label` shouldn't carry `Slider` API methods.
*   **Mechanism**: `Widget<T>` aggregates traits like `Positionable`, `Sizable`, `Stylable`.

### 2.2. The "Escape Hatch" (Zero Friction Interop)
We acknowledge that no wrapper can perfectly capture 100% of a shifting C API.
*   **Principle**: The C++ API must **never** block access to the underlying C API.
*   **Mechanism**:
    *   All wrapper objects provide a `.raw()` method returning `lv_obj_t*`.
    *   Constructors must accept `lv_obj_t*` to wrap existing C objects.
    *   Memory management defaults to "smart" but allows "unmanaged" modes for strictly borrowing C pointers.

### 2.3. Type-Safe Builders
We replace untyped property sets (`lv_style_set_prop`) with strongly typed builders.
*   **Mechanism**: `Style().bg_color(Palette::Blue)` instead of passing enums and unions.

### 2.4. Embedded First
*   **Principle**: Abstractions must compile away.
*   **Mechanism**: Use `constexpr`, `inline`, and templates. Avoid virtual methods in hot paths. Use `std::unique_ptr` for extra data, but rely on LVGL's heap for widget memory.

---

## 3. Architecture & Modules

The design is split into specific modules. Please refer to the detailed design docs below:

### 3.1. Core System
*   **[Core Architecture](core/README.md)**: `Object` base class, `Widget<T>` template, Memory Management, and the Event System.
*   **[Style System](core/STYLES.md)**: The new `Style` builder and Property proxies.

### 3.2. Widget Ecosystem
*   **[Input Widgets](widgets/INPUTS.md)**: `Button`, `Checkbox`, `Slider`, `Switch`, `Textarea`, `Keyboard`, `Encoder`.
*   **[Output/Visual Widgets](widgets/OUTPUTS.md)**: `Label`, `LED`, `Bar`, `Arc`, `Line`, `Image`, `Spinner`.
*   **[Container Widgets](widgets/CONTAINERS.md)**: `Window`, `TabView`, `TileView`, `Table`, `List`.
*   **[Menus and Complex](widgets/COMPLEX.md)**: `Menu`, `Calendar`, `Chart`, `Roller`, `Dropdown`.

### 3.3. System Modules
*   **[Layouts](system/LAYOUTS.md)**: Declaration helpers for Flex and Grid.
*   **[Display and Input](system/DRIVERS.md)**: `Display` and `InDev` wrappers.

---

## 4. Migration Strategy

1.  **Phase 1 (Foundation)**: Implement `core/mixins` and `Widget<T>`.
2.  **Phase 2 (Core Widgets)**: Migrating `Button` and `Label` to prove the pattern.
3.  **Phase 3 (Expansion)**: Rolling out updates to all 60+ widgets group by group.
4.  **Phase 4 (Cleanup)**: Deprecating legacy `Object` methods.

