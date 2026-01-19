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

    html_content = f"""
    <!DOCTYPE html>
    <html>
    <head>
        <title>LVGL C++ Binding Memory Profile</title>
        <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
        <style>
            body {{ font-family: 'Inter', sans-serif; background: #f4f6f8; margin: 0; padding: 20px; }}
            .container {{ max_width: 1000px; margin: 0 auto; background: white; padding: 30px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.05); }}
            h1 {{ text-align: center; color: #333; }}
            .chart-container {{ position: relative; height: 300px; margin-bottom: 50px; }}
            table {{ width: 100%; border-collapse: collapse; margin-top: 30px; }}
            th, td {{ padding: 12px; border-bottom: 1px solid #ddd; text-align: left; }}
            th {{ background-color: #f8f9fa; }}
            .status-pass {{ color: green; font-weight: bold; }}
            .status-fail {{ color: red; font-weight: bold; }}
        </style>
    </head>
    <body>
        <div class="container">
            <h1>LVGL C++ Wrapper Memory Overhead</h1>
            <p style="text-align: center; color: #666;">Generated on {datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")}</p>
            
            <div class="chart-container">
                <canvas id="rssChart"></canvas>
            </div>
            
            <div class="chart-container">
                <canvas id="timeChart"></canvas>
            </div>
            
             <div class="chart-container">
                <canvas id="heapChart"></canvas>
            </div>
            
            <h2>Detailed Results</h2>
            <table>
                <thead>
                    <tr>
                        <th>Benchmark</th>
                        <th>Lang</th>
                        <th>Time (ms)</th>
                        <th>RSS (KB)</th>
                        <th>Heap (Bytes)</th>
                        <th>Status</th>
                    </tr>
                </thead>
                <tbody>
    """
    
    for key in sorted_keys:
        pair = bench_data[key]
        for lang in ['c', 'cpp']:
            res = pair.get(lang)
            if res:
                status_class = "status-fail" if res.status != "SUCCESS" else "status-pass"
                html_content += f"""
                    <tr>
                        <td>{key.capitalize()}</td>
                        <td>{res.lang.upper()}</td>
                        <td>{res.time_ms:.2f}</td>
                        <td>{res.rss_kb}</td>
                        <td>{res.heap_bytes}</td>
                        <td class="{status_class}">{res.status}</td>
                    </tr>
                """
    
    html_content += """
                </tbody>
            </table>
        </div>
        
        <script>
            const labels = """ + json.dumps(labels) + """;
            
            new Chart(document.getElementById('rssChart'), {
                type: 'bar',
                data: {
                    labels: labels,
                    datasets: [
                        { label: 'C RSS (KB)', data: """ + json.dumps(rss_c) + """, backgroundColor: '#94a3b8' },
                        { label: 'C++ RSS (KB)', data: """ + json.dumps(rss_cpp) + """, backgroundColor: '#3b82f6' }
                    ]
                },
                options: { responsive: true, maintainAspectRatio: false, plugins: { title: { display: true, text: 'Peak Resident Set Size (RSS)' } } }
            });

            new Chart(document.getElementById('timeChart'), {
                type: 'bar',
                data: {
                    labels: labels,
                    datasets: [
                        { label: 'C Time (ms)', data: """ + json.dumps(time_c) + """, backgroundColor: '#94a3b8' },
                        { label: 'C++ Time (ms)', data: """ + json.dumps(time_cpp) + """, backgroundColor: '#ef4444' }
                    ]
                },
                options: { responsive: true, maintainAspectRatio: false, plugins: { title: { display: true, text: 'Execution Time' } } }
            });
            
             new Chart(document.getElementById('heapChart'), {
                type: 'bar',
                data: {
                    labels: labels,
                    datasets: [
                        { label: 'C Heap (Bytes)', data: """ + json.dumps(heap_c) + """, backgroundColor: '#94a3b8' },
                        { label: 'C++ Heap (Bytes)', data: """ + json.dumps(heap_cpp) + """, backgroundColor: '#10b981' }
                    ]
                },
                options: { responsive: true, maintainAspectRatio: false, plugins: { title: { display: true, text: 'Heap Usage (pprof)' } } }
            });
        </script>
    </body>
    </html>
    """
    
    with open(filename, "w") as f:
        f.write(html_content)
    print(f"\n[+] Report generated: {os.path.abspath(filename)}")

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
    
    # Check for critical failures
    crashes = [r for r in all_results if r.status == "CRASH"]
    if crashes:
        print(f"\n[!] WARNING: {len(crashes)} benchmarks crashed.")
        sys.exit(1)

if __name__ == "__main__":
    main()
