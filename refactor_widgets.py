import os
import re

widgets_dir = "/home/sunil/code/git/lvgl/lvgl/lvgl_cpp/widgets"

def process_header(filepath):
    with open(filepath, 'r') as f:
        content = f.read()

    # Find definition like: class Button : ...
    class_match = re.search(r'class\s+(\w+)\s*:', content)
    if not class_match:
        return
    
    class_name = class_match.group(1)
    
    # Pattern: explicit ClassName(Object *parent = nullptr);
    pattern = f'explicit {class_name}\\(Object \\*parent = nullptr\\);'
    
    if re.search(pattern, content):
        replacement = f'{class_name}();\n  explicit {class_name}(Object *parent);'
        new_content = re.sub(pattern, replacement, content)
        with open(filepath, 'w') as f:
            f.write(new_content)
        print(f"Updated header: {filepath}")

def process_source(filepath):
    with open(filepath, 'r') as f:
        content = f.read()
    
    filename = os.path.basename(filepath)
    header_name = filename.replace('.cc', '.h')
    
    # Infer class name from filename usually matching or look in file
    # Simple heuristic: look for ClassName::ClassName(Object
    match = re.search(r'(\w+)::\1\(Object', content)
    if not match:
        return
    
    class_name = match.group(1)
    
    # Check if we already have the default constructor
    if f'{class_name}::{class_name}()' in content:
        return
        
    # check namespace
    if "namespace lvgl {" in content:
        # Insert after namespace lvgl {
        insertion = f'\n\n{class_name}::{class_name}() : {class_name}((Object*)nullptr) {{}}'
        new_content = content.replace('namespace lvgl {', 'namespace lvgl {' + insertion, 1)
        with open(filepath, 'w') as f:
            f.write(new_content)
        print(f"Updated source: {filepath}")

for filename in os.listdir(widgets_dir):
    filepath = os.path.join(widgets_dir, filename)
    if filename.endswith(".h"):
        process_header(filepath)
    elif filename.endswith(".cc"):
        process_source(filepath)
