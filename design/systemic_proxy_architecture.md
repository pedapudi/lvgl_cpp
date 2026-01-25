# Systemic Proxy Architecture: Eliminating Redundancy in lvgl_cpp

## 1. Executive Summary

This document proposes a foundational architectural shift for `lvgl_cpp` to eliminate the pervasive redundancy between the `Object` class, various Mixins (`Stylable`, `Layoutable`, etc.), and the underlying LVGL C API.

Currently, `lvgl_cpp` "pollutes" the `Object` and `Widget` classes with hundreds of flat, verb-based setters (e.g., `set_style_bg_color`, `set_flex_flow`), often duplicated across mixins. This leads to:
1.  **API Bloat**: `Object` has too many methods.
2.  **Maintenance Burden**: Every new LVGL feature needs duplicate wrappers.
3.  **Inconsistency**: Users are confused between `style` objects and direct setters.

**The Solution**: Adopt a **Systemic Proxy Pattern**.
We will remove these flat methods and replace them with **Transient Proxies** accessed via factory methods on the main object.

## 2. The Proxy Pattern

Instead of:
```cpp
obj.set_flex_flow(FlexFlow::Row);
obj.set_flex_align(FlexAlign::Center, ...);
obj.set_scrollbar_mode(ScrollbarMode::Off);
obj.set_style_bg_color(Red, Part::Main);
```

We standardise on:
```cpp
// 1. Layout Proxy
obj.layout().flex(FlexFlow::Row)
            .align(FlexAlign::Center, ...);

// 2. Scroll Proxy
obj.scroll().mode(ScrollbarMode::Off)
            .to(0, 100);

// 3. Style Proxy (Already prototyped)
obj.style(Part::Main).bg_color(Red)
                     .radius(10);
```

### 2.1. Advantages
- **Zero Overhead**: Proxies are lightweight stack objects, elided by the compiler.
- **Discoverability**: IDE autocompletion becomes contextual (`obj.scroll().` shows only scroll methods).
- **Maintainability**: `Object` class shrinks drastically. Mixins are removed or become simple factories.

## 3. Scope of Change

### 3.1. Removing Mixins
The following mixins will be deprecated and eventually removed, replaced by Proxies:
*   `core/mixins/layoutable.h` -> `LayoutProxy`
*   `core/mixins/positionable.h` -> `Object` core (keep x, y, align fundamental) + `LayoutProxy`? 
    *   *Decision*: `x`, `y`, `size` are fundamental enough to stay on `Object`, but specialized layout logic moves to proxy.
*   `core/mixins/scrollable.h` -> `ScrollProxy`
*   `core/mixins/stylable.h` -> `StyleProxy` (Factory remains in `Object` or lightweight `Stylable` mixin).

### 3.2. Cleaning `Object`
All `set_style_*`, `set_flex_*`, `set_grid_*`, `set_scroll_*` methods will be removed from `Object`'s public interface (or deprecated).

## 4. Implementation Plan

### Phase 1: Style Proxy (Completed)
- **Status**: Implemented and verified.
- **Action**: All style properties accessible via `style()`.

### Phase 2: Layout Proxy
- **New Class**: `core/layout_proxy.h`
- **Methods**: `flex()`, `grid()`, `flex_grow()`, etc.
- **Integration**: `Object::layout()` returns `LayoutProxy`.

### Phase 3: Scroll Proxy
- **New Class**: `core/scroll_proxy.h`
- **Methods**: `to()`, `by()`, `mode()`, `snap()`.
- **Integration**: `Object::scroll()` returns `ScrollProxy`.

### Phase 4: Deprecation & Cleanup (Completed)
- Mark old methods `[[deprecated]]`. -> Skipped, removed directly.
- Update all examples and tests to use Proxies.
- Legacy mixins (`Stylable`) removed from `core/mixins` and `Widget` inheritance.
- Flat setters removed from `Object`.

## 5. Verification
Each Proxy will have a dedicated test suite verifying:
1.  **Fluent chaining** (compilation & API).
2.  **Effect** (underlying C function calls - verified via getters).
3.  **Zero overhead** (inspection of assembly not required, but design ensures it).

---
*Drafted by Antigravity, Lead Systems Architect*
