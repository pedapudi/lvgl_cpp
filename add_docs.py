import os
import re

widgets_dir = "/home/sunil/code/git/lvgl/lvgl/lvgl_cpp/widgets"

def get_class_name(content):
    match = re.search(r'class\s+(\w+)\s*:', content)
    if match:
        return match.group(1)
    return None

def generate_header_doc(filename, class_name):
    return f"""/**
 * @file {filename}
 * @brief C++ Wrapper for LVGL {class_name} Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::{class_name} widget(lv_screen_active());
 * widget.center();
 * ```
 */
"""

def generate_method_doc(line, class_name):
    if f"explicit {class_name}(Object *parent);" in line:
        return f"""  /**
   * @brief Create a {class_name} with a parent.
   * @param parent The parent object.
   */
"""
    if f"{class_name}();" in line:
        return f"""  /**
   * @brief Create a {class_name} on the active screen.
   */
"""
    if f"explicit {class_name}(lv_obj_t *obj);" in line:
        return f"""  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
"""
    return ""

def process_file(filepath):
    with open(filepath, 'r') as f:
        lines = f.readlines()
    
    content = "".join(lines)
    filename = os.path.basename(filepath)
    class_name = get_class_name(content)
    
    if not class_name:
        return

    new_lines = []
    
    # Check if file has header guard
    header_guard_found = False
    
    for i, line in enumerate(lines):
        # Insert File Doc after includes or define
        if not header_guard_found and "#define" in line:
            new_lines.append(line)
            new_lines.append("\n") # Spacer
            # Skip includes for now, insert doc before namespace?
            header_guard_found = True
            continue
            
        if "namespace lvgl" in line:
             # Insert top level doc before namespace if not present
             if "/**" not in "".join(lines[max(0, i-10):i]):
                 new_lines.append(generate_header_doc(filename, class_name))
             new_lines.append(line)
             continue
        
        # Insert method docs
        doc = generate_method_doc(line, class_name)
        if doc and "/**" not in "".join(lines[max(0, i-5):i]): # Avoid double doc
            new_lines.append(doc)
            
        new_lines.append(line)

    with open(filepath, 'w') as f:
        f.write("".join(new_lines))
    print(f"Processed {filepath}")

for filename in os.listdir(widgets_dir):
    if filename.endswith(".h") and filename != "button.h": # Skip button as we did it manually
        process_file(os.path.join(widgets_dir, filename))
