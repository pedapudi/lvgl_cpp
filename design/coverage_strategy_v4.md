# Technical design: Systemic API coverage expansion (Phase 4)

**Tracking Issue:** #150
**Status:** DRAFT

## 1. Goal
Increase `lv_obj` function coverage from 30.4% to >50% and `lv_event` coverage from 17.9% to >60%, while maintaining a premium, C++ idiomatic API.

## 2. Approach: Behavioral proxy architecture
The chosen approach for Phase 4 is a **Behavioral proxy architecture**. Instead of expanding the `Object` base class horizontally (which leads to the "Monolithic Object" anti-pattern), functionality is grouped into domain-specific proxies.

### 2.1. Why this is the right approach
1.  **Logical encapsulation**: LVGL's `lv_obj` combines layout, events, styles, and state. In C++, these are distinct concerns. Proxies allow us to separate these concerns at the API level (e.g., `obj.layout()` vs `obj.event()`).
2.  **Namespace hygiene**: By using proxies, we keep the primary `Object` class lean. Developers can explore specific capabilities via IntelliSense without being overwhelmed by 500+ methods.
3.  **Memory management**: Proxies can encapsulate complex handle management. For example, the `EventProxy` can handle the `user_data` lifecycle for C++ lambdas, a common source of leaks in manual C/C++ integrations.

### 2.2. Proposed proxy modules
1.  **`EventProxy` (#147)**: 
    - **How**: Wraps `lv_obj_add_event_cb` with a `std::vector` of C++ lambda wrappers.
    - **Methods**: `obj.event().send(code, data)`, `obj.event().add_cb(lambda)`, `obj.event().remove_all()`.
2.  **`StateProxy` (#148)**:
    - **How**: Direct mapping of `lv_obj_add_state`, `lv_obj_has_state`, using bitwise safety and `enum class`.
    - **Methods**: `obj.state().add(State::Checked)`, `obj.state().is(State::Pressed)`, `obj.state().clear(State::Disabled)`.
3.  **`GroupProxy` (#149)**:
    - **How**: Encapsulates the relationship between widgets and their navigation groups.
    - **Methods**: `obj.group().add()`, `obj.group().focus()`, `obj.group().is_focused()`.

### 2.3. Widget completeness
1.  **`Span` implementation (#150)**: Implement the `Span` and `SpanGroup` wrappers. These are currently at 0% coverage but essential for high quality typography.
## 3. Rationale and analysis

### 3.1. Why this is the right approach
- **Maintainability**: LVGL v9 has 1,460+ functions. A 1:1 mapping in the base `Object` class results in a "Monolithic Object" that is hard to navigate (IntelliSense overload).
- **Discoverability**: Proxies act as namespaces. Typing `obj.layout().` or `obj.event().` immediately narrows down the options for the developer.
- **Memory safety**: By using `std::function` inside proxies, we can manage the lifetime of C++ captures reliably.

### 3.2. Alternatives considered
| Alternative | Analysis | Why Rejected |
| :--- | :--- | :--- |
| **Monolithic inheritance** | 1:1 wrapping of functions into the base class. | Unmanageable API surface; high potential for naming collisions. |
| **Compile-time mixins** | Heavy use of templates/CRTP to "inject" behaviors. | **Failure to scope**: Objects inherit all mixins into a flat namespace. `Button` still has 100+ methods at the top level, defeating the purpose of behavioral modularity. |
| **Generic `set_property`** | implementing a single `set_property(Property, Value)` method. | Complete loss of type safety; relies on manual casts; poor developer experience (DX). |

### 3.3. Consistency with design trajectory
This approach is the logical evolution of the **Proxy Pattern** established in v0.2.0 (e.g., `obj.style()` and `obj.layout()`). It reinforces the "Composition over Inheritance" principle.

## 4. Verification plan

### 4.1. Functional verification
1.  **Unit tests**: Exercised in `tests/test_obj_proxies.cpp`.
2.  **Audit script**: Re-run `audit_api_coverage.py` to verify coverage increase.

### 4.2. Memory overhead verification
1.  **Size audit**: Verify `sizeof(Proxy)` is minimal (ideally `sizeof(void*)`).
2.  **Binary size delta**: Compare a widget with flat wrappers vs proxy wrappers. Use `nm --size-sort` to ensure proxy-related symbols are either elided or provide a net reduction by consolidating logic.
3.  **Heap stability**: Run `bench_churn_stability` to ensure `EventProxy` properly cleans up lambda captures.
