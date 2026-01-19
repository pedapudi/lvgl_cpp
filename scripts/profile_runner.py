#!/usr/bin/env python3
import os
import subprocess
import sys
import re
import json
import datetime

# Requirements:
# - CMAKE build directory at ./build or provided via args
# - pprof installed (google-perftools)

BUILD_DIR = "build"
BENCHMARKS = [
    ("bench_baseline_c", "bench_baseline"),
    ("bench_events_c", "bench_events"),
    ("bench_churn_c", "bench_churn"),
    ("bench_fragmentation_c", "bench_fragmentation"),
]

class BenchmarkResult:
    def __init__(self, name, lang):
        self.name = name
        self.lang = lang
        self.time_ms = 0.0
        self.rss_kb = 0
        self.heap_bytes = 0
        self.status = "PENDING"
        self.error_msg = ""

    def to_dict(self):
        return {
            "name": self.name,
            "lang": self.lang,
            "time_ms": self.time_ms,
            "rss_kb": self.rss_kb,
            "heap_bytes": self.heap_bytes,
            "status": self.status,
            "error_msg": self.error_msg
        }

def run_cmd(cmd, env=None, capture_output=True):
    try:
        start_env = os.environ.copy()
        if env:
            start_env.update(env)
        
        args = {
            "shell": True, 
            "check": True, 
            "env": start_env,
            "stdout": subprocess.PIPE if capture_output else None,
            "stderr": subprocess.PIPE if capture_output else None
        }
            
        result = subprocess.run(cmd, **args)
        return True, result.stdout.decode('utf-8', errors='ignore'), result.stderr.decode('utf-8', errors='ignore')
            
    except subprocess.CalledProcessError as e:
        stdout = e.stdout.decode('utf-8', errors='ignore') if e.stdout else ""
        stderr = e.stderr.decode('utf-8', errors='ignore') if e.stderr else ""
        return False, stdout, stderr

def parse_metrics(output, result_obj):
    # Format: BENCHMARK_METRIC: KEY=VALUE unit=UNIT
    for line in output.splitlines():
        if "BENCHMARK_METRIC" in line:
            try:
                parts = line.split(":")
                if len(parts) < 2: continue
                kv_part = parts[1].strip()
                # key=value unit=unit
                k_v = kv_part.split(" ")[0].split("=")
                key = k_v[0]
                val = float(k_v[1])
                
                if key == "TIME":
                    result_obj.time_ms = val
                elif key == "RSS":
                    result_obj.rss_kb = int(val)
            except Exception as e:
                print(f"Error parsing metric line '{line}': {e}")

def get_heap_file(prefix):
    # Returns the latest heap file for a given prefix in /tmp
    try:
        files = [f for f in os.listdir("/tmp") if f.startswith(prefix) and f.endswith(".heap")]
        if not files: return None
        # Sort by modification time or name (name usually contains seq number)
        files.sort(key=lambda x: os.path.getmtime(os.path.join("/tmp", x)))
        return f"/tmp/{files[-1]}"
    except OSError:
        return None

def get_pprof_heap_bytes(binary_path, heap_file):
    if not heap_file or not os.path.exists(heap_file):
        return 0
    
    # Run pprof to get text output showing total memory
    # pprof --text binary heap_file
    # Output first line usually: "Total: 123.4MB"
    # Or parsing the top line summary.
    
    # Better approach: "pprof --bytes --text ..." to get bytes directly ?
    # pprof doesn't always support --bytes in all versions.
    # Standard: "Total: 10.5 MB"
    
    success, out, err = run_cmd(f"pprof --text {binary_path} {heap_file} | head -n 1")
    if success and "Total:" in out:
        # Example: "Total: 1024.0 B" or "Total: 1.5 MB"
        try:
            parts = out.strip().split()
            # parts[0] is "Total:", parts[1] is value, parts[2] is unit
            val = float(parts[1])
            unit = parts[2]
            
            multipliers = {
                "B": 1,
                "kB": 1024,
                "KB": 1024,
                "MB": 1024*1024,
                "GB": 1024*1024*1024
            }
            return int(val * multipliers.get(unit, 1))
        except:
            pass
    return 0

def generate_html_report(results, filename="memory_report.html"):
    # Structuring data for Chart.js
    labels = []
    
    # Datasets
    time_c = []
    time_cpp = []
    rss_c = []
    rss_cpp = []
    heap_c = []
    heap_cpp = []
    
    # Process paired results
    # Assumes results list is ordered: C, Cpp, C, Cpp...
    # Or we can map by name format
    
    # Let's organize by benchmark base name
    bench_data = {} # name -> {c: res, cpp: res}
    
    for res in results:
        base_name = res.name.replace("_c", "").replace("_cpp", "").replace("_baseline", "Baseline").replace("_events", "Events").replace("_churn", "Churn").replace("_fragmentation", "Fragmentation")
        if base_name == "bench": base_name = "Baseline" # fix for bench_baseline -> Baseline
        
        # Cleaner logic
        if "_c" in res.name:
            key = res.name.replace("bench_", "").replace("_c", "")
            if key not in bench_data: bench_data[key] = {}
            bench_data[key]["c"] = res
        else:
            key = res.name.replace("bench_", "").replace("_cpp", "")
            if key not in bench_data: bench_data[key] = {}
            bench_data[key]["cpp"] = res

    # Sort keys for consistent order
    sorted_keys = sorted(bench_data.keys())
    
    for key in sorted_keys:
        labels.append(key.capitalize())
        pair = bench_data[key]
        
        c_res = pair.get("c")
        cpp_res = pair.get("cpp")
        
        time_c.append(c_res.time_ms if c_res else 0)
        time_cpp.append(cpp_res.time_ms if cpp_res else 0)
        
        rss_c.append(c_res.rss_kb if c_res else 0)
        rss_cpp.append(cpp_res.rss_kb if cpp_res else 0)
        
        heap_c.append(c_res.heap_bytes if c_res else 0)
        heap_cpp.append(cpp_res.heap_bytes if cpp_res else 0)

    bench_descriptions = {
        "Baseline": """
            <strong>Objective:</strong> Measure the fixed overhead of creating static LVGL objects via C++ wrappers.<br>
            <strong>Methodology:</strong> The test creates <strong>50</strong> <code>lvgl::Button</code> objects. Each button is wrapped in a <code>std::unique_ptr</code>, positioned, and sized. 
            This validates the baseline cost of the C++ class hierarchy, vtable overhead, and RAII container usage.<br>
            <strong>Comparison:</strong> Compares against equivalent C code calling <code>lv_button_create</code>, <code>lv_obj_set_pos</code>, and <code>lv_obj_set_size</code> directly.
        """,
        "Events": """
            <strong>Objective:</strong> Measure the overhead of the C++ event dispatch mechanism, specifically <code>std::function</code> storage and lambda copying.<br>
            <strong>Methodology:</strong> Creates <strong>50</strong> <code>lvgl::Button</code> objects and attaches a C++ lambda callback to each user <code>add_event_cb</code>. 
            The lambda is stateless but exercises the type-erasure mechanics of <code>std::function&lt;void(lvgl::Event&)&gt;</code>.<br>
            <strong>Comparison:</strong> Compares against C code attaching a static C function pointer <code>event_cb</code> via <code>lv_obj_add_event_cb</code>.
        """,
        "Churn": """
            <strong>Objective:</strong> Detect slow memory leaks or fragmentation over repeated create/destroy cycles.<br>
            <strong>Methodology:</strong> Runs <strong>100 cycles</strong>. In each cycle, it creates a <code>lvgl::Screen</code>, populates it with <strong>20</strong> <code>lvgl::Button</code> objects (each with a lambda callback), 
            processes events via <code>lv_timer_handler</code>, and then destroys the screen. This tests the destructors and parent-child cleanup mechanisms.<br>
            <strong>Scale:</strong> 2,000 total allocations/deallocations.<br>
            <strong>Comparison:</strong> Compares against C code performing the exact same create/destroy cycle logic.
        """,
        "Fragmentation": """
            <strong>Objective:</strong> Stress the heap allocator with randomized allocation patterns to measure fragmentation overhead from C++ closures.<br>
            <strong>Methodology:</strong> Runs <strong>50 iterations</strong> with an inner loop of 50 operations. Operations are randomized:
            <ul>
                <li><strong>Alloc (33%):</strong> Creates a button and attaches a lambda capturing a 64-byte struct <code>CaptureState</code>.</li>
                <li><strong>Free (33%):</strong> Deletes a random existing button (and its closure).</li>
                <li><strong>No-op (33%):</strong> Idle.</li>
            </ul>
            <strong>Comparison:</strong> Compares against C code creating buttons with standard C callbacks (no capture context), highlighting the specific cost of stateful C++ closures.
        """
    }

    html_content = f"""
    <!DOCTYPE html>
    <html>
    <head>
        <title>LVGL C++ Binding Memory Profile</title>
        <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
        <style>
            :root {{
                --primary: #2563eb;
                --secondary: #64748b;
                --success: #16a34a;
                --danger: #dc2626;
                --bg: #f8fafc;
                --card-bg: #ffffff;
                --text: #1e293b;
                --border: #e2e8f0;
            }}
            body {{ font-family: 'Inter', system-ui, -apple-system, sans-serif; background: var(--bg); color: var(--text); margin: 0; padding: 40px; line-height: 1.6; }}
            .container {{ max_width: 1200px; margin: 0 auto; }}
            h1 {{ text-align: center; color: var(--text); font-size: 2.5rem; margin-bottom: 0.5rem; }}
            .subtitle {{ text-align: center; color: var(--secondary); margin-bottom: 3rem; }}
            
            .grid {{ display: grid; grid-template-columns: repeat(auto-fit, minmax(400px, 1fr)); gap: 24px; margin-bottom: 40px; }}
            .card {{ background: var(--card-bg); padding: 24px; border-radius: 12px; box-shadow: 0 4px 6px -1px rgba(0,0,0,0.1); border: 1px solid var(--border); }}
            
            .chart-container {{ position: relative; height: 400px; }}
            
            .benchmark-section {{ display: flex; flex-direction: column; gap: 30px; }}
            .benchmark-card {{ background: white; border-radius: 12px; overflow: hidden; box-shadow: 0 2px 4px rgba(0,0,0,0.05); border: 1px solid var(--border); }}
            .benchmark-header {{ background: #f8fafc; padding: 20px 24px; border-bottom: 1px solid var(--border); display: flex; justify-content: space-between; align-items: center; }}
            .benchmark-header h2 {{ margin: 0; font-size: 1.25rem; color: var(--text); }}
            
            .benchmark-content {{ padding: 24px; display: grid; grid-template-columns: 1fr 1fr; gap: 32px; }}
            .benchmark-desc {{ color: var(--secondary); font-size: 0.95rem; }}
            .benchmark-desc strong {{ color: var(--text); font-weight: 600; }}
            .benchmark-desc ul {{ padding-left: 20px; margin: 8px 0; }}
            
            .benchmark-stats {{ display: grid; grid-template-columns: 1fr 1fr; gap: 16px; align-content: start; }}
            .stat-box {{ background: #f8fafc; padding: 16px; border-radius: 8px; border: 1px solid var(--border); }}
            .stat-box.cpp {{ background: #eff6ff; border-color: #bfdbfe; }}
            .stat-box h3 {{ margin: 0 0 12px 0; font-size: 0.85rem; text-transform: uppercase; letter-spacing: 0.05em; color: var(--secondary); }}
            .stat-row {{ display: flex; justify-content: space-between; margin-bottom: 8px; font-size: 0.9rem; }}
            .stat-row:last-child {{ margin-bottom: 0; }}
            .stat-val {{ font-weight: 600; font-family: monospace; }}
            
            .badge {{ display: inline-flex; align-items: center; padding: 2px 8px; border-radius: 4px; font-size: 0.7rem; font-weight: 600; text-transform: uppercase; }}
            .badge-pass {{ background: #dcfce7; color: var(--success); }}
            .badge-fail {{ background: #fee2e2; color: var(--danger); }}
            
            @media (max-width: 768px) {{
                .benchmark-content {{ grid-template-columns: 1fr; }}
            }}
        </style>
    </head>
    <body>
        <div class="container">
            <h1>LVGL C++ Memory Profile</h1>
            <p class="subtitle">Generated on {datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")}</p>
            
            <div class="card" style="margin-bottom: 40px;">
                <h2 style="margin-top:0;">Performance Overview</h2>
                <div class="grid" style="margin-bottom:0;">
                    <div class="chart-container">
                        <canvas id="rssChart"></canvas>
                    </div>
                    <div class="chart-container">
                        <canvas id="timeChart"></canvas>
                    </div>
                </div>
            </div>
            
            <div class="benchmark-section">
    """
    
    for key in sorted_keys:
        pair = bench_data[key]
        c_res = pair.get('c')
        cpp_res = pair.get('cpp')
        
        desc = bench_descriptions.get(key.capitalize(), "No description available.")
        
        # Helper to render stat box
        def render_stat_box(res, title, extra_class=""):
            if not res: return f'<div class="stat-box {extra_class}"><h3>{title}</h3><p>No Data</p></div>'
            status_class = "badge-pass" if res.status == "SUCCESS" else "badge-fail"
            return f"""
                <div class="stat-box {extra_class}">
                    <div style="display:flex; justify-content:space-between; margin-bottom:12px;">
                        <h3>{title}</h3>
                        <span class="badge {status_class}">{res.status}</span>
                    </div>
                    <div class="stat-row"><span>Time:</span> <span class="stat-val">{res.time_ms:.2f} ms</span></div>
                    <div class="stat-row"><span>RSS:</span> <span class="stat-val">{res.rss_kb} KB</span></div>
                    <div class="stat-row"><span>Heap:</span> <span class="stat-val">{res.heap_bytes} B</span></div>
                </div>
            """

        html_content += f"""
                <div class="benchmark-card">
                    <div class="benchmark-header">
                        <h2>{key}</h2>
                    </div>
                    <div class="benchmark-content">
                        <div class="benchmark-desc">
                            {desc}
                        </div>
                        <div class="benchmark-stats">
                            {render_stat_box(c_res, "C Baseline")}
                            {render_stat_box(cpp_res, "C++ Wrapper", "cpp")}
                        </div>
                    </div>
                </div>
        """
    
    html_content += """
            </div>
        </div>
        <script>
    
            Chart.defaults.font.family = "'Inter', sans-serif";
            Chart.defaults.color = '#64748b';
            
            const labels = """ + json.dumps(labels) + """;
            
            new Chart(document.getElementById('rssChart'), {
                type: 'bar',
                data: {
                    labels: labels,
                    datasets: [
                        { label: 'C Baseline', data: """ + json.dumps(rss_c) + """, backgroundColor: '#94a3b8', borderRadius: 4 },
                        { label: 'C++ Wrapper', data: """ + json.dumps(rss_cpp) + """, backgroundColor: '#2563eb', borderRadius: 4 }
                    ]
                },
                options: { 
                    responsive: true, 
                    maintainAspectRatio: false, 
                    plugins: { 
                        title: { display: true, text: 'Peak Resident Set Size (RSS in KB)', padding: 20, font: { size: 16 } },
                        legend: { position: 'bottom' }
                    },
                    scales: {
                        y: { beginAtZero: true, grid: { color: '#f1f5f9' }, title: { display: true, text: 'KB' } },
                        x: { grid: { display: false } }
                    }
                }
            });

            new Chart(document.getElementById('timeChart'), {
                type: 'bar',
                data: {
                    labels: labels,
                    datasets: [
                        { label: 'C Baseline', data: """ + json.dumps(time_c) + """, backgroundColor: '#94a3b8', borderRadius: 4 },
                        { label: 'C++ Wrapper', data: """ + json.dumps(time_cpp) + """, backgroundColor: '#ef4444', borderRadius: 4 }
                    ]
                },
                options: { 
                    responsive: true, 
                    maintainAspectRatio: false, 
                    plugins: { 
                        title: { display: true, text: 'Execution Time (ms)', padding: 20, font: { size: 16 } },
                        legend: { position: 'bottom' }
                    },
                    scales: {
                        y: { beginAtZero: true, grid: { color: '#f1f5f9' }, title: { display: true, text: 'ms' } },
                        x: { grid: { display: false } }
                    }
                }
            });
            
             new Chart(document.getElementById('heapChart'), {
                type: 'bar',
                data: {
                    labels: labels,
                    datasets: [
                        { label: 'C Baseline', data: """ + json.dumps(heap_c) + """, backgroundColor: '#94a3b8', borderRadius: 4 },
                        { label: 'C++ Wrapper', data: """ + json.dumps(heap_cpp) + """, backgroundColor: '#10b981', borderRadius: 4 }
                    ]
                },
                options: { 
                    responsive: true, 
                    maintainAspectRatio: false, 
                    plugins: { 
                        title: { display: true, text: 'Heap Usage (Bytes)', padding: 20, font: { size: 16 } },
                        legend: { position: 'bottom' }
                    },
                    scales: {
                        y: { beginAtZero: true, grid: { color: '#f1f5f9' }, title: { display: true, text: 'Bytes' } },
                        x: { grid: { display: false } }
                    }
                }
            });
        </script>
    </body>
    </html>
    """
    
    with open(filename, "w") as f:
        f.write(html_content)
    print(f"\n[+] Report generated: {os.path.abspath(filename)}")

def print_console_summary(results):
    print("\n" + "="*80)
    print(f"{'BENCHMARK':<20} | {'LANG':<6} | {'TIME (ms)':<10} | {'RSS (KB)':<10} | {'HEAP (B)':<10} | {'STATUS':<10}")
    print("-" * 80)
    
    # Sort results for consistent display
    sorted_results = sorted(results, key=lambda x: x.name)
    
    for res in sorted_results:
        # Simplify name
        name = res.name.replace("bench_", "").replace("_c", " (C)").replace("_cpp", "")
        # Adjust name if it doesn't match C pattern
        if res.lang == "cpp" and "(C)" not in name:
            name = f"{name} (C++)"
        elif res.lang == "c":
             name = f"{name}" # Keep (C) suffix
        
        # Nicer status
        status = res.status
        if status == "SUCCESS": status = "PASS"
        
        print(f"{name:<20} | {res.lang.upper():<6} | {res.time_ms:<10.2f} | {res.rss_kb:<10} | {res.heap_bytes:<10} | {status:<10}")
    print("="*80 + "\n")

def main():
    if not os.path.exists(BUILD_DIR):
        print(f"Build directory '{BUILD_DIR}' not found. Please build first.")
        sys.exit(1)

    print("=== LVGL Memory Profile Runner (Extended) ===")
    
    # Check for pprof
    has_pprof = False
    try:
        run_cmd("which pprof", capture_output=False)
        has_pprof = True
        print("[+] pprof detected.")
    except:
        print("[-] pprof not found.")

    all_results = []

    for c_bench, cpp_bench in BENCHMARKS:
        print(f"\n>>> Processing: {c_bench} vs {cpp_bench}")
        
        # Cleanup old profiles
        if has_pprof:
            # os.system(f"rm -f /tmp/{c_bench}* /tmp/{cpp_bench}*")
            # Safer to just let them accumulate and pick the latest, but cleaning ensures fresh data
            subprocess.run(f"rm -f /tmp/{c_bench}* /tmp/{cpp_bench}*", shell=True)

        # Run C
        res_c = BenchmarkResult(c_bench, "c")
        env_c = {}
        if has_pprof: env_c["HEAPPROFILE"] = f"/tmp/{c_bench}"
        
        print(f"  Running {c_bench}...", end=" ", flush=True)
        success_c, out_c, err_c = run_cmd(f"./{BUILD_DIR}/{c_bench}", env=env_c)
        
        if success_c:
            res_c.status = "SUCCESS"
            print("DONE")
            parse_metrics(out_c, res_c)
            if has_pprof:
                heap_file = get_heap_file(c_bench)
                if heap_file:
                    res_c.heap_bytes = get_pprof_heap_bytes(f"{BUILD_DIR}/{c_bench}", heap_file)
        else:
            res_c.status = "CRASH"
            res_c.error_msg = err_c
            print("FAIL")
            print(err_c)

        all_results.append(res_c)

        # Run C++
        res_cpp = BenchmarkResult(cpp_bench, "cpp")
        env_cpp = {}
        if has_pprof: env_cpp["HEAPPROFILE"] = f"/tmp/{cpp_bench}"
        
        print(f"  Running {cpp_bench}...", end=" ", flush=True)
        success_cpp, out_cpp, err_cpp = run_cmd(f"./{BUILD_DIR}/{cpp_bench}", env=env_cpp)
        
        if success_cpp:
            res_cpp.status = "SUCCESS"
            print("DONE")
            parse_metrics(out_cpp, res_cpp)
            if has_pprof:
                heap_file = get_heap_file(cpp_bench)
                if heap_file:
                    res_cpp.heap_bytes = get_pprof_heap_bytes(f"{BUILD_DIR}/{cpp_bench}", heap_file)
        else:
            res_cpp.status = "CRASH"
            res_cpp.error_msg = err_cpp
            print("FAIL")
            print(err_cpp)
            
        all_results.append(res_cpp)

    generate_html_report(all_results)
    print_console_summary(all_results)
    
    # Check for critical failures
    crashes = [r for r in all_results if r.status == "CRASH"]
    if crashes:
        print(f"\n[!] WARNING: {len(crashes)} benchmarks crashed.")
        sys.exit(1)

if __name__ == "__main__":
    main()
