# Architectural Discussion: Mixins vs. Proxies in `lvgl_cpp`

## 1. Context
The `lvgl_cpp` library currently utilizes a hybrid approach for its API design: **Facet Proxies** for core concerns (styles, events, layouts) and **CRTP-based Mixins** for widget-specific functionality and fluent chaining. This document explores the congruency of these patterns and evaluates whether the `Widget` class should migrate to a pure proxy-based approach.

---

## 2. Analysis of Current Patterns

### 2.1. CRTP Mixins (e.g., `Widget<Derived>`)
Used to solve the "Returning `*this` Problem" in inheritance. Without CRTP, `Button::set_size()` would return `Object&`, breaking the chain if the developer intended to call a `Button`-specific method immediately after.
*   **Behavior**: Injects methods directly into the derived class namespace.
*   **Syntax**: `btn.set_size(x).on_click(y);`

### 2.2. Facet Proxies (e.g., `StyleProxy`, `EventProxy`)
Used to namespace massive API surfaces that would otherwise clutter the base `Object` class (like the 50+ individual style properties in LVGL).
*   **Behavior**: Provides a temporary portal to a subdomain of functionality.
*   **Syntax**: `obj.style().bg_color(Color::Red()).radius(10);`

---

## 3. Comparison of Approaches

| Approach | Pros | Cons |
| :--- | :--- | :--- |
| **Current Hybrid** (Mixins for Fluent + Proxies for Domains) | **Best of both worlds**. Widgets feel like native classes; complex domains don't pollute the autocomplete of `Object`. Chaining is preserved. | Two different syntactical patterns for the user to learn (`.set_x()` vs `.style().x()`). |
| **Pure Proxy** (e.g., `label.text().set("foo")`) | **Extreme Namespace Hygiene**. The widget class only has domain-specific portals. Very consistent with the `Object` facet design. | **Verbosity**. Simple operations like setting label text or a slider value require an extra function call. Breaks familiar C++ class expectations. |
| **Pure Mixin** (e.g., `label.set_bg_color()`) | **Discoverability**. Every possible action is available via autocomplete on the main object. No "portal" jumping. | **Massive Bloat**. The base `Object` class would have 200+ methods. Compilation times increase due to header size and template instantiations. |
| **The "Flattened" Proxy** (Mixins that return proxies) | Allows `btn.style_bg_color()` to exist while still grouping things. | Doesn't solve the bloat problem; arguably makes naming conventions messier. |

---

## 4. Recommendation

### 4.1. Maintain the `Widget` Mixin Pattern
The `Widget<Derived>` template should continue to use Mixins rather than Proxies for its primary setters. Changing to a proxy (e.g., `label.text().set("foo")`) would necessitate the proxy returning a reference to `Label` to maintain fluent chaining—effectively re-implementing the CRTP logic inside every proxy and increasing boilerplate.

### 4.2. The "Substance vs. Attribute" Rule
To ensure architectural consistency, the following rule is proposed for future additions:
1.  **Use Mixins** for fundamental properties that define **"What the widget is"** (Position, Size, Value, Text). These are intrinsic to the widget's state machine.
2.  **Use Proxies** for cross-cutting concerns that define **"How the widget behaves or looks"** (Styles, Events, Hierarchy, Layout definition). These are external configurations applied to the object.

## 5. Future Evolution: Terminal Proxies
To further unify the patterns, proxies could be enhanced to support "Terminal" methods that return the `Derived` type of the calling widget, allowing a seamless transition back into core widget setters without breaking the fluent chain.

```cpp
btn.style()
   .bg_color(Color::Red())
   .radius(5)
   .apply() // Terminal method returning Button&
   .set_width(100);
```

---

## 6. Conclusion
The hybrid Mixin/Proxy model is the most ergonomic balance for an embedded C++ library. It preserves the performance and familiarity of direct class methods for high-frequency operations while leveraging modern C++ proxy patterns to manage the sheer scale of the LVGL API.
