# lvgl_cpp Refactoring Roadmap v2.0

## 1. Executive Summary
The primary goal of the v0.2.0 refactoring—standardizing the widget architecture and eliminating "God Object" inheritance—is **100% complete**. Every widget now inherits from the CRTP `Widget<T>` template and utilizes shared mixins for core functionality (Position, Size, Style, Events).

The focus now shifts from **Structural Migration** to **Behavioral idiomaticness** and **System Management**.

## 2. Current Status: Phase Verification

| Phase | Milestone | Status | Notes |
| :--- | :--- | :--- | :--- |
| **Phase 1** | Foundation (Mixins, CRTP) | ✅ 100% | Architecture is stable and modular. |
| **Phase 2** | Core Pilot (Button, Label) | ✅ 100% | Patterns proven and fluent. |
| **Phase 3** | Global Expansion | ✅ 100% | All 60+ widgets migrated to `Widget<T>`. |
| **Phase 4** | Constructor Standardization | ✅ 100% | Standard delegation and `nullptr` handling. |
| **Phase 5** | Legacy Cleanup | ✅ 100% | Removed deprecated methods from `Object`. |

## 3. The Next Horizon: Phase 6 & 7

The remaining work is categorized into three strategic batches.

### Batch A: The "Idiomatic Core" (Unblocking Data Types)
Before deep widget APIs can be finalized, we need strictly typed data structures and scoped enums.
- **[x] Geometry Utilities (#22)**: Implement C++ `Point`, `Area`, and `Coord` classes to replace raw `lv_point_t`.
- **[x] Scoped Enums (#24)**: Migrate `LV_STATE_...` and `LV_PART_...` to `lvgl::State::Pressed` and `lvgl::Part::Main`.

### Batch B: Behavioral Deepening (The Event System)
Moving from wrapping C functions to idiomatic C++ behaviors.
- **[x] Advanced Callbacks (#31)**: Support `std::function` and lambdas for widget-specific events (e.g., `on_clicked`, `on_value_changed`).
- **[x] Comprehensive Callbacks Extension (#35)**: Extend standardized callbacks to `Checkbox`, `Arc`, `Textarea`, `Spinbox`, `Calendar`, and `Table`.
- **[x] Group Management (#25)**: Wrapper for `lv_group_t` to handle focus/editing logic for input devices.

### Batch C: High-Level Components & Systems
Handling the application lifecycle, complex UI flows, and system abstractions.
- **[x] Screen Management (#26)**: A dedicated `Screen` class for transitions, background tasks, and active screen tracking.
- **[ ] Complex Widget Encapsulation (#34)**: Deep refactor of the `Menu` widget to encapsulate "Pages" as first-class C++ objects.
- [x] TabView & TileView Refactor (#37): Encapsulate page/tile management for `TabView` and `TileView`, similar to `Menu`.
- **[x] Modal & Window Management (#38)**: Idiomatic wrappers for `MsgBox` (modals) and `Win` (header/content windows).
- [x] Animation & Timer System (#39): RAII wrappers for `lv_anim` and `lv_timer` to manage safe lifecycles using C++ functors.
- **[x] Filesystem & Images (#40)**: C++ wrappers for `lv_fs` and `lv_img` handling, ensuring resource safety.

## 4. Definition of Done for v0.2.0 Release
The refactoring is considered complete when:
1. All open architectural issues (#22, #24, #25, #26, #31, #34, #61) are closed.
2. The `MIGRATION_GUIDE.md` is updated for the final API surface.
3. Test coverage for non-visual logic reaches >80%.

---
**Documented by**: Technical Execution Architect
**Date**: 2026-01-18
