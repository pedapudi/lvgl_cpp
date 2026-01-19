# Memory test suite design plan

**Tracking Issue:** #68

## Objective
Empirically measure heap usage, detect memory leaks, and generate detailed comparative profiles between `lvgl_cpp` and pure C `lvgl`.

## Methodology

### 1. Profiling tools integration (`pprof`)
We will use Google's `gperftools` (specifically `tcmalloc`) to capture high-fidelity heap profiles.

**Why `pprof`?**
- **Call Graph Attribution**: Accurately blames allocations to specific functions (e.g., distinguishing `std::vector` resize from `std::function` allocation).
- **Differential Profiling**: `pprof --base` allows subtracting the C baseline profile from the C++ profile to isolate the wrapper overhead.

### 2. Test scenarios

Each scenario will be compiled as a separate executable to ensure clean heap state.

#### Scenario A: "Baseline Widget"
- **Task**: Create 1000 `Button` objects, set position, set size.
- **Metric**: Fixed overhead per instance.

#### Scenario B: "Event Heavy"
- **Task**: Create 1000 `Button` objects, add 1 lambda event callback to each.
- **Metric**: Dynamic overhead of `std::function` + `vector` + `wrappers`.

#### Scenario C: "Complex Layout"
- **Task**: Create a nested Flex/Grid layout with mixed C++ and raw C objects.
- **Metric**: Validation of "Zero Overhead" claims for mixins.

#### Scenario D: "Long-Run Stability (Churn)"
- **Task**: Loop 1,000,000 times: Create Screen -> Add 10 Widgets -> Destroy Screen.
- **Metric**: Pass/Fail on OOM. Detecting slow leaks that `valgrind` might miss due to false positives in intrusive pointers.

#### Scenario E: "Fragmentation Analysis"
- **Task**: Randomly allocate/deallocate 1,000 closures of varying sizes.
- **Metric**: "Largest Free Block" (High water mark). If Total Free is 20KB but Largest Block is 1KB, we have fatal fragmentation.

### 3. Implementation plan

#### 3.1 Build system updates
- Add `GPerfTools` dependency to CMake (optional, enabled via `-DENABLE_PROFILING=ON`).
- Link tests with `-lprofiler` and `-ltcmalloc`.

#### 3.2 Test runner script (`profile_runner.py`)
A wrapper script to execute tests and capture profiles.

```python
# Pseudo-code for runner logic
def run_comparison(test_name):
    # 1. Run C Implementation
    env = {"HEAPPROFILE": f"/tmp/{test_name}_c"}
    subprocess.run(f"./build/tests/bench_{test_name}_c", env=env)
    
    # 2. Run C++ Implementation
    env = {"HEAPPROFILE": f"/tmp/{test_name}_cpp"}
    subprocess.run(f"./build/tests/bench_{test_name}_cpp", env=env)
    
    # 3. Generate Diff Report
    # Uses `pprof --text --base=...` to show exactly what C++ added
    subprocess.run(f"pprof --text --base=/tmp/{test_name}_c.0001.heap /tmp/{test_name}_cpp.0001.heap")
```

#### 3.3 Comparative report format

The final output will be a Markdown report generated automatically.

**Example Output:**

| Metric | Pure C (lvgl) | C++ (lvgl_cpp) | Delta | Overhead per Object |
| :--- | :--- | :--- | :--- | :--- |
| **Total Heap** | 120 KB | 168 KB | +48 KB | +48 bytes |
| **Alloc Count** | 1000 | 2000 | +1000 | +1 alloc |

**Detailed Profile (pprof diff):**
```text
   48.5MB  98.1%  98.1%   48.5MB  98.1% lvgl::Object::add_event_cb
    0.5MB   1.1%  99.2%    0.5MB   1.1% std::vector::push_back
```

#### 3.4 Static Analysis & Performance Budget
We will enforce design constraints at compile time where possible.

- **Clang-Tidy Checks**:
    - `performance-unnecessary-value-param`: Detect large objects passed by value.
    - `bugprone-unused-raii`: Detects if wrappers are created but discarded immediately.
- **CI Budget**:
    - "Test B (Event Heavy) must not exceed 64 bytes per callback average."
    - "Base Object size must remain <= 24 bytes."

### 4. Leak detection strategy (Embedded/ESP32)

For targets where `pprof` is unavailable (ESP32), we fall back to the "Net Allocation Check":

```cpp
void test_leak_cycle() {
    size_t before = get_free_heap();
    {
        lvgl::Button btn; // Life cycle
    }
    size_t after = get_free_heap();
    if (before != after) ERROR("Leak detected: %d bytes", before - after);
}
```

## Infrastructure requirements
- Linux Host for `pprof` analysis (CI environment).
- `libgoogle-perftools-dev` package.
- Python 3 for the runner script.
