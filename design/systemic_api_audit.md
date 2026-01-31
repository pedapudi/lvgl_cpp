# Technical API Audit & Systemic Gap Analysis (v2.5)

## 1. Context & Constraints
The `lvgl_cpp` project aims for "Zero-Overhead Idiomatic Abstraction." We have reached a critical milestone where core widgets are Platinum/Gold (100-90% coverage), but the base `Object` and specialized subsystems (Draw, Color, Font) remain at <50%. 

The challenge is to achieve "Functional Completeness" without falling into the trap of wrapping 100% of the C API, which contains significant redundancy and low-level internal detail.

## 2. The Systemic Approach: Facet-Based Organization
We are transitioning from a **Monolithic Object** inheritance model to a **Facet Proxy** model. This addresses the root cause of "Forwarding Hell" and API pollution.

### 2.1 Interface Boundaries
- **Core Property Layer**: Direct access on `Object` (x, y, width, height, parent, etc.).
- **Behavioral Facets**: Scoped proxies for domain-specific logic.
    - `obj.style()` -> `StyleProxy` (Active)
    - `obj.scroll()` -> `ScrollProxy` (Active)
    - `obj.layout()` -> `LayoutProxy` (Planned/Partial)
    - `obj.interaction()` -> `InteractionProxy` (New)
    - `obj.tree()` -> `TreeProxy` (New)

## 3. API Audit: What NOT to Wrap
To maintain a clean C++ ecosystem, the following C APIs are explicitly excluded from direct wrapping. These account for ~25% of the raw C API surface.

| API Category | Details & Rationale | Recommended Alternative |
| :--- | :--- | :--- |
| **Generated Style Setters** | LVGL generates 100+ functions like `lv_obj_set_style_bg_color` and `lv_chart_set_style_...`. These are redundant with the Fluent Style API. | Use `obj.style().bg_color()`. |
| **Redundant Event Handlers** | `lv_obj_add_event_cb`, `lv_obj_remove_event_cb`, and widget-specific event wrappers. | Use `EventProxy` or `obj.on_click()`. |
| **Low-level Math/Geometry** | Most `lv_area_...`, `lv_point_...`, and `lv_color_...` logic. | Use `lvgl::Area`, `lvgl::Point`, and `lvgl::Color` operators. |
| **Internal Macros (`_lv_...`)** | Private implementations subject to change. | Rely on public stable C APIs. |
| **Software Draw Primitives** | Low-level software rendering primitives (e.g., `lv_draw_sw_...`). | Wrap `lv_draw_vector` for modern targets. |
| **Hierarchy Mutators** | `lv_obj_swap`, `lv_obj_move_to_index`. | Handled by the upcoming `TreeProxy`. |
| **Interaction Metadata** | `lv_obj_set_ext_click_area`, `lv_obj_hit_test`. | Handled by the upcoming `InteractionProxy`. |

## 4. Systemic Gaps & Remediation Plan

### 4.1 Coordinate & Geometry Rigor
**Gap**: Many `lv_obj` getters still return raw `int32_t` or `lv_area_t` pointers.
**Incremental Fix**: Standardize on `lvgl::Area` and `lvgl::Point` for `get_coords`, `get_content_coords`, and `hit_test`.
**Timeline**: Phase 9 (Core).

### 4.2 Hierarchy Management (The "Tree" Facet)
**Gap**: Operations like `lv_obj_swap` and `lv_obj_move_to_index` are missing or scattered.
**Incremental Fix**: Implement `TreeProxy` to handle siblings, child indexing, and type-based searches.
**Timeline**: Immediate (next sprint).

### 4.3 Interaction & Input Feedback
**Gap**: Extended click areas and hit-test overrides are not idiomatic.
**Incremental Fix**: Implement `InteractionProxy` to group `set_ext_click_area`, `hit_test`, and `is_editable`.
**Timeline**: Phase 10.

### 4.4 Resource Lifecycle (Font/Image)
**Gap**: Loading fonts (BPP/TTF) and images from diverse sources is inconsistent.
**Incremental Fix**: Unified `ResourceDescriptor` pattern for `OwnedFont` and `OwnedImage`.
**Timeline**: High-Coverage Completion Phase.

## 5. Verification Strategy
1. **Automated Coverage Sweep**: Run `scripts/audit_api_coverage.py` to track progress and verify idiomatic coverage goals.
2. **Benchmark Verification**: Ensure new proxies (Interaction, Tree) maintain zero-overhead using `bench_suite`.
3. **Consistency Check**: Use the `check_lvgl_cpp_consistency` skill to verify naming conventions across new proxies.

## 6. References
- [Missing Object Methods](file:///home/sunil/git/lvgl/lvgl_cpp/design/missing_obj_methods.md)
- [Redundancy Audit](file:///home/sunil/git/lvgl/lvgl_cpp/design/redundancy_audit.md)
- [Idiomatic Gap Analysis](file:///home/sunil/git/lvgl/lvgl_cpp/design/idiomatic_gap_analysis.md)
