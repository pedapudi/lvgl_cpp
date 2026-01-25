# Reconciling Style Architecture

## 1. Executive Summary

This document addresses the architectural conflict between `core/mixins/stylable.h` (Local Styles) and `misc/style.h` (Shared Styles). Currently, these two components implement redundant, divergent APIs for the same underlying LVGL styling system. This redundancy leads to maintenance burden, API bloat, and inconsistent user experience.

**The Solution:** Implement a **Unified Style Interface** pattern.
1.  **Standardize** on the fluent, noun-based naming convention (e.g., `bg_color()`) for all styling.
2.  **Introduce** a zero-overhead **Transient Style Proxy** for local styles (`obj.style(part).bg_color(...)`).
3.  **Deprecate** the flat, verb-based setters (`obj.set_style_bg_color(...)`) to reduce `Object` API bloat.

## 2. The Conflict

LVGL supports two styling methods:
1.  **Shared Styles (`lv_style_t`)**: Reusable style objects added to widgets. Key for memory efficiency and theming.
2.  **Local Styles (`lv_obj_set_style_*`)**: Direct customization of a specific widget. Key for one-off adjustments.

In `lvgl_cpp`, these are currently modeled as:
1.  **`misc/style.h` (`Style` class)**: Wraps `lv_style_t`. Uses fluent, noun-based API (e.g., `.bg_color(Red)`).
2.  **`core/mixins/stylable.h` (`Stylable` mixin)**: Wraps `lv_obj_set_style_*`. Uses flat, verb-based API (e.g., `.set_bg_color(Red)`).

### Issues
*   **Redundancy**: Every style property must be implemented in two places.
*   **Inconsistency**: Users must remember `style.bg_color(...)` vs `obj.set_bg_color(...)`.
*   **API Bloat**: The `Stylable` mixin adds 100+ methods directly to the `Object` interface, polluting the namespace and making autocomplete noisy.

## 3. Proposed Solution: The Unified Style Interface

We will unify the **Interface** while keeping the distinct **Implementations** (as they target different underlying C types).

### 3.1. The Style Proxy Pattern

Instead of adding style methods directly to `Object`, we expose a proxy object that represents the widget's local style interface.

```cpp
// Usage Example

// 1. Shared Style (Existing, just aligned)
Style s;
s.bg_color(Palette::Red)
 .radius(10);

// 2. Local Style (New Proxy Pattern)
// BEFORE: obj.set_bg_color(Palette::Red);
// AFTER:
button.style(Part::Main)
      .bg_color(Palette::Red)
      .radius(10);
```

### 3.2. Architecture

#### `StyleProxy` (New)
A simplified, transient class returned by `Stylable::style()`.

```cpp
template <typename ObjType>
class StyleProxy {
 public:
  StyleProxy(ObjType* obj, lv_style_selector_t selector)
      : obj_(obj), selector_(selector) {}

  // Fluent API strictly matching Style class
  StyleProxy& bg_color(Color color) {
    lv_obj_set_style_bg_color(obj_->raw(), color.raw(), selector_);
    return *this;
  }
  
  StyleProxy& radius(int32_t value) {
    lv_obj_set_style_radius(obj_->raw(), value, selector_);
    return *this;
  }

 private:
  ObjType* obj_;
  lv_style_selector_t selector_;
};
```

#### `Stylable` (Refactored)
The mixin becomes a factory for the proxy, rather than a container for all methods.

```cpp
template <typename Derived>
class Stylable {
 public:
  // Factory method
  auto style(Part part = Part::Main) {
    return StyleProxy<Derived>(static_cast<Derived*>(this), static_cast<lv_style_selector_t>(part));
  }
  
  auto style(lv_style_selector_t selector) {
    return StyleProxy<Derived>(static_cast<Derived*>(this), selector);
  }

  // Keep add_style/remove_style
  Derived& add_style(const Style& style, Part part = Part::Main);
  // ...

  // DEPRECATED: Flat setters
  // Derived& set_bg_color(...) [[deprecated]];
};
```

### 3.3. Zero Overhead Verification

The `StyleProxy` is a "Transient Proxy." It is intended to be used temporarily and destroyed immediately (e.g., in a single statement).

*   **Memory**: It has size `sizeof(void*) + sizeof(uint32_t)` (approx 12-16 bytes). It is allocated on the stack.
*   **Performance**: Since all methods are inline, modern compilers (O2/O3) will elide the proxy object entirely and generate direct calls to `lv_obj_set_style_*`.
*   **Result**: Zero runtime overhead compared to the flat API, but vastly improved developer experience.

## 4. Implementation Plan

### Phase 1: Prototype Proxy
1.  Define `core/style_proxy.h`.
2.  Implement a few properties (Background, Border) to verify ergonomics and compilation.
3.  Add `style()` method to `Stylable`.

### Phase 2: Align APIs
1.  Systematically implement all properties in `StyleProxy` to match `misc/style.h`.
2.  Ensure naming alignment (rename `Stylable` flat setters if we decide to keep them temporarily, or just map Proxy to current Style class names).

### Phase 3: Cleanup
1.  Mark flat setters in `Stylable` as `[[deprecated]]`.
2.  Update examples and tests to use the new syntax.

## 5. Decision Log

| Decision | Choice | Rationale |
| :--- | :--- | :--- |
| **Naming** | Noun-based (`bg_color`) | Matches Builder pattern; distinguishes styling from structural properties (`set_width`). |
| **Mechanism** | Transient Proxy | Removes bloat from Object; ensures zero instance size increase. |
| **Mixin** | Retain (Shrunk) | `Stylable` still needed to provide `add_style` and the `style()` factory. |
