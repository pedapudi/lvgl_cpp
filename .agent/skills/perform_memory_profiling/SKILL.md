---
name: perform_memory_profiling
description: Guide for running memory benchmarks, analyzing heap usage, and extending the profiling suite.
---

# Perform Memory Profiling

This skill effectively utilizes the `lvgl_cpp` memory profiling suite to detect leaks, measure overhead, and verify architectural stability.

## 1. Running the Profiling Suite

The profiling system is accessible via a Python runner script in `lvgl_cpp/scripts/`.

### Prerequisites
*   Build with profiling enabled: `-DENABLE_PROFILING=ON`
*   Install `google-pprof` (or `pprof`) and `graphviz` (for SVG generation).

### Execution
Run the runner from the `lvgl_cpp` directory:

```bash
cd lvgl_cpp
# Ensure fresh build with profiling symbols
rm -rf build && cmake -B build -S . -DENABLE_PROFILING=ON
cmake --build build -j$(nproc)

# Execute Runner
python3 scripts/profile_runner.py
```

## 2. Interpreting the Report (`memory_report.html`)

The runner generates an interactive HTML report at `lvgl_cpp/memory_report.html`.

### Key Metrics
*   **RSS (Resident Set Size)**: Total physical memory used. Good for spotting gross leaks or high base overhead.
*   **Heap (Bytes)**: Precise allocation data via `TCMalloc`.
*   **Time (ms)**: Execution time for the benchmark loop.

### Visualizations
*   **Trend Analysis**: Checks if memory usage grows linearly ($O(n)$) or stays constant ($O(1)$) with object count.
*   **Stability Test**: "Churn" test (create/destroy cycles). Slope should be ~0.
*   **Heap Call Graphs**: Click "View Heap Graph" to see an SVG call graph of memory allocation paths. Nodes show bytes allocated; edges show code paths.

## 3. Adding New Benchmarks

Benchmarks are paired: a C baseline (`bench_foo.c`) vs a C++ wrapper (`bench_foo.cpp`).

### Step 1: Create Source Files
Create files in `lvgl_cpp/tests/`.

**C Baseline (`tests/bench_mywidget.c`):**
```c
#include "lvgl.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    lv_init();
    // ... setup ...
    int N = (argc > 1) ? atoi(argv[1]) : 50;
    
    for(int i=0; i<N; i++) {
        lv_obj_t* obj = lv_button_create(lv_screen_active());
        // ... manipulate ...
    }
    return 0;
}
```

**C++ Wrapper (`tests/bench_mywidget.cpp`):**
```cpp
#include "lvgl.h"
#include "lvgl_cpp/core/object.h"
#include "lvgl_cpp/widgets/button.h"
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    lv_init();
    int N = (argc > 1) ? std::stoi(argv[1]) : 50;

    {
        std::vector<std::unique_ptr<lvgl::Button>> buttons;
        for(int i=0; i<N; i++) {
            buttons.push_back(std::make_unique<lvgl::Button>());
        }
    } // RAII destruction triggered here
    return 0;
}
```

### Step 2: Register in `CMakeLists.txt`
```cmake
add_benchmark(bench_mywidget tests/bench_mywidget.cpp)
add_benchmark(bench_mywidget_c tests/bench_mywidget.c)
```

### Step 3: Register in `scripts/profile_runner.py`
Add to the `BENCHMARKS` list:
```python
BENCHMARKS = [
    # ...
    ("bench_mywidget_c", "bench_mywidget"),
]

# Add description
bench_descriptions = {
    "bench_mywidget": "<b>Objective:</b> Measure overhead of MyWidget wrapper."
}
```

## 4. Troubleshooting & Implementation Details

### `pprof` Integration
*   **SVG Truncation**: `pprof` outputs SVGs with `width="100%"`. The runner (as of v2.5) explicitly extracts the graph's bounding box from the polygon element and sets `width`/`height` in `pt` to ensure scrollability.
*   **Empty Heap Graphs**: If graphs are empty or 0 bytes, ensure `TCMALLOC_SAMPLE_PARAMETER=1` is set in the environment variables (handled by runner) to force sampling of every allocation.

### Memory Configuration
*   **OOM Crashes**: `lv_conf_test.h` often defaults to small memory (128KB). For benchmarks, ensure `LV_MEM_SIZE` is at least 4MB (`4096 * 1024`).

### Run-time Dependencies
The runner requires `google-pprof` (often symlinked as `pprof`) and `graphviz` (for `dot`) to be in the `$PATH`.
