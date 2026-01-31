#!/usr/bin/env python3
import json
import re
from pathlib import Path
from typing import Dict, List, Set

def is_internal(name: str) -> bool:
    INTERNAL_PATTERNS = [
        r'^_lv_', r'_private$', r'_priv$', r'_class_init$', r'_destructor$',
        r'_constructor$', r'_class_', r'_xml_', r'_init_draw_', r'_event_base',
        r'_destruct$', r'_cb$', r'_style_init', r'_style_deinit', r'_obj_init',
        r'_obj_deinit', r'_delete_anim', r'^lv_draw_sw_',
    ]
    for pattern in INTERNAL_PATTERNS:
        if re.search(pattern, name):
            return True
    return False

def main():
    root = Path(__file__).parent.parent
    lvgl_cpp_path = root
    lvgl_path = root.parent / 'lvgl'
    
    # We'll use the same JSON report to find used/idiomatic
    with open(lvgl_cpp_path / 'docs/api_coverage.json', 'r') as f:
        report = json.load(f)
    
    used_funcs = set(report['used_functions'])
    idiomatic_funcs = set(report['idiomatic_covered_functions'])
    total_verified = used_funcs | idiomatic_funcs
    
    # We need the ground truth. Easiest way is to run a simplified version of the audit extraction
    import subprocess
    lvgl_h = lvgl_path / 'lvgl.h'
    cmd = [
        'clang', '-Xclang', '-ast-dump=json', '-fsyntax-only',
        '-I' + str(lvgl_path), '-DLV_CONF_SKIP', str(lvgl_h)
    ]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print("Clang failed")
        return
        
    data = json.loads(result.stdout)
    all_funcs = set()
    
    def walk(node):
        kind = node.get('kind')
        name = node.get('name')
        if kind == 'FunctionDecl' and name and name.startswith('lv_'):
            if not is_internal(name):
                all_funcs.add(name)
        for inner in node.get('inner', []):
            walk(inner)
    walk(data)
    
    # Categorize and find missing
    USER_API_PREFIXES = report['subsystems'].keys()
    
    missing_by_subsystem = {}
    for prefix_name in USER_API_PREFIXES:
        prefix = prefix_name + "_"
        sub_funcs = {f for f in all_funcs if f.startswith(prefix)}
        missing = sub_funcs - total_verified
        if missing:
            missing_by_subsystem[prefix_name] = sorted(list(missing))
            
    # Output to markdown-friendly format
    print("# Missing APIs for Platinum Status\n")
    for sub, missing in sorted(missing_by_subsystem.items()):
        print(f"## {sub} ({len(missing)} missing)")
        for m in missing:
            print(f"- [ ] `{m}`")
        print()

if __name__ == '__main__':
    main()
