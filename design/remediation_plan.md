# Systemic Remediation Plan

## Executive Summary
This document outlines a comprehensive plan to address current open issues in `lvgl_cpp` by categorizing them into systemic themes. It leverages recent architectural improvements (Input Device Refactor, Memory Design verification) to close technical debt and proposes a roadmap for remaining feature requests.

## 1. Architectural Consolidation
**Status**: Critical infrastructure is in place.
**Goal**: Verify and close resolved issues, ensuring no "zombie" tasks remain.

- **Issue #66 (Input Hierarchy)** & **#67 (EventSource Memory)**
  - **Analysis**: Addressed by `feat/issue-66-input-architect-review` and `feat/issue-67-memory-design`.
  - **Resolution**: Strict type safety and memory leak fixes are merged into `main`. The `test_input_device` and `EventSource` leak tests pass.
  - **Action**: Mark as **Resolved**.

## 2. Observability & Stability (Profiling)
**Status**: `lvgl_profiling_v2` is merged (#79), benchmark suite is expanded (#80).
**Goal**: Transition from "Experimental" to "Standard Operating Procedure".

- **Issue #80 (Churn Stability)**
  - **Gap**: `bench_churn` exists but runs for a fixed duration.
  - **Plan**: Create a `long_soak` CMake target that runs `bench_churn` for extended periods (e.g., 1 hour) with aggressive memory limits.

- **Issues #78 (Widget Coverage) & #77 (Trendlines)**
  - **Gap**: Current tests cover major widgets. Need a systematic audit to ensure *every* widget type is instantiated in the churn test.
  - **Plan**: Update `bench_churn.c` to iterate through a registry of all widget creation functions.

## 3. API Ergonomics & Completeness
**Status**: Fluent API is popular but inconsistent (#83). Font handling is primitive (#65).
**Goal**: Make the C++ wrapper usage seamless and idiomatic.

- **Issue #83 (Fluent Style Setters)**
  - **Problem**: Users have to grab `Style` objects for simple tweaks.
  - **Solution**: Implement "Mixin" pattern for common style properties (e.g., `Stylable<T>`).
  - **Action**: Design `core/mixins/fluent_style.h` to autogenerate fluent setters for `set_bg_color`, `set_pad_all`, etc., for any widget.

- **Issue #65 (OwnedFont)**
  - **Problem**: Raw `lv_font_t*` usage is unsafe and lacks lifecycle management.
  - **Solution**: Create `Font` class with `load/unload` capabilities and `freetype` integration hooks.

## 4. Systemic Quality Assurance
**Status**: Unit tests are robust (`test_header_integrity` added).
**Goal**: Prevent regression in API consistency.

- **Header Integrity**: Solved by `scripts/check_headers.py`.
- **API Consistency**:
  - **Risk**: New widgets might miss fluent methods.
  - **Plan**: Add a `clang-query` or python-based AST check to ensure all `widgets/*.h` classes return `*this` reference for setters.

## Implementation Roadmap

### Phase 1: Cleanup (Immediate)
- Close #66, #67, #74, #75, #81, #82 (Done via merge).
- Review #80, #79 against `profiling-v2` capabilities.

### Phase 2: Core Features (Short Term)
- Implement `FluentStyle` mixin (#83).
- Implement `Font` wrapper (#65).

### Phase 3: Stability Hardening (Medium Term)
- Implement `bench_soak` target.
- Expand `bench_churn` coverage.
