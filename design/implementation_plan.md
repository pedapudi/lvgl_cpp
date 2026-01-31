# Implementation Plan: lvgl_cpp Finalization (v0.3.0+)

This document outlines the strategic roadmap to complete the modernization of `lvgl_cpp` and achieve 100% architectural coverage of the core LVGL feature set. It bridges the gap between current progress and a cohesive, production-ready C++ framework.

## Executive Summary

The structural refactoring (mixins, CRTP widgets) is **complete** (v0.3.0). The next phases focus on **Systemic Ergonomics** (Proxies), **Resource Safety** (RAII), and **API Completeness**.

## Phase 1: Systemic Proxies (The fluent interface)

To eliminate the "Monolithic Object" problem where `lvgl::Object` has hundreds of methods, we will implement the **Proxy Pattern** for major subsystems. This parallels the successful `StyleProxy`.

### 1.1. LayoutProxy
**Goal**: Encapsulate Flex and Grid layout configuration.
- **Current State**: Raw `lv_obj_set_flex_*` calls on `Object`.
- **Target API**:
  ```cpp
  obj.layout().flex()
     .flow(FlexFlow::Row)
     .align(FlexAlign::Center, FlexAlign::SpaceBetween)
     .gap(10, 5);
  ```
- **Implementation**:
  - Create `core/layout_proxy.h`.
  - Add `LayoutProxy layout()` factory to `Object`.
  - Wrap `lv_flex_*` and `lv_grid_*` functions.

### 1.2. ScrollProxy
**Goal**: Fluent interface for scrolling and snapping.
- **Current State**: Scattered `scroll_to`, `set_scroll_snap` methods.
- **Target API**:
  ```cpp
  obj.scroll().to(0, 100, AnimEnable::On) // or Scroll::to_y(100)
     .snap_y(ScrollSnap::Center)
     .scrollbar_mode(ScrollbarMode::Auto);
  ```
- **Implementation**:
  - Create `core/scroll_proxy.h`.
  - Clean up `Scrollable` mixin (delegate to proxy or keep simple shortcuts).

## Phase 2: Animation & Lifecycle (RAII)

**Goal**: Ensure memory safety for asynchronous operations and callbacks.

### 2.1. Animation RAII
- **Problem**: `lv_anim_t` C callbacks are dangerous with C++ lambdas (dangling pointers).
- **Solution**:
  - Finalize `misc/animation.h` with `std::function` support (already in progress).
  - Ensure `Animation` destructor handles `lv_anim_del` correctly if strictly owned.
  - Implement `Animation::start()` returning a handle/future for completion tracking.

### 2.2. Timer RAII
- **Problem**: Similar memory risks with `lv_timer_create`.
- **Solution**:
  - `lvgl::Timer` class that wraps `lv_timer_t`.
  - Scoped lifetime: Timer stops/deletes when C++ object goes out of scope (unless detached).

## Phase 3: Widget & API Coverage

**Goal**: Reach >90% coverage for user-facing APIs.

### 3.1. Widget Gaps
- **SpanGroup**: Verify implementation depth. Add `Span` wrapper class if missing.
- **Canvas**: Implement C++ wrappers for drawing operations (`draw_rect`, `draw_text` on canvas).
- **MsgBox**: Ensure strict typing for buttons and text interactions.

### 3.2. Core API Hardening
- **Flags**: Ensure `add_flag` / `remove_flag` uses the type-safe `ObjFlag` enum everywhere.
- **Layers**: Wrap `lv_layer_top()` and `lv_layer_sys()` accessors in `Screen` or `Display`.
- **Input**: Verify `Indev` and `Group` interactions are fully wrapped (no raw `lv_indev_t*` needed in user code).

## Phase 4: Documentation & Polish

### 4.1. Migration Guide Finalization
- Document the shift from Mixins to Proxies (`style()`, `layout()`, `scroll()`).
- Provide distinct "C vs C++" comparison examples.

### 4.2. Example Projects
- Update all entries in `examples/` to use the new Fluent APIs.
- Create a "Kitchen Sink" demo showcasing all widgets and layout capabilities.

## Phase 5: Platinum Coverage Advancement

**Goal**: Elevate subsystems to 100% verified coverage by addressing identified gaps.

### 5.1. Input Device (lv_indev) -> Platinum
- **Gaps**: Missing 14 functions including `set_group`, `set_display`, `set_scroll_throw`.
- **Action**: Add missing methods to `InputDevice` class.
- **Verification**: `tests/test_indev_platinum.cpp`.

### 5.2. Style Properties (lv_style) -> Platinum [DONE]
- [x] 39 missing functions in `StyleProxy` (logical equivalence gaps).
- [x] Expand `StyleBase` and `StyleProxy` with properties like `radial_offset`, `opa_layered`, and filtered color accessors.
- [x] Verification: `tests/test_style_gaps.cpp`.

### 5.3. Vector & Draw (lv_draw) -> Modernization
- **Gaps**: 68 missing functions.
- **Action**: Create `DrawProxy` for specialized drawing tasks; expand `VectorDraw` with path and layer control.
- **Verification**: `tests/test_draw_platinum.cpp`.

## Execution Order

1. **Phase 1-2 Completion**: Ensure Layout/Scroll proxies and RAII are solid.
2. **Phase 5 Platinum Push**: Targeted gap bridging in `indev` and `style`.
3. **Phase 5 Draw Modernization**: Final push for graphics coverage.
4. **Documentation**: continuous updates.
