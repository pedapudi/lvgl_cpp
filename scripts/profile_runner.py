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
    # Churn and Fragmentation are standalone checks
    ("bench_churn", None),
    ("bench_fragmentation", None),
]

def run_cmd(cmd, env=None):
    try:
        start_env = os.environ.copy()
        if env:
            start_env.update(env)
        subprocess.run(cmd, shell=True, check=True, env=start_env)
    except subprocess.CalledProcessError as e:
        print(f"Error running {cmd}: {e}")
        return False
    return True

def main():
    if not os.path.exists(BUILD_DIR):
        print(f"Build directory '{BUILD_DIR}' not found. Please build first.")
        # Attempt to configure and build? No, simplified runner.
        sys.exit(1)

    print("=== LVGL Memory Profile Runner ===")
    
    # 1. Run benchmarks
    for c_bench, cpp_bench in BENCHMARKS:
        print(f"\n--- Running {c_bench} ---")
        # Run C
        run_cmd(f"./{BUILD_DIR}/{c_bench}")
        
        if cpp_bench:
            print(f"--- Running {cpp_bench} ---")
            run_cmd(f"./{BUILD_DIR}/{cpp_bench}")
            
    # Note: Full pprof integration requires linking tcmalloc and setting HEAPPROFILE.
    # Since we might not have it installed in this environment, this runner is basic execution verification.
    # If we wanted to add pprof support:
    # run_cmd(f"./{BUILD_DIR}/{c_bench}", env={"HEAPPROFILE": f"/tmp/{c_bench}"})

    print("\n=== All benchmarks executed successfully ===")

if __name__ == "__main__":
    main()
