#!/usr/bin/env python3
import os
import subprocess
import sys
import tempfile

def check_headers():
    # Set the root directory to the parent of the script directory
    script_dir = os.path.dirname(os.path.abspath(__file__))
    root_dir = os.path.abspath(os.path.join(script_dir, ".."))
    
    headers = []
    for root, dirs, files in os.walk(root_dir):
        # Skip directories that shouldn't be searched
        if "build" in dirs:
            dirs.remove("build")
        if "lvgl" in dirs:
            dirs.remove("lvgl")
        if ".git" in dirs:
            dirs.remove(".git")
        if "examples" in dirs:
            dirs.remove("examples")
        if ".agent" in root:
            continue
            
        for file in files:
            if file.endswith(".h") and "lv_conf_test.h" not in file:
                headers.append(os.path.join(root, file))

    print(f"Checking {len(headers)} headers in {root_dir}")
    
    failed = []
    
    for header in headers:
        rel_path = os.path.relpath(header, root_dir)
        with tempfile.NamedTemporaryFile(suffix=".cpp", mode="w") as f:
            f.write(f'#include "{rel_path}"\n')
            f.write('int main() { return 0; }\n')
            f.flush()
            
            # Using include paths from the root
            cmd = [
                "g++", "-std=c++20", "-c", f.name,
                f"-I{root_dir}",
                f"-I{root_dir}/../lvgl", # Crucial: Point to the actual LVGL root
                f"-I{root_dir}/../lvgl/src",
                "-o", "/dev/null"
            ]
            
            # Use the test configuration
            cmd.append(f"-DLV_CONF_SKIP")
            cmd.append(f'-DLV_CONF_PATH="{root_dir}/tests/lv_conf_test.h"')
            
            result = subprocess.run(cmd, capture_output=True, text=True)
            if result.returncode != 0:
                print(f"[FAIL] {rel_path}")
                # Filter noise if necessary, but keep error details
                print(result.stderr)
                failed.append(rel_path)

    if failed:
        print(f"\nFailed headers ({len(failed)}):")
        for f in failed:
            print(f"  - {f}")
        sys.exit(1)
    else:
        print("\nAll headers passed standalone compilation check.")

if __name__ == "__main__":
    check_headers()
