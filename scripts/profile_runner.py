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
    ("bench_widgets_c slider", "bench_widgets slider"),
    ("bench_widgets_c switch", "bench_widgets switch"),
    ("bench_widgets_c checkbox", "bench_widgets checkbox"),
    ("bench_widgets_c textarea", "bench_widgets textarea"),
    ("bench_widgets_c arc", "bench_widgets arc"),
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

    heap_graph_svg = None


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

def get_pprof_heap_bytes(pprof_bin, binary_path, heap_file):
    if not heap_file or not os.path.exists(heap_file):
        return 0
    
    # Run pprof to get text output showing total memory
    # pprof --text binary heap_file
    # Output first line usually: "Total: 123.4MB"
    # Or parsing the top line summary.
    
    # Better approach: "pprof --bytes --text ..." to get bytes directly ?
    # pprof doesn't always support --bytes in all versions.
    # Standard: "Total: 10.5 MB"
    
    success, out, err = run_cmd(f"{pprof_bin} --text --show_bytes {binary_path} {heap_file} | head -n 1")

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

def generate_heap_graph(pprof_bin, binary_path, heap_file):
    # Generates SVG call graph and returns it as a string
    # Try pprof --svg
    success, out, err = run_cmd(f"{pprof_bin} --svg --show_bytes {binary_path} {heap_file}")
    if success and "<svg" in out:
        # Pprof outputs 100% width/height which creates tiny graphs in modals.
        # We need to extract the actual bounding box and set explicit dimensions to allow scrolling.
        import re
        
        # 1. Parse background polygon to get dimensions from the FIRST polygon (background)
        # It usually looks like: <polygon fill="white" ... points="-4,4 -4,-1430 ..."/>
        poly_match = re.search(r'<polygon [^>]*points="([^"]+)"', out)
        if poly_match:
            try:
                pts = poly_match.group(1).strip().split()
                xs = []
                ys = []
                for p in pts:
                    if ',' in p:
                        x, y = map(float, p.split(','))
                        xs.append(x)
                        ys.append(y)
                
                if xs and ys:
                    width = max(xs) - min(xs)
                    height = max(ys) - min(ys)
                    
                    # 2. Replace 100% with calculated dimensions (in pt)
                    out = re.sub(r'width="[^"]+"', f'width="{width}pt"', out, count=1)
                    out = re.sub(r'height="[^"]+"', f'height="{height}pt"', out, count=1)
            except Exception as e:
                print(f"[!] Clean SVG failed: {e}")
                # Fallback: keep original or strip? If we fail, original 100% is better than broken.
                pass
        
        return out
    return None




def render_graph_modal(res, idx):
    if not res or not res.name: return "" 
    if getattr(res, 'heap_graph_svg', None) is None: return ""
    
    modal_id = f"modal_{idx.replace(' ', '_')}"
    return f"""
    <div style="margin-top:10px;">
        <button onclick="document.getElementById('{modal_id}').style.display='block'" class="badge badge-pass" style="cursor:pointer; border:none; background:#3b82f6; color:white;">View Heap Graph</button>
        <div id="{modal_id}" style="display:none; position:fixed; top:0; left:0; width:100%; height:100%; background:rgba(0,0,0,0.8); z-index:1000;">
            <div style="background:white; margin:2% auto; width:95%; height:95%; padding:20px; border-radius:8px; position:relative; display:flex; flex-direction:column;">
                <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:10px;">
                    <h3 style="margin:0;">Heap Call Graph: {res.name}</h3>
                    <span onclick="document.getElementById('{modal_id}').style.display='none'" style="cursor:pointer; font-size:24px; font-weight:bold;">&times;</span>
                </div>
                <div style="flex-grow:1; overflow:auto; border:1px solid #ccc; background:#f8fafc; text-align:center;">
                    {res.heap_graph_svg}
                </div>
                <p style="margin-top:10px; font-size:0.85rem; color:#64748b;">
                    Nodes show allocated objects. Edges show references/allocating functions. Larger nodes = more memory.
                </p>
            </div>
        </div>
    </div>
    """
def run_trend_analysis():
    print("\n>>> Running Trend Analysis (Linearity check)...")

    counts = list(range(1, 17)) + list(range(18, 33, 2))
    trend_data = {
        "counts": counts,
        "c_rss": [],
        "cpp_rss": [],
        "overhead_per_obj": []
    }
    
    # Pre-warm or cleanup
    # Safer to just use subprocess directly as run_cmd is defined in this file
    subprocess.run("rm -f /tmp/bench_baseline*", shell=True)

    for n in counts:
        # Run C
        res_c = BenchmarkResult(f"baseline_c_{n}", "c")
        env = {
            "HEAPPROFILE": f"/tmp/bench_baseline_c_{n}",
            "TCMALLOC_SAMPLE_PARAMETER": "1"
        }

        success, out, _ = run_cmd(f"./{BUILD_DIR}/bench_baseline_c {n}", env=env)
        if success:
            parse_metrics(out, res_c)
            trend_data["c_rss"].append(res_c.rss_kb)
        else:
            trend_data["c_rss"].append(0)

        # Run C++
        res_cpp = BenchmarkResult(f"baseline_cpp_{n}", "cpp")
        env = {
            "HEAPPROFILE": f"/tmp/bench_baseline_cpp_{n}",
            "TCMALLOC_SAMPLE_PARAMETER": "1"
        }

        success, out, _ = run_cmd(f"./{BUILD_DIR}/bench_baseline {n}", env=env)
        if success:
            parse_metrics(out, res_cpp)
            trend_data["cpp_rss"].append(res_cpp.rss_kb)
        else:
            trend_data["cpp_rss"].append(0)
            
        # Calculate overhead
        if trend_data["c_rss"][-1] > 0 and trend_data["cpp_rss"][-1] > 0:
            diff = (trend_data["cpp_rss"][-1] - trend_data["c_rss"][-1]) * 1024 # bytes
            trend_data["overhead_per_obj"].append(diff / n)
        else:
           trend_data["overhead_per_obj"].append(0)
           
        print(f"  N={n}: C={trend_data['c_rss'][-1]}KB CPP={trend_data['cpp_rss'][-1]}KB")

    return trend_data

def run_stability_test():
    print("\n>>> Running Stability Test (Issue #80)...")
    
    # Run benchmark and capture output live or post-process
    # Since it might take time, just capture standard output
    env = {} 
    # Use different heap profile? Or none, just RSS.
    # HEAPPROFILE might be too heavy for 100 iterations depending on sampling.
    # We rely on RSS reported by the benchmark itself.
    
    success, out, err = run_cmd(f"./{BUILD_DIR}/bench_churn_stability", env=env)
    
    stability_data = {
        "iters": [],
        "rss": []
    }

    if success:
        for line in out.splitlines():
            if "METRIC_STABILITY:" in line:
                # METRIC_STABILITY: ITER=50 RSS=1234
                try:
                    parts = line.split(":")
                    kv_parts = parts[1].strip().split()
                    iter_val = int(kv_parts[0].split("=")[1])
                    rss_val = int(kv_parts[1].split("=")[1])
                    
                    stability_data["iters"].append(iter_val)
                    stability_data["rss"].append(rss_val)
                except:
                    pass
    
    # Calculate simple slope or net change
    start_rss = stability_data["rss"][0] if stability_data["rss"] else 0
    end_rss = stability_data["rss"][-1] if stability_data["rss"] else 0
    stability_data["delta_kb"] = end_rss - start_rss
    print(f"  Stability Result: Start={start_rss}KB End={end_rss}KB Delta={stability_data['delta_kb']}KB")

    return stability_data

def generate_html_report(results, trend_data=None, stability_data=None, filename="memory_report.html"):
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
        # For widget benchmarks like "bench_widgets_c_slider", we need robust parsing
        # But we replaced " " with "_" in the loop
        
        # This parsing logic is a bit brittle if names contain multiple underscores.
        # But our names maximize 2 segments or 3.
        # bench_widgets_c_slider -> key=widgets_slider?
        
        # Current logic:
        # if "_c" in res.name: key = remove "bench_" remove "_c"
        # "bench_widgets_c_slider" -> "widgets_slider"
        
        if "_c" in res.name:
            key = res.name.replace("bench_", "").replace("_c", "").replace("__", "_")
            if key not in bench_data: bench_data[key] = {}
            bench_data[key]["c"] = res
        else:
            key = res.name.replace("bench_", "").replace("_cpp", "").replace("__", "_")
            # For C++, "bench_widgets_cpp_slider" (no, it was "bench_widgets_slider" if using cpp_name)
            
            # Wait, in the loop: 
            # c_name = "bench_widgets_c_slider" (bench_widgets_c slider -> replace space)
            # cpp_name = "bench_widgets_slider" (bench_widgets slider -> replace space)
            
            # So key extraction:
            # c: "widgets_slider"
            # cpp: "widgets_slider"
            
            if key not in bench_data: bench_data[key] = {}
            bench_data[key]["cpp"] = res

    # Sort keys for consistent order
    sorted_keys = sorted(bench_data.keys())
    
    for key in sorted_keys:
        labels.append(key.capitalize().replace("_", " "))
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
        "Baseline": "<b>Objective:</b> Measure the fixed overhead of the `lvgl::Object` C++ wrapper.<br><b>Methodology:</b> Creates 50 generic objects (Buttons) parented to the active screen. Compares raw C `lv_button_create` vs C++ `std::make_unique<lvgl::Button>`.<br><b>Insight:</b> Demonstrates the cost of the C++ smart pointer and wrapper allocation.",
        "Events": "<b>Objective:</b> Measure the overhead of the C++ event dispatch system.<br><b>Methodology:</b> Registers a C++ `std::function` callback vs a raw C function pointer. Triggers `LV_EVENT_CLICKED` 50 times.<br><b>Insight:</b> Validates the efficiency of `lvgl::Object::add_event_cb` and the proxy mechanism.",
        "Churn": "<b>Objective:</b> Stress-test memory stability during rapid creation/destruction.<br><b>Methodology:</b> Creates and destroys a Screen filled with 20 buttons, repeated 100 times.<br><b>Insight:</b> Detects memory leaks or fragmentation issues in the wrapper's lifecycle management.",
        "Fragmentation": "<b>Objective:</b> Simulate real-world usage with mixed allocations.<br><b>Methodology:</b> Creates objects with varying lifecycles and captures (lambdas), forcing non-monotonic heap usage.<br><b>Insight:</b> Checks for heap fragmentation that might not appear in simple linear benchmarks.",
        "Widgets slider": "<b>Widget:</b> Slider<br><b>Complexity:</b> Low (Bar + Knob)<br><b>Test:</b> Creation overhead of `lvgl::Slider`.",
        "Widgets switch": "<b>Widget:</b> Switch<br><b>Complexity:</b> Low (Rect + Circle)<br><b>Test:</b> Creation overhead of `lvgl::Switch`.",
        "Widgets checkbox": "<b>Widget:</b> Checkbox<br><b>Complexity:</b> Medium (Label + Indicator)<br><b>Test:</b> Creation overhead of `lvgl::Checkbox`.",
        "Widgets textarea": "<b>Widget:</b> Textarea<br><b>Complexity:</b> High (Label + Cursor + Background + Scroll)<br><b>Test:</b> Creation overhead of `lvgl::Textarea` (includes font rendering).",
        "Widgets arc": "<b>Widget:</b> Arc<br><b>Complexity:</b> Medium (Arc drawing)<br><b>Test:</b> Creation overhead of `lvgl::Arc`."
    }

    html_content = f"""
    <!DOCTYPE html>
    <html>
    <head>
        <title>LVGL C++ Binding Memory Profile</title>
        <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/svg-pan-zoom@3.6.1/dist/svg-pan-zoom.min.js"></script>
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
            
            <!-- Executive Summary -->
            <div class="card" style="margin-bottom: 40px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white;">
                <h2 style="margin-top:0; color: white;">Executive Summary</h2>
                <div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px;">
                    <div style="text-align: center;">
                        <div style="font-size: 2.5rem; font-weight: bold;">{len(results)}</div>
                        <div style="opacity: 0.8;">Benchmarks Run</div>
                    </div>
                    <div style="text-align: center;">
                        <div style="font-size: 2.5rem; font-weight: bold;">{sum(1 for r in results if r.status == 'SUCCESS')}/{len(results)}</div>
                        <div style="opacity: 0.8;">Passed</div>
                    </div>
                    <div style="text-align: center;">
                        <div style="font-size: 2.5rem; font-weight: bold;">{f'{sum(trend_data["overhead_per_obj"])/len(trend_data["overhead_per_obj"])/1024:.2f} KB' if trend_data and trend_data.get('overhead_per_obj') else 'N/A'}</div>
                        <div style="opacity: 0.8;">Avg Per-Object Overhead</div>
                    </div>
                    <div style="text-align: center;">
                        <div style="font-size: 2.5rem; font-weight: bold;">{f'{stability_data["delta_kb"]} KB' if stability_data else 'N/A'}</div>
                        <div style="opacity: 0.8;">Stability Delta (1k iterations)</div>
                    </div>
                </div>
            </div>
            
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
        
        # Friendly lookup description
        lookup_key = key.capitalize().replace("_", " ")
        # "Widgets slider" matches
        
        desc = bench_descriptions.get(lookup_key, "Benchmark measurements.")
        
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
                        <h2>{key.replace("_", " ").title()}</h2>
                    </div>
                    <div class="benchmark-content">
                        <div class="benchmark-desc">
                            {desc}
                        </div>
                        <div class="benchmark-stats">
                            <div class="stat-wrapper">
                                {render_stat_box(c_res, "C Baseline")}
                            </div>
                            <div class="stat-wrapper">
                                {render_stat_box(cpp_res, "C++ Wrapper", "cpp")}
                                {render_graph_modal(cpp_res, key)}
                            </div>
                        </div>
                    </div>
                </div>
        """
    
    # Add Trend Section
    if trend_data:
        html_content += """
        <div class="card" style="margin-top: 40px;">
            <h2 style="margin-top:0;">Memory Growth Trend (Issue #77)</h2>
            <div class="chart-container">
                <canvas id="trendChart"></canvas>
            </div>
            <p style="text-align:center; color:#64748b; margin-top:10px;">
                Measures RSS growth as object count increases from 1 to 32. 
                Slope indicates per-object overhead.
            </p>
        </div>
        """

    # Add Stability Section
    if stability_data and len(stability_data["iters"]) > 0:
        html_content += """
        <div class="card" style="margin-top: 40px;">
            <h2 style="margin-top:0;">Long-running Stability (Issue #80)</h2>
            <div class="chart-container">
                <canvas id="stabilityChart"></canvas>
            </div>
            <p style="text-align:center; color:#64748b; margin-top:10px;">
                RSS Usage over 1000 iterations of screen churn. Flat line indicates no leaks.
            </p>
        </div>
        """

    html_content += """
            </div>
        </div>
        <script>
    
            Chart.defaults.font.family = "'Inter', sans-serif";
            Chart.defaults.color = '#64748b';
            
            Chart.defaults.font.family = "'Inter', sans-serif";
            Chart.defaults.color = '#64748b';
            
            const labels = """ + json.dumps(labels) + """;
            
            // Trend Data
            const trendCounts = """ + (json.dumps(trend_data["counts"]) if trend_data else "[]") + """;
            const trendCRss = """ + (json.dumps(trend_data["c_rss"]) if trend_data else "[]") + """;
            const trendCppRss = """ + (json.dumps(trend_data["cpp_rss"]) if trend_data else "[]") + """;
            
            // Stability Data
            const stableIters = """ + (json.dumps(stability_data["iters"]) if stability_data else "[]") + """;
            const stableRss = """ + (json.dumps(stability_data["rss"]) if stability_data else "[]") + """;

            if (stableIters.length > 0) {
                 new Chart(document.getElementById('stabilityChart'), {
                    type: 'line',
                    data: {
                        labels: stableIters,
                        datasets: [
                            { label: 'RSS (KB)', data: stableRss, borderColor: '#16a34a', backgroundColor: '#16a34a', tension: 0.1, fill: false }
                        ]
                    },
                    options: {
                        responsive: true,
                        maintainAspectRatio: false,
                        plugins: {
                            title: { display: true, text: 'Stability Check (RSS vs Iterations)' },
                            tooltip: { mode: 'index', intersect: false }
                        },
                        scales: {
                            y: { title: { display: true, text: 'KB' } },
                            x: { title: { display: true, text: 'Iteration' } }
                        }
                    }
                });
            }

            if (trendCounts.length > 0) {
                new Chart(document.getElementById('trendChart'), {
                    type: 'line',
                    data: {
                        labels: trendCounts,
                        datasets: [
                            { label: 'C Baseline RSS', data: trendCRss, borderColor: '#94a3b8', backgroundColor: '#94a3b8', tension: 0.1 },
                            { label: 'C++ Wrapper RSS', data: trendCppRss, borderColor: '#2563eb', backgroundColor: '#2563eb', tension: 0.1 }
                        ]
                    },
                    options: {
                        responsive: true,
                        maintainAspectRatio: false,
                        plugins: {
                            title: { display: true, text: 'Memory Growth (RSS vs Object Count)' },
                            tooltip: { mode: 'index', intersect: false }
                        },
                        scales: {
                            y: { title: { display: true, text: 'KB' } },
                            x: { title: { display: true, text: 'Object Count (N)' } }
                        }
                    }
                });
            }
            
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
            
            // Initialize svg-pan-zoom on modal SVGs when they become visible
            document.querySelectorAll('[id^="modal_"]').forEach(modal => {
                modal.addEventListener('click', function(e) {
                    if (e.target === this) {
                        this.style.display = 'none';
                    }
                });
                const svgEl = modal.querySelector('svg');
                if (svgEl && !svgEl.dataset.panZoomInit) {
                    svgEl.dataset.panZoomInit = 'true';
                    svgEl.style.width = '100%';
                    svgEl.style.height = '100%';
                    try {
                        svgPanZoom(svgEl, {
                            zoomEnabled: true,
                            controlIconsEnabled: true,
                            fit: true,
                            center: true
                        });
                    } catch(e) { console.warn('svg-pan-zoom init failed:', e); }
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
    pprof_bin = "pprof"
    has_pprof = False
    if subprocess.call("which pprof > /dev/null 2>&1", shell=True) == 0:
        has_pprof = True
    elif subprocess.call("which google-pprof > /dev/null 2>&1", shell=True) == 0:
        has_pprof = True
        pprof_bin = "google-pprof"
        
    if has_pprof:
        print(f"[+] pprof detected: {pprof_bin}")
    else:
        print("[-] pprof not found.")

    all_results = []

    for c_bench_str, cpp_bench_str in BENCHMARKS:
        print(f"\n>>> Processing: {c_bench_str} vs {cpp_bench_str}")
        
        # Parse binary and args for C
        c_parts = c_bench_str.split()
        c_bin = c_parts[0]
        c_args = " ".join(c_parts[1:])
        c_name = c_bench_str.replace(" ", "_")
        
        # Parse binary and args for C++
        cpp_parts = cpp_bench_str.split()
        cpp_bin = cpp_parts[0]
        cpp_args = " ".join(cpp_parts[1:])
        cpp_name = cpp_bench_str.replace(" ", "_")

        # Cleanup old profiles
        if has_pprof:
            subprocess.run(f"rm -f /tmp/{c_name}* /tmp/{cpp_name}*", shell=True)

        # Run C
        res_c = BenchmarkResult(c_name, "c")
        env_c = {}
        if has_pprof: 
            env_c["HEAPPROFILE"] = f"/tmp/{c_name}"
            env_c["TCMALLOC_SAMPLE_PARAMETER"] = "1"

        
        print(f"  Running {c_bench_str}...", end=" ", flush=True)
        success_c, out_c, err_c = run_cmd(f"./{BUILD_DIR}/{c_bin} {c_args}", env=env_c)
        
        if success_c:
            res_c.status = "SUCCESS"
            print("DONE")
            parse_metrics(out_c, res_c)
            if has_pprof:
                heap_file = get_heap_file(c_name)
                if heap_file:
                    res_c.heap_bytes = get_pprof_heap_bytes(pprof_bin, f"{BUILD_DIR}/{c_bin}", heap_file)

        else:
            res_c.status = "CRASH"
            res_c.error_msg = err_c
            print("FAIL")
            print(err_c)
            
        all_results.append(res_c)

        # Run C++
        res_cpp = BenchmarkResult(cpp_name, "cpp")
        env_cpp = {}
        if has_pprof: 
            env_cpp["HEAPPROFILE"] = f"/tmp/{cpp_name}"
            env_cpp["TCMALLOC_SAMPLE_PARAMETER"] = "1"

        
        print(f"  Running {cpp_bench_str}...", end=" ", flush=True)
        success_cpp, out_cpp, err_cpp = run_cmd(f"./{BUILD_DIR}/{cpp_bin} {cpp_args}", env=env_cpp)
        
        if success_cpp:
            res_cpp.status = "SUCCESS"
            print("DONE")
            parse_metrics(out_cpp, res_cpp)
            if has_pprof:
                heap_file = get_heap_file(cpp_name)
                if heap_file:
                    res_cpp.heap_bytes = get_pprof_heap_bytes(pprof_bin, f"{BUILD_DIR}/{cpp_bin}", heap_file)

                    res_cpp.heap_graph_svg = generate_heap_graph(pprof_bin, f"{BUILD_DIR}/{cpp_bin}", heap_file)

        else:
            res_cpp.status = "CRASH"
            res_cpp.error_msg = err_cpp
            print("FAIL")
            print(err_cpp)
            
        all_results.append(res_cpp)

    # Run Trend Analysis
    trend_data = run_trend_analysis()
    
    # Run Stability Test
    stability_data = run_stability_test()

    generate_html_report(all_results, trend_data, stability_data)
    print_console_summary(all_results)
    
    # Check for critical failures
    crashes = [r for r in all_results if r.status == "CRASH"]
    if crashes:
        print(f"\n[!] WARNING: {len(crashes)} benchmarks crashed.")
        sys.exit(1)

if __name__ == "__main__":
    main()
