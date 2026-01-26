# API coverage and expansion plan (v0.4.0+)

This document outlines the roadmap for achieving near-100% API coverage of LVGL v9 features within the `lvgl_cpp` framework.

## 1. Executive summary

Current `lvgl_cpp` coverage stands at **53.2%** for functions and **65.0%** for enums. While the architectural foundation (RAII, Proxies) is stable, deepening support for core object behaviors (events, state, groups) is the priority for v0.4.0.

## 2. Coverage audit (v9.4 baseline - updated)

- **Total Functions**: 1,460 | **Used**: 776 | **Coverage**: 53.2%
- **Total Enums**: 423 | **Used**: 275 | **Coverage**: 65.0%

### 2.1. Widget inventory

| LVGL Widget | `lvgl_cpp` Wrapper | Status | Notes |
| :--- | :--- | :--- | :--- |
| `lv_obj` | `core/object.h` | ⚠️ | 30.4% coverage. Needs behavior proxies. |
| `lv_event` | `core/event.h` | ❌ | 17.9% coverage. Needs EventProxy. |
| `lv_span` | `widgets/span.h` | 🚧 | Implementation in progress. |
| `lv_lottie` | `widgets/lottie.h` | ✅ | |
| `lv_vector` | `misc/vector.h` | ✅ | |

### 2.2. System modules

- **Display (`lv_display`)**: ✅ **DONE**. `display/display.h` provides full abstraction.
- **Input Device (`lv_indev`)**: ✅ **DONE**. `indev/input_device.h` covers inputs and cursors.
- **Group (`lv_group`)**: ✅ **DONE**. Core group management is functional.
- **File System (`lv_fs`)**: **PARTIAL**. `misc/file_system.h` exists but driver registration logic is complex.
- **Theme (`lv_theme`)**: ✅ **DONE**. `misc/theme.h` wrapper implemented.

## 3. Implementation roadmap

### Phase 4: Core subsystem proxies (upcoming)
**Goal**: Reach >60% total function coverage.
1.  **`EventProxy` (#147)**: Systematic wrapping of `lv_obj_add_event_cb` and standard event handlers.
2.  **`StateProxy` (#148)**: Fluent interface for widget states (Checked, Disabled, etc.).
3.  **`GroupProxy` (#149)**: RAII management for focus groups and device assignment.
4.  **`Span` Widget (#150)**: Complete the rich text implementation.

### Phase 5: Advanced media and hardening
1.  **Animation Timelines**: RAII-safe timeline management.
2.  **Lottie Optimizations**: Memory-safe data source handling.
3.  **Audit Automation**: Integrate `audit_api_coverage.py` into the CI flow.
