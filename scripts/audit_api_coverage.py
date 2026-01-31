#!/usr/bin/env python3
"""
Generate an accurate API coverage report for lvgl_cpp.

This script:
1. Extracts public C API functions and enums from LVGL headers
2. Scans ALL lvgl_cpp source files for actual usage
3. Reports coverage based on actual usage
4. Flags any use of internal LVGL APIs

Usage:
  python3 scripts/audit_api_coverage.py [lvgl_path] [lvgl_cpp_path]
"""

import os
import re
import sys
import json
from pathlib import Path
from typing import Dict, List, Set, Tuple


# User-facing API prefixes to track
USER_API_PREFIXES = [
    'lv_timer_',
    'lv_anim_',
    'lv_anim_timeline_',
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
    'lv_imagebutton_',
    'lv_led_',
    'lv_line_',
    'lv_list_',
    'lv_span_',
    'lv_spangroup_',
    'lv_scale_',
    'lv_tabview_',
    'lv_tileview_',
    'lv_win_',
    'lv_flex_',
    'lv_grid_',
    'lv_fs_',
    'lv_event_',
    'lv_async_',
    'lv_font_',
    'lv_color_',
    'lv_palette_',
    'lv_animimg_',
    'lv_buttonmatrix_',
    'lv_imagebutton_',
    'lv_spinner_',
    'lv_lottie_',
    'lv_vector_',
    'lv_observer_',
    'lv_subject_',
    'lv_screen_',
    'lv_draw_',
    'lv_layer_',
]

# Enum prefixes to track (LV_* constants)
ENUM_PREFIXES = [
    'LV_EVENT_',
    'LV_KEY_',
    'LV_ALIGN_',
    'LV_DIR_',
    'LV_FLEX_',
    'LV_GRID_',
    'LV_STATE_',
    'LV_PART_',
    'LV_OPA_',
    'LV_PALETTE_',
    'LV_ANIM_',
    'LV_SCR_LOAD_ANIM_',
    'LV_TEXT_',
    'LV_LABEL_',
    'LV_BORDER_',
    'LV_GRAD_',
    'LV_ARC_',
    'LV_BAR_',
    'LV_SLIDER_',
    'LV_CHART_',
    'LV_TABLE_',
    'LV_KEYBOARD_',
    'LV_ROLLER_',
    'LV_DROPDOWN_',
    'LV_MENU_',
    'LV_SCALE_',
    'LV_INDEV_',
    'LV_DISPLAY_',
    'LV_BUTTONMATRIX_',
    'LV_IMAGEBUTTON_',
    'LV_SPAN_',
    'LV_IMAGE_',
    'LV_LOTTIE_',
    'LV_VECTOR_',
    'LV_SUBJECT_',
    'LV_OBJ_',
    'LV_STYLE_',
]

# Internal/private API patterns
INTERNAL_PATTERNS = [
    r'^_lv_',
    r'_private$',
    r'_priv$',
    r'_class_init$',
    r'_destructor$',
    r'_constructor$',
    r'_class_',          # Widget class factory methods
    r'_xml_',            # XML parser internal bindings
    r'_init_draw_',      # Low-level draw descriptor init
    r'_event_base',      # Internal event dispatch
    r'_destruct$',       # Destructors
    r'_cb$',             # Callback implementations (not setters)
    r'_style_init',      # Internal style initialization
    r'_style_deinit',    # Internal style deinitialization
    r'_obj_init',        # Internal object initialization
    r'_obj_deinit',      # Internal object deinitialization
    r'_delete_anim',     # Internal animation cleanup
]

DRAW_INTERNAL_PATTERNS = [
    r'^lv_draw_sw_',
]


def is_internal_function(func_name: str, header_path: str) -> bool:
    """Check if a function is internal/private."""
    path_str = str(header_path).lower()
    if 'private' in path_str or '_priv' in path_str:
        return True
    if 'lvgl_private.h' in path_str:
        return True
    
    for pattern in INTERNAL_PATTERNS:
        if re.search(pattern, func_name):
            return True
    
    for pattern in DRAW_INTERNAL_PATTERNS:
        if re.search(pattern, func_name):
            return True
            
    # Specific internal handlers to exclude
    if func_name in [
        'lv_indev_scroll_handler',
        'lv_indev_scroll_throw_handler',
        'lv_timer_handler_run_in_period', # Internal helper
        'lv_display_delete_refr_timer',   # Internal timer management
    ]:
        return True
    
    return False


# Non-idiomatic/Generated patterns that are covered by proxies or better abstractions
NON_IDIOMATIC_PATTERNS = [
    r'lv_obj_(set|get)_style_',      # Covered by StyleProxy
    r'lv_\w+_(set|get)_style_',      # Covered by StyleProxy
    r'lv_area_',                     # Covered by lvgl::Area
    r'lv_point_',                    # Covered by lvgl::Point
    r'lv_color_',                    # Covered by lvgl::Color
    r'lv_palette_',                  # Covered by lvgl::Color/Palette
    r'lv_obj_add_event_cb',          # Covered by EventProxy/Object::on_...
    r'lv_obj_remove_event_cb',       # Covered by EventProxy
    r'lv_obj_get_event_cb',          # Covered by EventProxy
    r'lv_style_set_',                # Covered by StyleProxy
    r'lv_style_get_',                # Covered by StyleProxy
    r'lv_obj_set_ext_click_area',    # Planned for InteractionProxy
    r'lv_obj_get_ext_click_area',    # Planned for InteractionProxy
    r'lv_obj_hit_test',              # Planned for InteractionProxy
    r'lv_obj_swap',                  # Planned for TreeProxy
    r'lv_obj_move_to_index',         # Planned for TreeProxy
    r'lv_obj_get_draw_area',         # Handled by Area wrapper for redraw
]


def is_non_idiomatic(func_name: str) -> bool:
    """Check if a function should not be explicitly wrapped (covered by proxies/types)."""
    for pattern in NON_IDIOMATIC_PATTERNS:
        if re.search(pattern, func_name):
            return True
    return False


def extract_enums(lvgl_path: Path) -> Dict[str, Set[str]]:
    """Extract enum values from LVGL headers."""
    enums: Dict[str, Set[str]] = {prefix: set() for prefix in ENUM_PREFIXES}
    
    for header in lvgl_path.rglob('*.h'):
        if 'private' in str(header).lower():
            continue
        if 'test' in str(header).lower():
            continue
        if 'api_map' in str(header).lower():
            continue
            
        try:
            with open(header, 'r', errors='ignore') as f:
                content = f.read()
        except:
            continue
        
        # Remove comments
        content = re.sub(r'//.*?$', '', content, flags=re.MULTILINE)
        content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
        
        # Find enum values
        for prefix in ENUM_PREFIXES:
            pattern = rf'\b({re.escape(prefix)}[A-Z_0-9]+)\b'
            for match in re.finditer(pattern, content):
                enum_val = match.group(1)
                # Skip if it's a typedef or type declaration
                if not re.search(rf'typedef.*{re.escape(enum_val)}', content):
                    enums[prefix].add(enum_val)
    
    return enums


def extract_c_functions(lvgl_path: Path, skip_non_idiomatic: bool = True) -> Tuple[Dict[str, Set[str]], Set[str]]:
    """Extract C functions from LVGL headers."""
    user_functions: Dict[str, Set[str]] = {prefix: set() for prefix in USER_API_PREFIXES}
    internal_functions: Set[str] = set()
    
    for header in lvgl_path.rglob('*.h'):
        if 'test' in str(header).lower() or 'example' in str(header).lower():
            continue
        if 'api_map' in str(header).lower():
            continue
            
        try:
            with open(header, 'r', errors='ignore') as f:
                content = f.read()
        except:
            continue
        
        content = re.sub(r'//.*?$', '', content, flags=re.MULTILINE)
        content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
        
        for prefix in USER_API_PREFIXES:
            pattern = rf'\b({re.escape(prefix)}\w+)\s*\('
            for match in re.finditer(pattern, content):
                func_name = match.group(1)
                suffix = func_name[len(prefix):]
                if suffix.isupper():
                    continue
                
                if is_internal_function(func_name, header):
                    internal_functions.add(func_name)
                elif skip_non_idiomatic and is_non_idiomatic(func_name):
                    continue
                else:
                    user_functions[prefix].add(func_name)
        
        internal_pattern = r'\b(_lv_\w+)\s*\('
        for match in re.finditer(internal_pattern, content):
            internal_functions.add(match.group(1))
    
    return user_functions, internal_functions


def scan_cpp_for_usage(cpp_path: Path, known_functions: Set[str]) -> Tuple[Set[str], Set[str], Set[str]]:
    """Scan lvgl_cpp for LVGL C function and enum usage."""
    user_calls: Set[str] = set()
    internal_calls: Set[str] = set()
    enum_usage: Set[str] = set()
    
    # Pre-compile patterns
    enum_patterns = [re.compile(rf'\b({re.escape(prefix)}[A-Z_0-9]+)\b') for prefix in ENUM_PREFIXES]
    token_pattern = re.compile(r'\b(lv_\w+)\b')
    internal_token_pattern = re.compile(r'\b(_lv_\w+)\b')
    
    for source_file in list(cpp_path.rglob('*.cpp')) + list(cpp_path.rglob('*.h')):
        if any(skip in str(source_file) for skip in ['test', 'build', 'example', '.git']):
            continue
            
        try:
            with open(source_file, 'r', errors='ignore') as f:
                content = f.read()
        except:
            continue
        
        content = re.sub(r'//.*?$', '', content, flags=re.MULTILINE)
        content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
        
        # 1. Scan for function calls AND callback usage using tokens
        # This is O(Tokens) instead of O(Functions * FileSize)
        for match in token_pattern.finditer(content):
            token = match.group(1)
            if token in known_functions:
                user_calls.add(token)
        
        # 2. Scan for internal functions
        for match in internal_token_pattern.finditer(content):
            internal_calls.add(match.group(1))
        
        # 3. Scan for enum usage
        for pattern in enum_patterns:
            for match in pattern.finditer(content):
                enum_usage.add(match.group(1))
    
    return user_calls, internal_calls, enum_usage


def generate_report(lvgl_path: Path, cpp_path: Path) -> Dict:
    """Generate coverage report."""
    user_api_dict, internal_api = extract_c_functions(lvgl_path, skip_non_idiomatic=True)
    raw_user_api_dict, _ = extract_c_functions(lvgl_path, skip_non_idiomatic=False)
    
    # Flatten user_api for easier scanning
    known_user_functions: Set[str] = set()
    for funcs in user_api_dict.values():
        known_user_functions.update(funcs)
        
    enums = extract_enums(lvgl_path)
    used_user_api, used_internal_api, used_enums = scan_cpp_for_usage(cpp_path, known_user_functions)
    
    report = {
        'summary': {
            'total_user_api': 0,
            'total_raw_api': 0,
            'wrapped': 0,
            'not_wrapped': 0,
            'non_idiomatic_count': 0,
            'internal_api_violations': [],
            'total_enums': 0,
            'enums_used': 0,
        },
        'subsystems': {},
        'enums': {}
    }
    
    # Calculate non-idiomatic count
    total_raw = sum(len(funcs) for funcs in raw_user_api_dict.values())
    total_idiomatic = sum(len(funcs) for funcs in user_api_dict.values())
    report['summary']['total_raw_api'] = total_raw
    report['summary']['non_idiomatic_count'] = total_raw - total_idiomatic
    
    # Check for internal API violations
    for func in used_internal_api:
        if func in internal_api:
            report['summary']['internal_api_violations'].append(func)
    
    # Function coverage per subsystem
    for prefix in USER_API_PREFIXES:
        all_funcs = user_api_dict[prefix]
        if not all_funcs:
            continue
        
        wrapped = all_funcs & used_user_api
        not_wrapped = all_funcs - used_user_api
        
        subsystem = {
            'prefix': prefix,
            'total': len(all_funcs),
            'wrapped_count': len(wrapped),
            'wrapped': sorted(list(wrapped)),
            'not_wrapped': sorted(list(not_wrapped))
        }
        
        report['summary']['total_user_api'] += len(all_funcs)
        report['summary']['wrapped'] += len(wrapped)
        report['summary']['not_wrapped'] += len(not_wrapped)
        
        report['subsystems'][prefix.rstrip('_')] = subsystem
    
    # Enum coverage
    for prefix in ENUM_PREFIXES:
        all_enums = enums[prefix]
        if not all_enums:
            continue
        
        used = all_enums & used_enums
        not_used = all_enums - used_enums
        
        enum_data = {
            'prefix': prefix,
            'total': len(all_enums),
            'used_count': len(used),
            'used': sorted(list(used)),
            'not_used': sorted(list(not_used))
        }
        
        report['summary']['total_enums'] += len(all_enums)
        report['summary']['enums_used'] += len(used)
        
        report['enums'][prefix.rstrip('_')] = enum_data
    
    # Calculate percentages
    total_funcs = report['summary']['total_user_api']
    if total_funcs > 0:
        report['summary']['function_coverage_percent'] = round(
            100 * report['summary']['wrapped'] / total_funcs, 1
        )
    else:
        report['summary']['function_coverage_percent'] = 0
    
    # Raw coverage percentage
    if total_raw > 0:
        report['summary']['raw_coverage_percent'] = round(
            100 * report['summary']['wrapped'] / total_raw, 1
        )
    else:
        report['summary']['raw_coverage_percent'] = 0
    
    total_enums = report['summary']['total_enums']
    if total_enums > 0:
        report['summary']['enum_coverage_percent'] = round(
            100 * report['summary']['enums_used'] / total_enums, 1
        )
    else:
        report['summary']['enum_coverage_percent'] = 0
    
    return report


def print_report(report: Dict):
    """Print human-readable report."""
    summary = report['summary']
    
    print(f"\n{'='*60}")
    print(f"LVGL C API Coverage Report")
    print(f"{'='*60}")
    
    violations = summary['internal_api_violations']
    if violations:
        print(f"\n⚠️  INTERNAL API VIOLATIONS ({len(violations)}):")
        for func in violations:
            print(f"    - {func}")
    else:
        print(f"\n✅ No internal API violations detected")
    
    print(f"\nFunction Coverage:")
    print(f"  Idiomatic C API:       {summary['total_user_api']} (Filtered {summary['non_idiomatic_count']} non-idiomatic)")
    print(f"  Called by lvgl_cpp:    {summary['wrapped']}")
    print(f"  Idiomatic Coverage:    {summary['function_coverage_percent']}%")
    print(f"  Raw C API Coverage:    {summary['raw_coverage_percent']}%")
    
    print(f"\nEnum/Constant Coverage:")
    print(f"  Total enums:           {summary['total_enums']}")
    print(f"  Used by lvgl_cpp:      {summary['enums_used']}")
    print(f"  Coverage:              {summary['enum_coverage_percent']}%")
    
    print(f"\n{'-'*60}")
    print(f"Subsystem Details (Functions):")
    print(f"{'-'*60}")
    
    for name, data in sorted(report['subsystems'].items(), key=lambda x: -x[1]['wrapped_count']):
        wrapped_count = data['wrapped_count']
        total = data['total']
        pct = round(100 * wrapped_count / total, 1) if total > 0 else 0
        print(f"  {name}: {wrapped_count}/{total} ({pct}%)")
    
    print(f"\n{'-'*60}")
    print(f"Enum Categories:")
    print(f"{'-'*60}")
    
    for name, data in sorted(report['enums'].items(), key=lambda x: -x[1]['used_count']):
        used_count = data['used_count']
        total = data['total']
        pct = round(100 * used_count / total, 1) if total > 0 else 0
        print(f"  {name}: {used_count}/{total} ({pct}%)")


def main():
    if len(sys.argv) >= 3:
        lvgl_path = Path(sys.argv[1])
        cpp_path = Path(sys.argv[2])
    else:
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
    
    report = generate_report(lvgl_path, cpp_path)
    print_report(report)
    
    # Save JSON report
    report_path = cpp_path / 'docs' / 'api_coverage.json'
    report_path.parent.mkdir(parents=True, exist_ok=True)
    with open(report_path, 'w') as f:
        json.dump(report, f, indent=2)
    print(f"\nJSON report saved to: {report_path}")
    
    if report['summary']['internal_api_violations']:
        sys.exit(1)


if __name__ == '__main__':
    main()
