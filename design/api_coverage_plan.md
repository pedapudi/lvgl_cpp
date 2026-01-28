# API coverage and expansion plan (v0.4.0+)

This document outlines the roadmap for achieving near-100% API coverage of LVGL v9 features within the `lvgl_cpp` framework.

## 1. Executive summary

Current `lvgl_cpp` coverage stands at **56.3%** for functions and **34.5%** for enums based on the latest v9.4 inclusive user-facing audit.

## 2. Coverage audit (v9.4 baseline - user-facing inclusive)

- **Total Functions**: 1,835 | **Used**: 1,034 | **Coverage**: 56.3%
- **Total Enums**: 859 | **Used**: 296 | **Coverage**: 34.5%

### 2.1. Widget inventory

| LVGL Widget | `lvgl_cpp` Wrapper | Status | Notes |
| :--- | :--- | :--- | :--- |
| `lv_obj` | `core/object.h` | ⚠️ | 42.2% coverage. Includes behavior proxies. |
| `lv_anim` | `misc/animation.h` | ⚠️ | 62.9% coverage. |
| `lv_event` | `core/event.h` | ✅ | Improved via `EventProxy`. |
| `lv_span` | `widgets/span.h` | ✅ | Phase 4 implementation complete. |
| `lv_lottie` | `widgets/lottie.h` | ✅ | |
| `lv_vector` | `misc/vector.h` | ✅ | |

### 2.2. System modules

- **Display (`lv_display`)**: ✅ **DONE**. `display/display.h` provides full abstraction.
- **Input Device (`lv_indev`)**: ✅ **DONE**. `indev/input_device.h` covers inputs and cursors.
- **Group (`lv_group`)**: ✅ **DONE**. Core group management is functional.
- **File System (`lv_fs`)**: **PARTIAL**. `misc/file_system.h` exists but driver registration logic is complex.
- **Theme (`lv_theme`)**: ✅ **DONE**. `misc/theme.h` wrapper implemented.

## 3. Implementation roadmap

### Phase 4: Core subsystem proxies (COMPLETE)
**Goal**: Reach >53% total function coverage.
1.  **\`EventProxy\` (#147)**: Systematic wrapping of \`lv_obj_add_event_cb\` and standard event handlers. ✅
2.  **\`StateProxy\` (#148)**: Fluent interface for widget states (Checked, Disabled, etc.). ✅
3.  **\`GroupProxy\` (#149)**: RAII management for focus groups and device assignment. ✅
4.  **\`Span\` Widget (#150)**: Complete the rich text implementation. ✅

### Phase 5: Advanced media and hardening
1.  **Complete Animation Timeline**: Add support for repeats, delays, and merging (`lv_anim_timeline_merge`).
2.  **Lottie Optimizations**: Memory-safe data source handling for Lottie animations.
3.  **Audit Automation**: Integrate `audit_api_coverage.py` into the CI flow to prevent coverage regressions.

### Phase 9: Core Usage API Expansion (#163) (PLANNED)
**Goal**: Increase `lv_obj` function coverage to >80%.
1.  **Proxies**: Introduce `InteractionProxy` and `TreeProxy` to modularize object API.
2.  **Types**: standardize `Area` and `Point` usage.
3.  **Features**: Implement missing scrolling getters, coordinate transforms, and tree swapping.
