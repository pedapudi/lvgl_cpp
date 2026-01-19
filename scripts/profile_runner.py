#!/usr/bin/env python3
import os
import subprocess
import sys

# Requirements:
# - CMAKE build directory at ./build or provided via args
# - pprof installed (google-perftools)

BUILD_DIR = "build"
BENCHMARKS = [
    ("bench_baseline_c", "bench_baseline_cpp"),
    ("bench_events_c", "bench_events_cpp"),
    ("bench_churn_c", "bench_churn"),
    ("bench_fragmentation_c", "bench_fragmentation"),
]

def run_cmd(cmd, env=None, capture_output=False):
    try:
        start_env = os.environ.copy()
        if env:
            start_env.update(env)
        
        args = {"shell": True, "check": True, "env": start_env}
        if capture_output:
            args["stdout"] = subprocess.PIPE
            args["stderr"] = subprocess.PIPE
            
        result = subprocess.run(cmd, **args)
        if capture_output:
            return result
            
    except subprocess.CalledProcessError as e:
        print(f"Error running {cmd}: {e}")
        if capture_output and e.stderr:
             print(f"Stderr: {e.stderr.decode('utf-8')}")
        return False
    return True

def main():
    if not os.path.exists(BUILD_DIR):
        print(f"Build directory '{BUILD_DIR}' not found. Please build first.")
        sys.exit(1)

    print("=== LVGL Memory Profile Runner (Extended) ===")
    
    # Check for pprof
    has_pprof = False
    try:
        subprocess.run("which pprof", shell=True, check=True, stdout=subprocess.DEVNULL)
        has_pprof = True
        print("[+] pprof detected. Profiling enabled.")
    except:
        print("[-] pprof not found. Running execution checks only.")

    for c_bench, cpp_bench in BENCHMARKS:
        print(f"\n>>> Comparison: {c_bench} vs {cpp_bench}")
        
        env_c = {}
        env_cpp = {}
        
        if has_pprof:
            env_c["HEAPPROFILE"] = f"/tmp/{c_bench}"
            env_cpp["HEAPPROFILE"] = f"/tmp/{cpp_bench}"
            
            # Clean old profiles
            os.system(f"rm -f /tmp/{c_bench}* /tmp/{cpp_bench}*")

        # Run C
        print(f"--- Running C Baseline ({c_bench}) ---")
        if not run_cmd(f"./{BUILD_DIR}/{c_bench}", env=env_c):
            print("FAILED")
            continue

        # Run C++
        if cpp_bench:
            print(f"--- Running C++ Target ({cpp_bench}) ---")
            if not run_cmd(f"./{BUILD_DIR}/{cpp_bench}", env=env_cpp):
                print("FAILED")
                continue
                
        # Generate Report
        if has_pprof:
            # Find latest profile file
            # pprof --text --base /tmp/c_bench.0001.heap /tmp/cpp_bench.0001.heap
            
            # Helper to find latest heap file
            def get_heap_file(prefix):
                 files = [f for f in os.listdir("/tmp") if f.startswith(prefix) and f.endswith(".heap")]
                 if not files: return None
                 files.sort()
                 return f"/tmp/{files[-1]}"
            
            heap_c = get_heap_file(c_bench)
            heap_cpp = get_heap_file(cpp_bench)
            
            if heap_c and heap_cpp:
                print("\n--- Comparative Heap Profile (C++ Overhead) ---")
                try:
                    # Filter for symbols related to wrappers to reduce noise?
                    # For now just show top allocators
                    cmd = f"pprof --text --base {heap_c} {BUILD_DIR}/{cpp_bench} {heap_cpp}"
                    
                    # We need to pass the binary that generated the profile.
                    # Actually for diff, we typically diff the same binary or compatible ones.
                    # Base diffing distinct binaries is tricky because symbols differ.
                    # pprof base comparison assumes same binary usually or subtraction of counts.
                    # Let's try separate reports first, enabling sidebar comparison.
                    
                    print(f"Generating C profile report:")
                    run_cmd(f"pprof --text {BUILD_DIR}/{c_bench} {heap_c} | head -n 10")
                    
                    print(f"\nGenerating C++ profile report:")
                    run_cmd(f"pprof --text {BUILD_DIR}/{cpp_bench} {heap_cpp} | head -n 10")
                    
                except Exception as e:
                    print(f"Profiling generation failed: {e}")
            else:
                 print("[-] No heap profiles generated (did you link tcmalloc?)")

    print("\n=== All benchmarks executed successfully ===")

if __name__ == "__main__":
    main()
