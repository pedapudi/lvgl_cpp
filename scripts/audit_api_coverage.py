#!/usr/bin/env python3
"""
Generate an accurate API coverage report for lvgl_cpp using Clang AST analysis.

This script:
1. Extracts public C API functions, enums, and macros from LVGL using Clang AST.
2. Verifies that non-idiomatic functions have logical equivalents in C++ Proxies.
3. Scans lvgl_cpp source files for actual usage.
4. Reports coverage based on AST-verified ground truth.
"""

import os
import re
import sys
import json
import subprocess
from pathlib import Path
from typing import Dict, List, Set, Tuple

# User-facing API prefixes to track
USER_API_PREFIXES = [
    'lv_timer_', 'lv_anim_', 'lv_anim_timeline_', 'lv_obj_', 'lv_display_',
    'lv_indev_', 'lv_group_', 'lv_style_', 'lv_label_', 'lv_button_',
    'lv_arc_', 'lv_bar_', 'lv_slider_', 'lv_checkbox_', 'lv_switch_',
    'lv_textarea_', 'lv_dropdown_', 'lv_roller_', 'lv_chart_', 'lv_table_',
    'lv_keyboard_', 'lv_calendar_', 'lv_menu_', 'lv_msgbox_', 'lv_spinbox_',
    'lv_canvas_', 'lv_image_', 'lv_imagebutton_', 'lv_led_', 'lv_line_',
    'lv_list_', 'lv_span_', 'lv_spangroup_', 'lv_scale_', 'lv_tabview_',
    'lv_tileview_', 'lv_win_', 'lv_flex_', 'lv_grid_', 'lv_fs_',
    'lv_event_', 'lv_async_', 'lv_font_', 'lv_color_', 'lv_palette_',
    'lv_animimg_', 'lv_buttonmatrix_', 'lv_spinner_', 'lv_lottie_',
    'lv_vector_', 'lv_observer_', 'lv_subject_', 'lv_screen_', 'lv_draw_',
    'lv_layer_',
]

# Enum/Macro prefixes to track (LV_* constants)
ENUM_MACRO_PREFIXES = [
    'LV_EVENT_', 'LV_KEY_', 'LV_ALIGN_', 'LV_DIR_', 'LV_FLEX_', 'LV_GRID_',
    'LV_STATE_', 'LV_PART_', 'LV_OPA_', 'LV_PALETTE_', 'LV_ANIM_',
    'LV_SCR_LOAD_ANIM_', 'LV_TEXT_', 'LV_LABEL_', 'LV_BORDER_', 'LV_GRAD_',
    'LV_ARC_', 'LV_BAR_', 'LV_SLIDER_', 'LV_CHART_', 'LV_TABLE_',
    'LV_KEYBOARD_', 'LV_ROLLER_', 'LV_DROPDOWN_', 'LV_MENU_', 'LV_SCALE_',
    'LV_INDEV_', 'LV_DISPLAY_', 'LV_BUTTONMATRIX_', 'LV_IMAGEBUTTON_',
    'LV_SPAN_', 'LV_IMAGE_', 'LV_LOTTIE_', 'LV_VECTOR_', 'LV_SUBJECT_',
    'LV_OBJ_', 'LV_STYLE_',
]

INTERNAL_PATTERNS = [
    r'^_lv_', r'_private$', r'_priv$', r'_class_init$', r'_destructor$',
    r'_constructor$', r'_class_', r'_xml_', r'_init_draw_', r'_event_base',
    r'_destruct$', r'_cb$', r'_style_init', r'_style_deinit', r'_obj_init',
    r'_obj_deinit', r'_delete_anim', r'^lv_draw_sw_',
]

def is_internal(name: str) -> bool:
    for pattern in INTERNAL_PATTERNS:
        if re.search(pattern, name):
            return True
    return False

# Functions covered by Proxies or RAII wrappers
NON_IDIOMATIC_PATTERNS = {
    'StyleProxy': r'lv_obj_(set|get)_style_',
    'StyleProxy_Direct': r'lv_style_(set|get)_',
    'TreeProxy': r'lv_obj_(swap|move_to_index|get_child|get_parent|get_child_count|clean)',
    'InteractionProxy': r'lv_obj_(set_ext_click_area|get_ext_click_area|hit_test|set_group|get_group)',
    'Geometry': r'lv_(area|point|color)_',
    'EventProxy': r'lv_obj_(add|remove|get)_event_cb',
    'LabelProxy': r'lv_label_',
    'VectorProxy': r'lv_vector_path_',
    'SubjectProxy': r'lv_subject_',
}

def get_proxy_mapping(func_name: str) -> str:
    for proxy, pattern in NON_IDIOMATIC_PATTERNS.items():
        if re.search(pattern, func_name):
            return proxy
    return None

def extract_raw_api(lvgl_path: Path) -> Tuple[Set[str], Set[str], Set[str]]:
    """Extract functions, enums, and macros using Clang AST and Preprocessor."""
    lvgl_h = lvgl_path / 'lvgl.h'
    # Include path should be the root of LVGL so that #include "src/..." works
    include_path = lvgl_path
    
    # 1. Functions and Enums via AST
    cmd = [
        'clang', '-Xclang', '-ast-dump=json', '-fsyntax-only',
        '-I' + str(include_path), '-DLV_CONF_SKIP', str(lvgl_h)
    ]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0 and not result.stdout:
        print(f"Clang failed: {result.stderr}")
        sys.exit(1)
        
    data = json.loads(result.stdout)
    functions = set()
    enums = set()
    
    def walk(node):
        kind = node.get('kind')
        name = node.get('name')
        if kind == 'FunctionDecl' and name and name.startswith('lv_'):
            functions.add(name)
        elif kind == 'EnumConstantDecl' and name and name.startswith('LV_'):
            enums.add(name)
        for inner in node.get('inner', []):
            walk(inner)
            
    walk(data)
    
    # 2. Macros via Preprocessor
    cmd = [
        'clang', '-E', '-dM', '-I' + str(include_path), 
        '-DLV_CONF_SKIP', str(lvgl_h)
    ]
    result = subprocess.run(cmd, capture_output=True, text=True)
    macros = set()
    for line in result.stdout.splitlines():
        # Match LV_ macros and also common LVGL constants that might not have LV_ prefix (rare)
        match = re.search(r'#define\s+(LV_[A-Z_0-9]+)\b', line)
        if match:
            m = match.group(1)
            # Filter out internal-looking macros but keep things like LV_ANIM_REPEAT_INFINITE
            if not m.startswith('LV_UNUSED') and not m.startswith('LV_ATTRIBUTE'):
                macros.add(m)
            
    return functions, enums, macros

def scan_proxy_methods(lvgl_cpp_path: Path) -> Dict[str, Set[str]]:
    """Scan Proxy classes for available methods to verify logical equivalence."""
    proxies = {
        'StyleProxy': [lvgl_cpp_path / 'core/style_proxy.h', lvgl_cpp_path / 'misc/style_base.h'],
        'TreeProxy': [lvgl_cpp_path / 'core/tree_proxy.h'],
        'InteractionProxy': [lvgl_cpp_path / 'core/interaction_proxy.h'],
        'EventProxy': [lvgl_cpp_path / 'core/event_proxy.h'],
        'LabelProxy': [lvgl_cpp_path / 'widgets/label.h'],
        'VectorProxy': [lvgl_cpp_path / 'draw/vector/vector_path.h', lvgl_cpp_path / 'draw/vector/vector_draw.h'],
        'SubjectProxy': [lvgl_cpp_path / 'core/subject.h'],
    }
    
    methods = {name: set() for name in proxies}
    for name, paths in proxies.items():
        for path in paths:
            if not path.exists(): continue
            with open(path, 'r') as f:
                content = f.read()
                # Simple regex for method names
                # Focus on common patterns like set_X, get_X, or just X
                # Updated to catch 'MethodName(' even with return types and 'const'
                matches = re.findall(r'[\w&*]+\s+(\w+)\s*\(', content)
                methods[name].update(matches)
            
    return methods

def scan_usage(cpp_path: Path, functions: Set[str], constants: Set[str]) -> Tuple[Set[str], Set[str]]:
    """Scan C++ code for usage of C APIs and constants."""
    used_functions = set()
    used_constants = set()
    
    # Pre-compile patterns for speed
    func_pattern = re.compile(r'\b(lv_\w+)\b')
    const_pattern = re.compile(r'\b(LV_[A-Z_0-9]+)\b')
    
    for source_file in list(cpp_path.rglob('*.cpp')) + list(cpp_path.rglob('*.h')):
        # Only skip build directories, internal .git, etc.
        # We WANT to scan tests to verify verified coverage.
        if any(skip in str(source_file) for skip in ['build', 'example', '.git', '.agent', 'brain']):
            continue
        try:
            with open(source_file, 'r', errors='ignore') as f:
                content = f.read()
            # Basic strip
            content = re.sub(r'//.*?$', '', content, flags=re.MULTILINE)
            content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
            
            for match in func_pattern.finditer(content):
                f = match.group(1)
                if f in functions: used_functions.add(f)
            for match in const_pattern.finditer(content):
                c = match.group(1)
                if c in constants: used_constants.add(c)
        except: continue
        
    return used_functions, used_constants

def generate_report(lvgl_path: Path, cpp_path: Path):
    funcs, enums, macros = extract_raw_api(lvgl_path)
    all_constants = enums | macros
    used_funcs, used_consts = scan_usage(cpp_path, funcs, all_constants)
    proxy_methods = scan_proxy_methods(cpp_path)
    
    report = {
        'summary': {
            'total_c_functions': len([f for f in funcs if not is_internal(f)]),
            'total_c_constants': len([c for c in all_constants if not is_internal(c)]),
            'functions_wrapped': len(used_funcs),
            'functions_idiomatic': 0,
            'total_verified': 0,
            'constants_used': len(used_consts),
            'internal_violations': [],
            'logical_equivalence_gaps': []
        },
        'used_functions': sorted(list(used_funcs)),
        'idiomatic_covered_functions': [],
        'used_constants': sorted(list(used_consts)),
        'subsystems': {},
        'proxies': {name: sorted(list(methods)) for name, methods in proxy_methods.items()}
    }
    
    # DEBUG: Check for a specific problematic constant
    if 'LV_ANIM_REPEAT_INFINITE' in all_constants:
        print(f"DEBUG: LV_ANIM_REPEAT_INFINITE found in Ground Truth.")
        if 'LV_ANIM_REPEAT_INFINITE' in used_consts:
            print(f"DEBUG: LV_ANIM_REPEAT_INFINITE found in USAGE.")
        else:
            print(f"DEBUG: LV_ANIM_REPEAT_INFINITE NOT found in USAGE.")
    else:
        print(f"DEBUG: LV_ANIM_REPEAT_INFINITE NOT found in Ground Truth!")
    
    # Logical Equivalence Check
    idiomatic_covered_funcs = set()
    for f in funcs:
        if is_internal(f): continue
        if f in used_funcs: continue
        proxy = get_proxy_mapping(f)
        if proxy:
            # e.g., lv_obj_set_style_text_font -> text_font (in StyleProxy)
            # or lv_style_set_text_font -> text_font
            m = None
            prefix = ""
            prefix = "set_" if "_set_" in f or f.startswith("lv_style_set_") else "get_"
            if proxy.startswith('StyleProxy'):
                m = re.sub(r'lv_(obj|style)_(set|get)_style_', '', f)
                m = re.sub(r'lv_style_(set|get)_', '', m)
                lookup = 'StyleProxy'
            elif proxy == 'TreeProxy':
                m = f.replace('lv_obj_', '')
                lookup = 'TreeProxy'
            elif proxy == 'InteractionProxy':
                m = f.replace('lv_obj_', '').replace('set_', '').replace('get_', '')
                lookup = 'InteractionProxy'
            elif proxy == 'VectorProxy':
                # Map lv_vector_path_append_rect to append_rectangle
                m = f.replace('lv_vector_path_', '').replace('set_', '').replace('get_', '')
                if m == 'append_rect': m = 'append_rectangle'
                lookup = 'VectorProxy'
            elif proxy == 'LabelProxy':
                # Label uses set_text_fmt but calls vfmt internally
                m = f.replace('lv_label_', '').replace('set_', '').replace('get_', '')
                lookup = 'LabelProxy'
            elif proxy == 'SubjectProxy':
                m = f.replace('lv_subject_', '').replace('set_', '').replace('get_', '')
                lookup = 'SubjectProxy'
            elif proxy == 'EventProxy':
                m = f.replace('lv_obj_', '').replace('add_', '').replace('remove_', '').replace('get_', '')
                lookup = 'EventProxy'
            
            if m and lookup in proxy_methods:
                # Check both with and without prefix for better matching
                possible = {m, prefix + m}
                if not (possible & proxy_methods[lookup]):
                    report['summary']['logical_equivalence_gaps'].append({
                        'function': f,
                        'proxy': lookup,
                        'missing_method': f"{prefix}{m} or {m}"
                    })
                else:
                    idiomatic_covered_funcs.add(f)
                    
    report['idiomatic_covered_functions'] = sorted(list(idiomatic_covered_funcs))
    report['summary']['functions_idiomatic'] = len(idiomatic_covered_funcs)
    # Total verified is union of direct and idiomatic coverage
    total_verified_funcs = used_funcs | idiomatic_covered_funcs
    report['summary']['total_verified'] = len(total_verified_funcs)
                    
    # Categorize by subsystem
    for prefix in USER_API_PREFIXES:
        sub_funcs = {f for f in funcs if f.startswith(prefix) and not is_internal(f)}
        if not sub_funcs: continue
        
        wrapped = sub_funcs & used_funcs
        idiomatic = sub_funcs & idiomatic_covered_funcs
        verified = sub_funcs & total_verified_funcs
        
        report['subsystems'][prefix.rstrip('_')] = {
            'total': len(sub_funcs),
            'wrapped': len(wrapped),
            'idiomatic': len(idiomatic),
            'verified': len(verified),
            'direct_percent': round(100 * len(wrapped) / len(sub_funcs), 1) if sub_funcs else 0,
            'verified_percent': round(100 * len(verified) / len(sub_funcs), 1) if sub_funcs else 0
        }
        
    return report

def main():
    script_dir = Path(__file__).parent.parent
    lvgl_path = script_dir.parent / 'lvgl'
    cpp_path = script_dir
    
    print(f"Starting AST-based API Audit...")
    report = generate_report(lvgl_path, cpp_path)
    
    print(f"\nAPI Coverage Summary:")
    print(f"  C Functions (Direct Usage): {report['summary']['functions_wrapped']} / {report['summary']['total_c_functions']}")
    print(f"  C Functions (Proxy Covered): {report['summary']['functions_idiomatic']}")
    print(f"  TOTAL VERIFIED COVERAGE: {report['summary']['total_verified']} / {report['summary']['total_c_functions']} ({round(100 * report['summary']['total_verified'] / report['summary']['total_c_functions'], 1)}%)")
    print(f"  C Constants: {report['summary']['constants_used']} / {report['summary']['total_c_constants']}")
    
    gaps = report['summary']['logical_equivalence_gaps']
    if gaps:
        print(f"\n⚠️ Logical Equivalence Gaps ({len(gaps)}):")
        # Just show first 5
        for g in gaps[:5]:
            print(f"    - {g['function']} -> {g['proxy']} (missing '{g['missing_method']}')")
        if len(gaps) > 5: print(f"    ... and {len(gaps)-5} more")
    else:
        print(f"\n✅ All non-idiomatic APIs have logical equivalents in Proxies.")
        
    # Save
    with open(cpp_path / 'docs/api_coverage.json', 'w') as f:
        json.dump(report, f, indent=2)
    print(f"\nDetailed report saved to docs/api_coverage.json")

if __name__ == '__main__':
    main()
