# Comprehensive API Coverage Report (LVGL v9.4)

This report provides a detailed breakdown of the `lvgl_cpp` wrapper coverage against the underlying LVGL C API.

## 1. Executive Summary

| Metric | Baseline (v9.4) | Wrapped | Coverage |
| :--- | :--- | :--- | :--- |
| **Total Functions** | 1,835 | 1,034 | **56.3%** |
| **Total Enums** | 859 | 296 | **34.5%** |

---

## 2. Coverage Buckets (Subsystems)

Subsystems are categorized by their coverage percentage to highlight areas of strength and opportunities for improvement.

### 🌟 Platinum (100%)
Complete wrapping of all user-facing APIs.

- **lv_textarea**: 37/37 (100.0%)
- **lv_arc**: 28/28 (100.0%)
- **lv_dropdown**: 24/24 (100.0%)
- **lv_menu**: 20/20 (100.0%)
- **lv_anim_timeline**: 19/19 (100.0%)
- **lv_bar**: 16/16 (100.0%)
- **lv_buttonmatrix**: 15/15 (100.0%)
- **lv_msgbox**: 12/12 (100.0%)
- **lv_roller**: 11/11 (100.0%)
- **lv_keyboard**: 11/11 (100.0%)
- **lv_tabview**: 11/11 (100.0%)
- **lv_line**: 9/9 (100.0%)
- **lv_led**: 7/7 (100.0%)
- **lv_imagebutton**: 6/6 (100.0%)
- **lv_lottie**: 6/6 (100.0%)
- **lv_list**: 5/5 (100.0%)
- **lv_tileview**: 5/5 (100.0%)
- **lv_win**: 5/5 (100.0%)
- **lv_checkbox**: 4/4 (100.0%)
- **lv_observer**: 4/4 (100.0%)
- **lv_switch**: 3/3 (100.0%)
- **lv_screen**: 3/3 (100.0%)
- **lv_async**: 2/2 (100.0%)
- **lv_spinner**: 2/2 (100.0%)
- **lv_button**: 1/1 (100.0%)

### 🥇 Gold (80% - 99.9%)
High-priority wrappers with only specialized or redundant methods missing.

- **lv_subject**: 30/32 (93.8%) - Missing specialized subject notification flags.
- **lv_vector**: 15/16 (93.8%) - Missing subtle path transform optimizations.
- **lv_table**: 16/17 (94.1%) - Missing low-level cell resize notification.
- **lv_canvas**: 13/14 (92.9%) - Missing raw internal buffer set-ptr.
- **lv_label**: 20/21 (95.2%) - Missing legacy text wrap mode getter.
- **lv_spinbox**: 19/20 (95.0%) - Missing internal cursor position setter.
- **lv_layer**: 4/5 (80.0%) - Core rendering layers covered.

### 🥈 Silver (50% - 79.9%)
Functional but incomplete. Good for production but lacks full depth.

- **lv_display**: 51/68 (75.0%) - Core display management is complete; missing driver-level events.
- **lv_style**: 95/145 (65.5%) - Fluent API covers all common properties; missing obscure filters.
- **lv_anim**: 39/62 (62.9%) - Basic and Timeline animations covered; missing custom bezier logic.
- **lv_indev**: 31/61 (50.8%) - Standard pointers/keypads covered; missing complex gesture recognizers.
- **lv_scale**: 23/34 (67.6%) - Linear and radial scales covered.
- **lv_chart**: 23/42 (54.8%) - Series and point management covered; missing legacy 1D array setters.
- **lv_fs**: 19/37 (51.4%) - File operations wrapped; missing driver registration hooks.
- **lv_group**: 14/23 (60.9%) - Focus management functional.
- **lv_spangroup**: 18/26 (69.2%) - Rich text support is strong.
- **lv_calendar**: 11/22 (50.0%) - Core widget coverage.

### 🥉 Bronze (< 50%)
Emerging coverage or complex architectural modules.

- **lv_obj**: 196/465 (42.2%) - Core logic is high; missing hundreds of generated style getters.
- **lv_draw**: 52/224 (23.2%) - Modern VectorDraw covered; missing software-rendering primitives.
- **lv_color**: 11/50 (22.0%) - Basic Palette and Hex covered; missing 16-bit specific math.
- **lv_event**: 7/39 (17.9%) - `EventProxy` handles all events; missing manual event code creation.
- **lv_font**: 2/24 (8.3%) - Basic built-in fonts; missing custom TTF/BPP load logic.

---

## 3. Enum & Constant Coverage

| Category | Coverage | Notes |
| :--- | :--- | :--- |
| **LV_EVENT** | 94.4% | Nearly all v9 events wrapped. |
| **LV_OBJ_FLAG** | 51.6% | Common usage flags covered. |
| **LV_ALIGN** | 95.7% | Complete layout alignment. |
| **LV_PALETTE** | 90.9% | Full color set. |
| **LV_STATE** | 100.0% | Functional widget states. |
| **LV_PART** | 90.0% | Widget parts for styling. |

---

## 4. Benchmark Gap Analysis (v2 Implementation)

### 4.1. Current Coverage
As of `2026-01-28`, the following benchmarks are implemented in `bench_suite` and reporting to `bench_report_v2.html`:

| Benchmark | Category | Wrappers Tested | Status |
| :--- | :--- | :--- | :--- |
| **Widgets_Slider** | Simple Widget | `lvgl::Slider` | 🟢 Active |
| **Widgets_Arc** | Simple Widget | `lvgl::Arc` | 🟢 Active |
| **Widgets_Switch** | Simple Widget | `lvgl::Switch` | 🟢 Active |
| **Widgets_Checkbox** | Simple Widget | `lvgl::Checkbox` | 🟢 Active |
| **Widgets_Table** | Complex Widget | `lvgl::Table` | 🟢 Active |
| **Widgets_Chart** | Complex Widget | `lvgl::Chart` | 🟢 Active |
| **Widgets_Span** | Complex Widget | `lvgl::SpanGroup` | 🟢 Active |
| **Core_Observer** | Mechanism | `lvgl::Subject`, `lvgl::Observer` | 🟢 Active |
| **Core_Style** | Mechanism | `lvgl::Style` | 🟢 Active |
| **Core_Timer** | Mechanism | `lvgl::Timer` | 🟢 Active |
| **Core_Group** | Mechanism | `lvgl::Group` | 🟢 Active |
| **Layout_Flex** | Layout | `lvgl::Object` (Flex) | 🟢 Active |
| **API_Fluent** | API | Method Chaining | 🟢 Active |

### 4.2. Identified Gaps (Missing Benchmarks)
The following benchmarks are defined in the design but currently **MISSING** or legacy (not ported):

#### 🔴 Critical Gaps (No Implementation)
1.  **Rendering Stress**: No benchmarks for `Layer` allocation, `Transform` operations, or `FPS` measurement.
2.  **Asset Loading**: `Font` loading and `Image` decoding are completely unmeasured.
3.  **Input Pipeline**: No input injection stress tests.

#### 🟡 Legacy Porting Needed
The following tests exist in `tests/` but are not part of `bench_suite`:
1.  **Fragmentation**: `bench_fragmentation.c/cpp` (Critical for embedded stability).
2.  **Churn Stability**: `bench_churn.c/cpp` (Measures heap health over time).
3.  **Event Dispatch**: `bench_events.c/cpp` (Measures `EventProxy` overhead).

---

## 5. Technical design: Generalized benchmarking framework (v2)

**Status:** Draft
**Related Issues:** #61, #80

## 1. Context and motivation

The current benchmarking infrastructure in `lvgl_cpp` has served as a functional prototype, successfully identifying key overheads in the C++ wrapper. However, as the library expands (Animations, Events, Layouts, Fluent APIs), the current ad-hoc method of "One Executable Per Benchmark" + "Regex Parsing in Python" is becoming a bottleneck.

### Critical pain points
1.  **High Friction:** Adding a new benchmark requires modifying `CMakeLists.txt`, `scripts/profile_runner.py`, and creating new source files.
2.  **Platform Coupling:** The system is tightly coupled to Linux `rusage` and `tcmalloc`, making it impossible to validate performance on actual embedded targets later.
3.  **Limited Metrics:** We only track Time, RSS, and Heap. We miss critical embedded metrics like "Binary Size" (code size impact of templates) and "Fragmentation" (internal LVGL allocator state).
4.  **Lack of Isolation:** While multiple binaries provide isolation, maintaining 20+ binaries is messy for the build system.

## 2. Systemic approach

We will transition from a "Script-Driven" architecture (where Python orchestrates `grep` on stdout) to a "Data-Driven" architecture. The core principle is **Inversion of Control**: the C++ benchmark suite should self-register tests and output structured data (JSON), which the Python layer simply consumes and visualizes.

### Architectural pillars

1.  **Macro-Based Registration:** similar to Google Benchmark or GTest. A developer should only need to write `LVGL_BENCHMARK(MyTest) { ... }` in a `.cpp` file, and the system automatically discovers it.
2.  **Single-Binary, Multi-Process:** We will build a single `bench_suite` executable containing all benchmarks. However, to ensure **memory isolation**, the Python runner will invoke this binary 100 times, once for each test (e.g., `./bench_suite --run=Widgets_Slider`). This combines valid build times with perfect test isolation.
3.  **Abstrated Metric Collection:** A `MetricCollector` interface will allow swapping implementation based on platform:
    *   **Host (Linux):** Uses `rusage`, `tcmalloc`, `/proc/self/maps`.
    *   **Embedded (Future):** Uses `lv_mem_monitor`, `heap_caps_get_info` (ESP32).

## 3. Implementation strategy

### 3.1 C++ layer (`lvgl_cpp/bench`)

We will introduce a lightweight, header-only (or static lib) benchmarking micro-framework.

#### **Interface (`bench.h`)**

```cpp
#pragma once
#include <vector>
#include <string>
#include <functional>

namespace lvgl::bench {

struct State {
    // helpers for loop iterations, etc.
    int iterations = 100;
};

// Base class for all benchmarks
class Benchmark {
public:
    virtual const char* name() const = 0;
    virtual void run(State& state) = 0;
};

// Registry singleton
class Registry {
public:
    static Registry& get();
    void register_benchmark(Benchmark* bench);
    void run(const std::string& pattern);
};

// Macro for easy registration
#define LVGL_BENCHMARK(Name) \
    class Bench_##Name : public lvgl::bench::Benchmark { \
    public: \
        const char* name() const override { return #Name; } \
        void run(lvgl::bench::State& state) override; \
    }; \
    static struct Register_##Name { \
        Register_##Name() { \
            lvgl::bench::Registry::get().register_benchmark(new Bench_##Name()); \
        } \
    } register_##Name; \
    void Bench_##Name::run(lvgl::bench::State& state)
}
```

#### **Usage Example**

```cpp
#include "lvgl_cpp/bench/bench.h"
#include "lvgl_cpp/widgets/button.h"

LVGL_BENCHMARK(Widgets_Button_Create) {
    for (int i = 0; i < state.iterations; ++i) {
        auto btn = std::make_unique<lvgl::Button>();
    }
}
```

#### **Run-time Logic**
The `main()` entry point (in `bench_main.cpp`) will check command line args.
- `--list`: Print JSON array of all registered tests.
- `--run=<Pattern>`: Execute matching tests.
- Output: Structured JSON to `stdout`.

```json
{
  "benchmark": "Widgets_Button_Create",
  "metrics": {
    "time_ns": 45000,
    "rss_kb": 1024,
    "heap_bytes": 500,
    "lv_fragmentation": 5
  }
}
```

### 3.2 Metric collection system

We need a tiered metric system.

1.  **System Metrics (OS Level):**
    *   Provided by default on Linux/MacOS.
    *   `RSS`, `Page Faults`.
2.  **Allocator Metrics (Heap Level):**
    *   If `ENABLE_PROFILING` (tcmalloc): `MallocExtension::instance()->GetAllocatedSize()`.
    *   If Default: `lv_mem_monitor()`.
3.  **Binary Metrics (Static):**
    *   This is calculated *offline* by the Python script using `bloaty` or `nm`, not at runtime.

### 3.3 Python layer (`scripts/profile_runner_v2.py`)

The new script will be simpler but more powerful.

1.  **Discovery:** Runs `./bench_suite --list` to get all available tests.
2.  **Orchestration:**
    *   For each test in List:
        *   Launch `./bench_suite --run=TestName` (sets fresh environment).
        *   Pass strict environment variables (e.g., `TCMALLOC_SAMPLE_PARAMETER`).
        *   Capture JSON output.
        *   Process `pprof` output if generated.
3.  **Reporting:**
    *   Updates the HTML generator to consume this new JSON structure.
    *   **New Feature:** "Delta Reporting". load `bench_baseline.json` (committed in repo?) and compare current run vs baseline.

### 3.4 Visualization and reporting

The generated `memory_report.html` will be significantly upgraded to provide actionable insights at a glance.

*   **Executive summary:** A top-level dashboard showing "Pass/Fail" status based on defined budgets (e.g., "Button must be < 512 bytes").
*   **Interactive trend charts:** Replacing static images with interactive JS charts (e.g., Chart.js) to visualize regression history over the last 50 commits.
*   **Delta view:** A specific view mode highlighting percent change vs. the `main` branch baseline.
*   **Flame graph integration:** Embed `pprof` SVGs directly into the report for deep-dive analysis of heap allocation hotspots.
*   **Binary size treemap:** (If Bloaty is active) A visual treemap showing which templates or functions contribute most to the binary size.

## 4. Migration plan

1.  **Phase 1 (Infrastructure):**
    *   Create `lvgl_cpp/bench/` directory.
    *   Implement `Registry`, `Benchmark` class, and `main` runner.
    *   Create the `bench_suite` target in CMake.
2.  **Phase 2 (Porting):**
    *   Migrate `bench_widgets.cpp` -> `bench/bench_widgets.cpp` using the new macro.
    *   Migrate `bench_events`, `bench_churn`.
3.  **Phase 3 (Tooling):**
    *   Update `profile_runner.py` to support the new mode.
    *   Remove old individual CMake targets.

## 5. Verification and tooling

### Verification
*   **Unit Test for Benchmarks:** A simple script that runs `bench_suite --run=NonExistent` and checks for graceful failure, or runs a dummy benchmark to verify JSON validity.

### New implementation tools
*   **Bloaty McBloatface:** (Required) Integration for binary size analysis.
*   **Valgrind Massif:** The runner should optionally support running under Massif for peak heap detailed analysis, although `pprof` is usually sufficient and faster.

## 6. Future work: Embedded target
The design allows compiling `bench_suite` for ESP32.
*   `LVGL_BENCHMARK` registration works identical in C++.
*   `main()` would just run all tests sequentially (since we can't `exec` on MCU).
*   `MetricCollector` would implement `lv_mem_monitor`.
*   Output would be printed to Serial Console (UART) in JSON format, which the Python runner can scrape.

## 7. Benchmark expansion strategy

Based on a comprehensive survey of the `lvgl_cpp` codebase, we have identified key areas requiring performance validation.

### 7.1 Core mechanisms
These low-level systems are used pervasively and must be highly optimized.

*   **Observer Binding:** `bench_core_observer`
    *   **Goal:** Measure overhead of `Subject::bind_flag_if_eq` vs manual `add_event_cb`.
    *   **Metric:** Time to propagate update to 100 listeners.
*   **Style System:** `bench_core_style`
    *   **Goal:** Measure cost of Fluid Style API vs raw `lv_style_set_*`.
    *   **Scenario:** Create 50 styles, set 10 properties each (color, opa, border, shadow, etc.).
*   **Animation Callbacks:** `bench_core_anim`
    *   **Goal:** Measure overhead of `std::function` vs raw C function pointer in `lv_anim_set_exec_cb`.
    *   **Scenario:** 100 concurrent animations updating strict integer values.
*   **Timer System:** `bench_core_timer`
    *   **Goal:** Measure lifecycle cost of `lvgl::Timer` (creation, callback proxy, deletion).
    *   **Scenario:** 100 periodic timers toggling a flag.
*   **Input Groups:** `bench_core_group`
    *   **Goal:** Measure overhead of `Group` management (adding/removing objects).
    *   **Scenario:** Create a Group and add 100 widgets to it.


### 7.2 Complex widgets
Beyond simple buttons, these widgets involve heavy initialization logic.

*   **Chart Population:** `bench_widget_chart`
    *   **Goal:** Measure `Chart::add_series` and massive point insertion `ChartSeries::set_next_value`.
    *   **Scenario:** Line chart with 2 series, 1000 points.
*   **Table Layout:** `bench_widget_table`
    *   **Goal:** Measure cell population overhead.
    *   **Scenario:** 10x10 Table, setting text for every cell.
*   **Message Box:** `bench_widget_msgbox`
    *   **Goal:** Measure creation cost of modal dialogs (Title, Text, Buttons).
*   **Lottie Animation:** `bench_widget_lottie`
    *   **Goal:** Measure parsing and initialization overhead of Lottie archives.
    *   **Scenario:** Load a standard small Lottie JSON wrapper.
*   **Rich Text:** `bench_widget_span`
    *   **Goal:** Measure `SpanGroup` layout with mixed styles.
    *   **Scenario:** 50 spans with different fonts/colors in a single group.

### 7.3 Layout and drawing
Verify abstractions don't introduce lag in layout passes.

*   **Flex Layout:** `bench_layout_flex`
    *   **Goal:** Measure layout recalculation time.
    *   **Scenario:** 50 items in a Flex wrapper, changing width of parent to trigger reflow.
*   **Grid Layout:** `bench_layout_grid`
    *   **Goal:** Measure grid placement cost.
    *   **Scenario:** 10x10 grid with `set_grid_cell` called on children.
*   **Fluent Drawing:** `bench_draw_canvas`
    *   **Goal:** Canvas buffer manipulation.
    *   **Scenario:** `Canvas::draw_rect`, `Canvas::draw_text` loop.
*   **Image Decoding:** `bench_draw_image`
    *   **Goal:** Measure `ImageDescriptor` creation and header parsing.
    *   **Scenario:** Opening 50 distinct image sources.
*   **Display Initialization:** `bench_draw_display`
    *   **Goal:** Measure `Display::auto_configure_buffers` and screen setup.
    *   **Scenario:** Create 10 virtual displays with double buffering.
*   **Vector Graphics:** `bench_draw_vector`
    *   **Goal:** Measure `VectorPath` construction and `VectorDraw` command recording.
    *   **Scenario:** Build a complex path (cubics, arcs) and apply gradient stroke.

### 7.4 Misc and utilities
*   **File System:** `bench_misc_fs`
    *   **Goal:** Measure overhead of `lvgl::File` abstraction over `lv_fs_*`.
    *   **Scenario:** Open/Read/Close 100 small files.
*   **Tree Traversal:** `bench_core_tree`
    *   **Goal:** Measure `TreeProxy` traversal vs raw `lv_obj_get_child`.
    *   **Scenario:** Deeply nested hierarchy (depth 10, width 2), iterate all.
*   **Async Dispatch:** `bench_misc_async`
    *   **Goal:** Measure thread-safe callback overhead.
    *   **Scenario:** Queue 1000 async calls.
*   **Font Loading:** `bench_misc_font`
    *   **Goal:** Measure `OwnedFont::load_bin` vs raw `lv_binfont_create`.
    *   **Scenario:** Load a simulated 20KB binary font.
*   **Log System:** `bench_misc_log`
    *   **Goal:** Measure `lvgl::Log` overhead with and without active listeners.
    *   **Scenario:** Log 1000 INFO messages with formatting.

### 7.5 API comparisons
*   **Fluent vs Standard:** `bench_api_fluent`
    *   **Goal:** Verify that `obj.set_size().set_pos()` incurs zero overhead over `obj.set_size(); obj.set_pos();`.

### 7.6 Input processing
*   **Input Injection:** `bench_indev_inject`
    *   **Goal:** Measure latency of input processing pipeline.
    *   **Scenario:** Inject 100 pointer events into a `PointerInput` device.

---
**References:**
*   [Google Benchmark](https://github.com/google/benchmark) (Inspiration)
*   [Catch2](https://github.com/catchorg/Catch2) (Registration style)
