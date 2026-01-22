#!/usr/bin/env python3
"""
Generate an accurate API coverage report for lvgl_cpp.

This script:
1. Extracts public C API functions from LVGL headers
2. Scans ALL lvgl_cpp source files for actual usage of these functions
3. Reports coverage based on actual calls, not method name matching

Usage:
  python3 scripts/audit_api_coverage.py [lvgl_path] [lvgl_cpp_path]
"""

import os
import re
import sys
import json
from pathlib import Path
from typing import Dict, List, Set, Tuple


# Subsystem prefixes to track
API_PREFIXES = [
    'lv_timer_',
    'lv_anim_',
    'lv_obj_',
    'lv_display_',
    'lv_indev_',
    'lv_group_',
    'lv_style_',
    'lv_label_',
    'lv_button_',
    'lv_arc_',
    'lv_bar_',
    'lv_slider_',
    'lv_checkbox_',
    'lv_switch_',
    'lv_textarea_',
    'lv_dropdown_',
    'lv_roller_',
    'lv_chart_',
    'lv_table_',
    'lv_keyboard_',
    'lv_calendar_',
    'lv_menu_',
    'lv_msgbox_',
    'lv_spinbox_',
    'lv_canvas_',
    'lv_image_',
    'lv_led_',
    'lv_line_',
    'lv_list_',
    'lv_span_',
    'lv_scale_',
    'lv_tabview_',
    'lv_tileview_',
    'lv_win_',
    'lv_flex_',
    'lv_grid_',
    'lv_fs_',
    'lv_draw_',
    'lv_event_',
    'lv_async_',
    'lv_font_',
    'lv_color_',
    'lv_palette_',
]


def extract_c_functions(lvgl_path: Path) -> Dict[str, Set[str]]:
    """Extract all public C function names from LVGL headers, grouped by prefix."""
    functions: Dict[str, Set[str]] = {prefix: set() for prefix in API_PREFIXES}
    
    for header in lvgl_path.rglob('*.h'):
        if 'private' in str(header).lower():
            continue
            
        try:
            with open(header, 'r', errors='ignore') as f:
                content = f.read()
        except:
            continue
        
        # Remove comments to avoid false matches
        content = re.sub(r'//.*?$', '', content, flags=re.MULTILINE)
        content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
        
        # Pattern: function declarations
        for prefix in API_PREFIXES:
            pattern = rf'\b({re.escape(prefix)}\w+)\s*\('
            for match in re.finditer(pattern, content):
                func_name = match.group(1)
                # Skip macros (all caps after prefix) and internal functions
                suffix = func_name[len(prefix):]
                if suffix.isupper() or suffix.startswith('_'):
                    continue
                functions[prefix].add(func_name)
    
    return functions


def scan_cpp_for_usage(cpp_path: Path) -> Set[str]:
    """Scan all lvgl_cpp source files for LVGL C function calls."""
    used_functions: Set[str] = set()
    
    # Scan both .h and .cpp files
    for source_file in list(cpp_path.rglob('*.cpp')) + list(cpp_path.rglob('*.h')):
        # Skip test files for coverage (they test the wrapper, not wrap LVGL)
        if 'test' in str(source_file):
            continue
        # Skip build directories
        if 'build' in str(source_file):
            continue
            
        try:
            with open(source_file, 'r', errors='ignore') as f:
                content = f.read()
        except:
            continue
        
        # Remove comments
        content = re.sub(r'//.*?$', '', content, flags=re.MULTILINE)
        content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
        
        # Find all lv_* function calls
        for prefix in API_PREFIXES:
            pattern = rf'\b({re.escape(prefix)}\w+)\s*\('
            for match in re.finditer(pattern, content):
                func_name = match.group(1)
                used_functions.add(func_name)
    
    return used_functions


def generate_coverage_report(lvgl_path: Path, cpp_path: Path) -> Dict:
    """Generate an accurate coverage report."""
    c_functions = extract_c_functions(lvgl_path)
    used_functions = scan_cpp_for_usage(cpp_path)
    
    report = {
        'summary': {'total_c_functions': 0, 'wrapped': 0, 'not_wrapped': 0},
        'subsystems': {}
    }
    
    for prefix in API_PREFIXES:
        all_funcs = c_functions[prefix]
        if not all_funcs:
            continue
            
        wrapped = all_funcs & used_functions
        not_wrapped = all_funcs - used_functions
        
        subsystem = {
            'prefix': prefix,
            'total': len(all_funcs),
            'wrapped_count': len(wrapped),
            'wrapped': sorted(list(wrapped)),
            'not_wrapped': sorted(list(not_wrapped))
        }
        
        report['summary']['total_c_functions'] += len(all_funcs)
        report['summary']['wrapped'] += len(wrapped)
        report['summary']['not_wrapped'] += len(not_wrapped)
        
        report['subsystems'][prefix.rstrip('_')] = subsystem
    
    # Calculate percentage
    total = report['summary']['total_c_functions']
    if total > 0:
        report['summary']['coverage_percent'] = round(
            100 * report['summary']['wrapped'] / total, 1
        )
    else:
        report['summary']['coverage_percent'] = 0
    
    return report


def print_report(report: Dict):
    """Print a human-readable coverage report."""
    summary = report['summary']
    print(f"\n{'='*60}")
    print(f"LVGL C API Coverage Report (Accurate)")
    print(f"{'='*60}")
    print(f"\nSummary:")
    print(f"  Total C functions in LVGL: {summary['total_c_functions']}")
    print(f"  Called by lvgl_cpp:        {summary['wrapped']}")
    print(f"  Not used:                  {summary['not_wrapped']}")
    print(f"  Coverage:                  {summary['coverage_percent']}%")
    
    print(f"\n{'-'*60}")
    print(f"Subsystem Details:")
    print(f"{'-'*60}")
    
    for name, data in sorted(report['subsystems'].items(), key=lambda x: -x[1]['wrapped_count']):
        wrapped_count = data['wrapped_count']
        total = data['total']
        pct = round(100 * wrapped_count / total, 1) if total > 0 else 0
        print(f"\n{name}: {wrapped_count}/{total} ({pct}%)")
        
        # Show a few not-wrapped functions for subsystems with low coverage
        not_wrapped = data['not_wrapped']
        if not_wrapped and pct < 80 and len(not_wrapped) <= 15:
            print(f"  Not used:")
            for func in not_wrapped[:15]:
                print(f"    - {func}")
        elif not_wrapped and pct < 80:
            print(f"  Not used: {len(not_wrapped)} functions (too many to list)")


def main():
    if len(sys.argv) >= 3:
        lvgl_path = Path(sys.argv[1])
        cpp_path = Path(sys.argv[2])
    else:
        # Default paths
        script_dir = Path(__file__).parent.parent
        lvgl_path = script_dir.parent / 'lvgl'
        cpp_path = script_dir
    
    if not lvgl_path.exists():
        print(f"Error: LVGL path not found: {lvgl_path}")
        sys.exit(1)
    
    if not cpp_path.exists():
        print(f"Error: lvgl_cpp path not found: {cpp_path}")
        sys.exit(1)
    
    print(f"Scanning LVGL at: {lvgl_path}")
    print(f"Checking usage in: {cpp_path}")
    
    report = generate_coverage_report(lvgl_path, cpp_path)
    print_report(report)
    
    # Save JSON report
    report_path = cpp_path / 'docs' / 'api_coverage.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    with open(report_path, 'w') as f:
        json.dump(report, f, indent=2)
    print(f"\nJSON report saved to: {report_path}")


if __name__ == '__main__':
    main()
