# Gap Analysis: `lv_obj` Subsystem

This document provides a detailed analysis of the coverage gaps in the `lv_obj` subsystem and categorizes them based on `lvgl_cpp` design principles.

## 1. Executive Summary

As of the latest audit, the `lv_obj` subsystem has **299/447 (66.9%)** verified coverage. This analysis covers the remaining 148 functions to determine which are essential for a Platinum C++ experience and which should remain internal.

## 2. Methodology

We categorize missing APIs into four buckets:
1.  **🚀 Idiomatic Gaps**: User-facing features that provide high value and should be wrapped.
2.  **🔧 Internal / Implementation Detail**: Low-level functions used by LVGL's core; wrapping them would leak implementation details.
3.  **📦 Redundant / Proxy Candidates**: Functions logically covered by `StyleProxy`, `TreeProxy`, or `InteractionProxy`.
4.  **🛑 Legacy / Deprecated**: APIs replaced by modern v9 equivalents.

---

## 3. Categorization Table

| API Pattern | Category | Recommendation |
| :--- | :--- | :--- |
| `lv_obj_fade_in/out` | 🚀 Idiomatic | Add to `Object` as high-level animation helpers. |
| `lv_obj_set_id / find_by_id` | 🚀 Idiomatic | Expose to support the modern LVGL ID system. |
| `lv_obj_add_play_timeline_event` | 🚀 Idiomatic | Essential for `lvgl::AnimationTimeline` integration. |
| `lv_obj_add_subject_X` | 🚀 Idiomatic | Bridge to `lvgl::Subject` and `lvgl::Observer`. |
| `lv_obj_set_ext_click_area` | 🚀 Idiomatic | High value for UX (hit testing). |
| `lv_obj_allocate_spec_attr` | 🔧 Internal | **DO NOT WRAP**. Managed automatically. |
| `lv_obj_mark_layout_as_dirty` | 🔧 Internal | **DO NOT WRAP**. Controlled via layout setters. |
| `lv_obj_null_on_delete` | 🔧 Internal | **DO NOT WRAP**. Handled by C++ smart pointers/RAII. |
| `lv_obj_calculate_X` | 🔧 Internal | **DO NOT WRAP**. These are internal resolution steps. |
| `lv_obj_get_style_X` | 📦 Redundant | Ensure these are mapped in `StyleProxy`. |
| `lv_obj_get_index_by_type` | 📦 Redundant | Already achievable via `TreeProxy` traversal. |

---

## 4. Deep Dive: The Platinum Roadmap for `lv_obj`

To reach Platinum status, the following specific items MUST be implemented or logically verified:

### 4.1. Core Animation Helpers
LVGL provides several "macro-like" animations that are extremely useful for UI development but are currently missing wrappers.
- `lv_obj_fade_in(obj, time, delay)`
- `lv_obj_fade_out(obj, time, delay)`

### 4.2. Metadata & Inspection
Modern LVGL v9 promotes a "Data-Driven" approach using IDs and user-data pointers.
- `lv_obj_set_id` / `lv_obj_get_id`
- `lv_obj_find_by_id` (Recursive search)

### 4.3. Event-Driven Subject Binding
While we have `Subject::bind()`, the direct `lv_obj_add_subject_...` functions allow for "side-effect" events (like toggling an object state when a subject changes) without a middleman. These should be exposed via a fluent API:
```cpp
obj.on_subject(my_subject).toggle();
```

---

## 5. Conclusion: What defines "Platinum"?

For `lv_obj`, Platinum does NOT mean 100% of all functions. It means:
1.  **100% of Public, Non-Internal Functions** are accounted for.
2.  **All Style Properties** are accessible via `obj.style()`.
3.  **All Tree Operations** are accessible via `obj.tree()`.
4.  **All Interaction Tuning** is accessible via `obj.interaction()`.

Functions like `lv_obj_allocate_spec_attr` and `lv_obj_calculate_style_text_align` will be added to the **"Anti-Pattern" (Should Not Wrap) list** in the audit script to accurately reflect the true "Verified" percent.
