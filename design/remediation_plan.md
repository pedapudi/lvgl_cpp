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
  - **Resolution**: Implemented `bench_soak` custom target and `--duration` support in `bench_churn` (PR #89).
  - **Action**: Run nightly soak tests.

- **Issues #78 (Widget Coverage) & #77 (Trendlines)**
  - **Resolution**: `bench_widgets` expanded to cover complex widgets (`Chart`, `Table` etc) in PR #87.
  - **Action**: Monitor stability.

## 3. API Ergonomics & Completeness
**Status**: Fluent API is popular but inconsistent (#83). Font handling is primitive (#65).
**Goal**: Make the C++ wrapper usage seamless and idiomatic.

- **Issue #83 (Fluent Style Setters)**
  - **Resolution**: Implemented `Stylable<T>` mixin in `core/mixins/stylable.h`.
  - **Status**: **Resolved** in PR #85.

- **Issue #65 (OwnedFont)**
  - **Resolution**: Created `OwnedFont` RAII wrapper and `Font` view.
  - **Status**: **Resolved** in PR #86.

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

### Phase 2: Core Features (Completed)
- [x] Implement `FluentStyle` mixin (#83).
- [x] Implement `Font` wrapper (#65).

### Phase 3: Stability Hardening (Completed)
- [x] Implement `bench_soak` target (#80).
- [x] Expand `bench_churn` coverage (#78).
