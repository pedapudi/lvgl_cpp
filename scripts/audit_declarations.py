#!/usr/bin/env python3
"""
Audit lvgl_cpp headers for declarations without implementations.

This script parses .h files for method declarations and checks that
corresponding .cpp files contain matching definitions.

Usage:
  python3 scripts/audit_declarations.py [path/to/lvgl_cpp]
"""

import os
import re
import sys
from pathlib import Path
from typing import Dict, List, Tuple


def extract_class_methods_from_header(header_path: Path) -> Dict[str, List[str]]:
    """
    Extract method declarations from a header file.
    Returns a dict mapping class names to lists of method signatures.
    """
    with open(header_path, 'r') as f:
        content = f.read()

    # Remove comments
    content = re.sub(r'//.*?$', '', content, flags=re.MULTILINE)
    content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)

    classes: Dict[str, List[str]] = {}
    current_class = None

    # Simple state machine to find class definitions and their methods
    lines = content.split('\n')
    brace_depth = 0
    class_brace_depth = 0

    for line in lines:
        line = line.strip()

        # Track class definitions
        class_match = re.match(r'class\s+(\w+)', line)
        if class_match and '{' in line:
            current_class = class_match.group(1)
            classes[current_class] = []
            class_brace_depth = brace_depth + 1

        # Track brace depth
        brace_depth += line.count('{') - line.count('}')

        if current_class and brace_depth < class_brace_depth:
            current_class = None

        # Look for method declarations (but not definitions with {})
        if current_class and ';' in line and '(' in line and '{' not in line:
            # Skip constructors, destructors, operators, and deleted/defaulted
            if '=' in line and ('delete' in line or 'default' in line):
                continue
            if '~' in line:  # destructor
                continue
            if 'operator' in line:
                continue

            # Extract method name
            # Pattern: [return_type] [method_name]([params]);
            method_match = re.search(r'\b(\w+)\s*\([^)]*\)\s*(const)?\s*;', line)
            if method_match:
                method_name = method_match.group(1)
                # Skip if it's the class name (constructor)
                if method_name != current_class:
                    classes[current_class].append(method_name)

    return classes


def extract_method_definitions_from_cpp(cpp_path: Path) -> List[str]:
    """
    Extract method definitions from a cpp file.
    Returns a list of "ClassName::method_name" strings.
    """
    if not cpp_path.exists():
        return []

    with open(cpp_path, 'r') as f:
        content = f.read()

    # Remove comments
    content = re.sub(r'//.*?$', '', content, flags=re.MULTILINE)
    content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)

    definitions = []

    # Pattern: [return_type] ClassName::method_name([params]) [const] {
    pattern = r'\b(\w+)::(\w+)\s*\([^)]*\)\s*(const)?\s*{'
    for match in re.finditer(pattern, content):
        class_name = match.group(1)
        method_name = match.group(2)
        definitions.append(f"{class_name}::{method_name}")

    return definitions


def audit_directory(root_path: Path) -> List[Tuple[str, str, str]]:
    """
    Audit all header/cpp pairs in the directory.
    Returns a list of (file, class, method) tuples for unimplemented methods.
    """
    issues = []

    for header_path in root_path.rglob('*.h'):
        # Skip test files
        if 'test' in str(header_path):
            continue

        cpp_path = header_path.with_suffix('.cpp')

        classes = extract_class_methods_from_header(header_path)
        definitions = extract_method_definitions_from_cpp(cpp_path)

        for class_name, methods in classes.items():
            for method in methods:
                full_name = f"{class_name}::{method}"
                if full_name not in definitions:
                    relative_path = header_path.relative_to(root_path)
                    issues.append((str(relative_path), class_name, method))

    return issues


def main():
    if len(sys.argv) > 1:
        root_path = Path(sys.argv[1])
    else:
        # Default to the lvgl_cpp directory
        root_path = Path(__file__).parent.parent

    print(f"Auditing declarations in: {root_path}")
    print("-" * 60)

    issues = audit_directory(root_path)

    if issues:
        print(f"\nFound {len(issues)} unimplemented declaration(s):\n")
        for file, class_name, method in sorted(issues):
            print(f"  {file}: {class_name}::{method}()")
        print(f"\nTotal: {len(issues)} issues")
        sys.exit(1)
    else:
        print("\nAll declarations have implementations.")
        sys.exit(0)


if __name__ == '__main__':
    main()
