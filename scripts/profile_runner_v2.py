#!/usr/bin/env python3
"""
profile_runner_v2.py

High-Fidelity Reporting Engine (v2.1)
Author: Lead Systems Architect

Features:
- Discovery: Finds all C++ benchmarks via ./bench_suite --list
- Baseline Comparison: Runs correlating C benchmarks if available (e.g. Widgets_Slider -> bench_widgets_c slider)
- Data aggregation: Combines metrics into unified JSON
- Gold Standard Visualization: Generates an interactive Report dashboard

Usage:
    python3 scripts/profile_runner_v2.py
"""

import subprocess
import json
import os
import sys
import datetime
import re

BENCH_BINARY_CPP = "./build/bench_suite"
BENCH_BINARY_C = "./build/bench_widgets_c"
REPORT_FILE = "bench_report_v2.html"

# Mapping between C++ Benchmark Names and C Benchmark arguments
# Key: C++ Name, Value: C Binary Argument
BASELINE_MAP = {
    "Widgets_Slider": "slider",
    "Widgets_Arc": "arc",
    "Widgets_Switch": "switch",
    "Widgets_Checkbox": "checkbox",
    "Widgets_Table": "table", # Not in C bench yet?
    "Widgets_Chart": "chart", # Not in C bench yet?
}

def get_benchmarks():
    """Run --list to get available benchmarks."""
    cmd = [BENCH_BINARY_CPP, "--list"]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        output = result.stdout.strip()
        start = output.find('[')
        end = output.rfind(']') + 1
        return json.loads(output[start:end])
    except Exception as e:
        print(f"Error listing benchmarks: {e}")
        return []

def run_benchmark_cpp(name):
    """Run a C++ benchmark and return its JSON result."""
    
    # Enable pprof profiling if not on embedded
    env = os.environ.copy()
    prof_file = f"build/bench_{name}"
    env["HEAPPROFILE"] = prof_file # Generates bench_Name.0001.heap
    # env["CPUPROFILE"] = prof_file + ".prof" # Optional CPU profiling
    
    cmd = [BENCH_BINARY_CPP, f"--run={name}"]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True, env=env)
        output = result.stdout.strip()
        start = output.find('{')
        end = output.rfind('}') + 1
        return json.loads(output[start:end]), prof_file
    except Exception as e:
        print(f"Error running C++ {name}: {e}")
        return None, None

def run_benchmark_c(arg):
    """Run a C benchmark and return parsed metrics."""
    if not os.path.exists(BENCH_BINARY_C):
        return None
        
    cmd = [BENCH_BINARY_C, arg]
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        # Parse ad-hoc output: "BENCHMARK_METRIC: TIME=5.76 unit=ms"
        # "BENCHMARK_METRIC: RSS=4616 unit=kb"
        metrics = {"time_ns": 0, "rss_kb": 0, "heap_bytes": 0}
        
        for line in result.stdout.splitlines():
            if "BENCHMARK_METRIC: TIME=" in line:
                val = re.search(r"TIME=([\d\.]+)", line).group(1)
                metrics["time_ns"] = float(val) * 1000000 # Convert ms to ns for consistency
            if "BENCHMARK_METRIC: RSS=" in line:
                val = re.search(r"RSS=(\d+)", line).group(1)
                metrics["rss_kb"] = int(val)
                
        return {"metrics": metrics}
    except Exception as e:
        print(f"Error running C {arg}: {e}")
        return None

def get_flame_data(prefix):
    """Run pprof --collapsed and return nested JSON for d3-flame-graph."""
    if not prefix: return None
    try:
        files = [f for f in os.listdir(os.path.dirname(prefix)) if f.startswith(os.path.basename(prefix)) and ".heap" in f]
        if not files: return None
        latest_file = sorted(files)[-1]
        full_path = os.path.join(os.path.dirname(prefix), latest_file)
        
        cmd = ["google-pprof", "--collapsed", BENCH_BINARY_CPP, full_path]
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        
        root = {"name": "root", "value": 0, "children": []}
        
        for line in result.stdout.splitlines():
            if not line: continue
            parts = line.rsplit(" ", 1)
            if len(parts) != 2: continue
            stack, val = parts[0], int(parts[1])
            frames = stack.split(";")
            
            curr = root
            root["value"] += val
            for f in frames:
                found = None
                for child in curr["children"]:
                    if child["name"] == f:
                        found = child
                        break
                if found:
                    found["value"] += val
                    curr = found
                else:
                    new_node = {"name": f, "value": val, "children": []}
                    curr["children"].append(new_node)
                    curr = new_node
        return root
    except Exception as e:
        print(f"    Warning: Could not generate flame data: {e}")
        return None

def get_svg_file(prefix, name):
    """Find the latest heap file, run pprof, write SVG to file, return filename."""
    if not prefix: return None
    
    # Find files like bench_Name.0001.heap
    try:
        files = [f for f in os.listdir(os.path.dirname(prefix)) if f.startswith(os.path.basename(prefix)) and ".heap" in f]
        if not files: return None
        
        latest_file = sorted(files)[-1]
        full_path = os.path.join(os.path.dirname(prefix), latest_file)
        
        # Output SVG file path
        svg_filename = f"build/profile_{name}.svg"
        
        # Run pprof to get SVG
        cmd = ["google-pprof", "--svg", BENCH_BINARY_CPP, full_path]
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        
        # Write SVG to file
        with open(svg_filename, "w") as f:
            f.write(result.stdout)
        
        return svg_filename
        
    except Exception as e:
        print(f"    Warning: Could not generate SVG for {name}: {e}")
        return None

def generate_report(results):
    """Generate the Gold Standard HTML Report - Replicating memory_report.html visuals."""
    
    total = len(results)
    chart_labels = []
    chart_time_c = []
    chart_time_cpp = []
    chart_heap_delta = []
    
    benchmark_cards = []
    total_heap_delta = 0
    total_time_c = 0
    total_time_cpp = 0
    flame_data_all = {}
    
    for r in results:
        name = r["name"]
        cpp_m = r["cpp"].get("metrics", {})
        c_m = r["c"].get("metrics", {}) if r["c"] else None
        
        time_c = c_m.get("time_ns", 0) / 1e6 if c_m else 0
        time_cpp = cpp_m.get("time_ns", 0) / 1e6
        heap_delta = cpp_m.get("heap_delta_bytes", 0)
        rss_delta = cpp_m.get("rss_delta_kb", 0)
        rss_c = c_m.get("rss_kb", 0) if c_m else 0
        
        total_heap_delta += heap_delta
        total_time_c += time_c
        total_time_cpp += time_cpp
        
        chart_labels.append(name)
        chart_time_c.append(round(time_c, 2))
        chart_time_cpp.append(round(time_cpp, 2))
        chart_heap_delta.append(heap_delta)
        
        time_ratio = (time_cpp / time_c) if time_c > 0 else 1.0
        badge_class = "badge-pass" if time_ratio < 2.0 else "badge-fail"
        
        svg_file = get_svg_file(r.get("prof"), name)
        flame_json = get_flame_data(r.get("prof"))
        if flame_json:
            flame_data_all[name] = flame_json
        
        c_stats_html = ""
        if c_m:
            c_stats_html = f'''
                <div class="stat-box">
                    <div style="display:flex; justify-content:space-between; margin-bottom:12px;">
                        <h3>C Baseline</h3>
                        <span class="badge badge-pass">REFERENCE</span>
                    </div>
                    <div class="stat-row"><span>Time:</span> <span class="stat-val">{time_c:.2f} ms</span></div>
                    <div class="stat-row"><span>RSS:</span> <span class="stat-val">{rss_c:,} KB</span></div>
                </div>'''
        else:
            c_stats_html = f'''
                <div class="stat-box" style="opacity:0.5;">
                    <h3>C Baseline</h3>
                    <p style="font-size:0.8rem; margin:10px 0;">No baseline available</p>
                </div>'''

        if svg_file:
            analysis_btn = f'<button onclick="toggleAnalysis(\'prof-{name}\')" class="badge" style="margin-top:10px; cursor:pointer; border:none; background:#3b82f6; color:white; padding:6px 12px; width:100%; border-radius:6px; font-weight:600;">VIEW HEAP ANALYSIS</button>'
            
            flame_btn = f'<button onclick="showFlame(\'{name}\')" style="padding:6px 12px; cursor:pointer; background:#fff; border:1px solid #ccc; border-radius:4px; font-size:0.8rem;">Flamegraph</button>' if name in flame_data_all else ""
            
            prof_section = f'''
            <div id="prof-{name}" style="display:none; padding: 0 24px 24px 24px; border-top: 1px solid var(--border); background: #f8fafc;">
                <div style="display:flex; justify-content:space-between; align-items:center;">
                    <h3 style="margin:20px 0 10px 0; font-size:0.9rem; color:var(--secondary);">INTERACTIVE DIAGNOSTICS</h3>
                    <div style="margin-top:10px;">
                        <button onclick="showGraph(\'{name}\')" style="padding:6px 12px; cursor:pointer; background:#fff; border:1px solid #ccc; border-radius:4px; font-size:0.8rem; margin-right:5px;">Call Graph</button>
                        {flame_btn}
                    </div>
                </div>
                <div id="graph-container-{name}" style="background:white; border-radius:8px; border:1px solid var(--border); overflow:hidden; height:600px;">
                    <iframe src="{svg_file}" style="width:100%; height:100%; border:none;"></iframe>
                </div>
                <div id="flame-container-{name}" style="display:none; background:white; border-radius:8px; border:1px solid var(--border); overflow:auto; height:600px; padding:20px;">
                    <div id="flame-div-{name}" style="width:100%;"></div>
                </div>
            </div>'''
        else:
            analysis_btn = ""
            prof_section = ""

        card_html = f'''
        <div class="benchmark-card">
            <div class="benchmark-header">
                <h2>{name}</h2>
                <span class="badge {badge_class}">{time_ratio:.1f}x Slowdown</span>
            </div>
            <div class="benchmark-content">
                <div class="benchmark-desc">
                    <strong>Benchmark Scenario</strong><br>
                    Creation and initialization of {cpp_m.get('loop_count', 'N')} instances.<br><br>
                    <strong>Performance Insight</strong><br>
                    Wrapper adds {(time_cpp - time_c):.2f}ms of logic overhead.<br>
                    Heap growth: <b>{heap_delta:,} bytes</b>.<br>
                    RSS growth: <b>{rss_delta:+,} KB</b>.
                </div>
                <div class="benchmark-stats">
                    <div class="stat-wrapper">
                        {c_stats_html}
                    </div>
                    <div class="stat-wrapper">
                        <div class="stat-box cpp">
                            <div style="display:flex; justify-content:space-between; margin-bottom:12px;">
                                <h3>C++ Wrapper</h3>
                                <span class="badge badge-pass">RECORDED</span>
                            </div>
                            <div class="stat-row"><span>Time:</span> <span class="stat-val">{time_cpp:.2f} ms</span></div>
                            <div class="stat-row"><span>Heap Δ:</span> <span class="stat-val">{heap_delta:,} B</span></div>
                            <div class="stat-row"><span>RSS Δ:</span> <span class="stat-val">{rss_delta:+,} KB</span></div>
                        </div>
                        {analysis_btn}
                    </div>
                </div>
            </div>
            {prof_section}
        </div>'''
        benchmark_cards.append(card_html)

    cards_html = "\n".join(benchmark_cards)
    
    html = f'''<!DOCTYPE html>
<html>
<head>
    <title>LVGL C++ Performance Dashboard</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="https://d3js.org/d3.v7.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/d3-flame-graph@4.1.3/dist/d3-flamegraph.min.js"></script>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/d3-flame-graph@4.1.3/dist/d3-flamegraph.css">
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
        .container {{ max-width: 1400px; margin: 0 auto; }}
        h1 {{ text-align: center; color: var(--text); font-size: 2.5rem; margin-bottom: 0.5rem; }}
        .subtitle {{ text-align: center; color: var(--secondary); margin-bottom: 3rem; }}
        
        .card {{ background: var(--card-bg); padding: 24px; border-radius: 12px; box-shadow: 0 4px 6px -1px rgba(0,0,0,0.1); border: 1px solid var(--border); margin-bottom: 40px; }}
        
        .grid {{ display: grid; grid-template-columns: repeat(auto-fit, minmax(450px, 1fr)); gap: 24px; }}
        .chart-container {{ position: relative; height: 400px; }}
        
        .benchmark-section {{ display: flex; flex-direction: column; gap: 30px; margin-top: 40px; }}
        .benchmark-card {{ background: white; border-radius: 12px; overflow: hidden; box-shadow: 0 2px 4px rgba(0,0,0,0.05); border: 1px solid var(--border); }}
        .benchmark-header {{ background: #f8fafc; padding: 20px 24px; border-bottom: 1px solid var(--border); display: flex; justify-content: space-between; align-items: center; }}
        .benchmark-header h2 {{ margin: 0; font-size: 1.25rem; color: var(--text); }}
        
        .benchmark-content {{ padding: 24px; display: grid; grid-template-columns: 1fr 1fr; gap: 32px; }}
        .benchmark-desc {{ color: var(--secondary); font-size: 0.95rem; }}
        .benchmark-desc strong {{ color: var(--text); font-weight: 600; }}
        
        .benchmark-stats {{ display: grid; grid-template-columns: 1fr 1fr; gap: 16px; align-content: start; }}
        .stat-box {{ background: #f8fafc; padding: 16px; border-radius: 8px; border: 1px solid var(--border); }}
        .stat-box.cpp {{ background: #eff6ff; border-color: #bfdbfe; }}
        .stat-box h3 {{ margin: 0 0 12px 0; font-size: 0.85rem; text-transform: uppercase; letter-spacing: 0.05em; color: var(--secondary); }}
        .stat-row {{ display: flex; justify-content: space-between; margin-bottom: 8px; font-size: 0.9rem; }}
        .stat-val {{ font-weight: 600; font-family: 'JetBrains Mono', monospace; }}
        
        .badge {{ display: inline-flex; align-items: center; padding: 2px 8px; border-radius: 4px; font-size: 0.7rem; font-weight: 700; text-transform: uppercase; }}
        .badge-pass {{ background: #dcfce7; color: var(--success); }}
        .badge-fail {{ background: #fee2e2; color: var(--danger); }}
        
        .d3-flame-graph rect {{ stroke: #eee; fill-opacity: .8; }}
        .d3-flame-graph rect:hover {{ stroke: #474747; stroke-width: .5; cursor: pointer; }}
        .d3-flame-graph-label {{ pointer-events: none; text-anchor: middle; font-size: 10px; font-family: sans-serif; fill: black; }}

        @media (max-width: 900px) {{
            .benchmark-content {{ grid-template-columns: 1fr; }}
        }}
    </style>
</head>
<body>
    <div class="container">
        <h1>LVGL C++ Performance Dashboard</h1>
        <p class="subtitle">Generated on {datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")}</p>
        
        <!-- Executive Summary -->
        <div class="card" style="background: linear-gradient(135deg, #1e293b 0%, #334155 100%); color: white;">
            <h2 style="margin-top:0; color: white;">Executive Summary</h2>
            <div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px;">
                <div style="text-align: center;">
                    <div style="font-size: 2.5rem; font-weight: bold;">{total}</div>
                    <div style="opacity: 0.8; font-size: 0.8rem; text-transform: uppercase;">Benchmarks Run</div>
                </div>
                <div style="text-align: center;">
                    <div style="font-size: 2.5rem; font-weight: bold;">{total_heap_delta:,}</div>
                    <div style="opacity: 0.8; font-size: 0.8rem; text-transform: uppercase;">Total Heap Delta (Bytes)</div>
                </div>
                <div style="text-align: center;">
                    <div style="font-size: 2.5rem; font-weight: bold;">{(total_time_cpp / total_time_c if total_time_c > 0 else 1.0):.1f}x</div>
                    <div style="opacity: 0.8; font-size: 0.8rem; text-transform: uppercase;">Avg. Wrapper Overhead</div>
                </div>
            </div>
        </div>
        
        <!-- Charts -->
        <div class="card">
            <h2 style="margin-top:0;">Performance Overview</h2>
            <div class="grid">
                <div class="chart-container">
                    <canvas id="timeChart"></canvas>
                </div>
                <div class="chart-container">
                    <canvas id="heapChart"></canvas>
                </div>
            </div>
        </div>

        <!-- Detailed Benchmarks -->
        <div class="benchmark-section">
            {cards_html}
        </div>
    </div>

    <script>
        const flameData = {json.dumps(flame_data_all)};
        const initializedFlames = {{}};

        function toggleAnalysis(id) {{
            var x = document.getElementById(id);
            if (x.style.display === "none") {{
                x.style.display = "block";
            }} else {{
                x.style.display = "none";
            }}
        }}

        function showGraph(name) {{
            document.getElementById('graph-container-' + name).style.display = 'block';
            document.getElementById('flame-container-' + name).style.display = 'none';
        }}

        function showFlame(name) {{
            document.getElementById('graph-container-' + name).style.display = 'none';
            document.getElementById('flame-container-' + name).style.display = 'block';
            
            if (!initializedFlames[name] && flameData[name]) {{
                initializedFlames[name] = true;
                setTimeout(() => {{
                    const container = document.getElementById('flame-div-' + name);
                    const width = container.clientWidth;
                    const chart = flamegraph().width(width).height(400);
                    d3.select('#flame-div-' + name)
                        .datum(flameData[name])
                        .call(chart);
                }}, 100);
            }}
        }}

        new Chart(document.getElementById('timeChart'), {{
            type: 'bar',
            data: {{
                labels: {json.dumps(chart_labels)},
                datasets: [
                    {{ label: 'C Baseline (ms)', data: {json.dumps(chart_time_c)}, backgroundColor: '#94a3b8' }},
                    {{ label: 'C++ Wrapper (ms)', data: {json.dumps(chart_time_cpp)}, backgroundColor: '#3b82f6' }}
                ]
            }},
            options: {{
                responsive: true,
                maintainAspectRatio: false,
                plugins: {{ title: {{ display: true, text: 'Execution Time Comparison' }} }},
                scales: {{ y: {{ beginAtZero: true }} }}
            }}
        }});

        new Chart(document.getElementById('heapChart'), {{
            type: 'bar',
            data: {{
                labels: {json.dumps(chart_labels)},
                datasets: [{{ label: 'Heap Delta (bytes)', data: {json.dumps(chart_heap_delta)}, backgroundColor: '#16a34a' }}]
            }},
            options: {{
                responsive: true,
                maintainAspectRatio: false,
                plugins: {{ title: {{ display: true, text: 'Heap Allocation Per Benchmark' }} }},
                scales: {{ y: {{ beginAtZero: true }} }}
            }}
        }});
    </script>
</body>
</html>'''
    
    report_dir = os.path.dirname(REPORT_FILE)
    if report_dir:
        os.makedirs(report_dir, exist_ok=True)
    os.makedirs("build", exist_ok=True)
    
    with open(REPORT_FILE, "w") as f:
        f.write(html)
    print(f"Detailed Report generated: {REPORT_FILE}")

def main():
    if not os.path.exists(BENCH_BINARY_CPP):
        print(f"Binary not found: {BENCH_BINARY_CPP}")
        sys.exit(1)
        
    print("Disovering benchmarks...") 
    benchmarks = get_benchmarks()
    print(f"Found {len(benchmarks)} benchmarks.")
    
    results = []
    for b in benchmarks:
        print(f"  Running C++: {b}...")
        cpp_res, prof_prefix = run_benchmark_cpp(b)
        
        c_res = None
        if b in BASELINE_MAP:
             arg = BASELINE_MAP[b]
             print(f"  Running C Baseline: {arg}...")
             c_res = run_benchmark_c(arg)
             if not c_res:
                 print(f"    Warning: No C baseline found for {b}")
        
        if cpp_res:
            results.append({
                "name": b,
                "cpp": cpp_res,
                "c": c_res,
                "prof": prof_prefix
            })
            
    generate_report(results)

if __name__ == "__main__":
    main()
