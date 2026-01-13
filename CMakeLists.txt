cmake_minimum_required(VERSION 3.10)
project(lvgl_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LVGL (Assuming it's in the parent directory)
# In a real scenario, this might need more robust finding logic
# or be added as a subdirectory.

if(NOT TARGET lvgl)
    # If LVGL is not already a target (e.g. from parent project), we include it here.
    # This is primarily for building the tests or standalone development of lvgl_cpp.
    
    # Use the test configuration for internal builds/tests
    set(LV_BUILD_CONF_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tests/lv_conf_test.h CACHE STRING "Path to lv_conf.h" FORCE)
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../lvgl/CMakeLists.txt")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lvgl lvgl_build)
    else()
        message(WARNING "LVGL not found at ../lvgl. Tests might fail to build.")
    endif()
endif()

add_library(lvgl_cpp STATIC
    core/object.cc
    core/style.cc
    misc/color.cc
    font/font.cc
    misc/file_system.cc
    core/observer.cc
    draw/image_decoder.cc
    draw/draw.cc
    
    widgets/button.cc
    widgets/label.cc
    widgets/bar.cc
    widgets/slider.cc
    widgets/checkbox.cc
    widgets/switch.cc
    widgets/line.cc
    widgets/arc.cc
    widgets/image.cc
    widgets/textarea.cc
    widgets/spinbox.cc
    widgets/dropdown.cc
    widgets/roller.cc
    widgets/button_matrix.cc
    widgets/keyboard.cc
    widgets/table.cc
    widgets/canvas.cc
    widgets/chart.cc
    widgets/calendar.cc
    widgets/msgbox.cc
    widgets/menu.cc
    widgets/tileview.cc
    widgets/tabview.cc
    widgets/win.cc
    widgets/scale.cc
    widgets/anim_image.cc
    widgets/span.cc
    widgets/list.cc
    widgets/led.cc
    widgets/image_button.cc
    widgets/spinner.cc
    
    core/group.cc
    display/display.cc
    indev/input_device.cc
    misc/timer.cc
    misc/animation.cc
)

target_include_directories(lvgl_cpp PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/.. # Allow including via lvgl_cpp/...
)

# Link against LVGL
# We assume the target 'lvgl' exists (e.g. from the main LVGL CMakeLists.txt)
# If not, the user will need to link it manually.
target_link_libraries(lvgl_cpp PUBLIC lvgl)

add_executable(test_core_widgets tests/test_core_widgets.cc)
target_link_libraries(test_core_widgets PRIVATE lvgl_cpp)

add_executable(test_input_widgets tests/test_input_widgets.cc)
target_link_libraries(test_input_widgets PRIVATE lvgl_cpp)

add_executable(test_complex_widgets tests/test_complex_widgets.cc)
target_link_libraries(test_complex_widgets PRIVATE lvgl_cpp)

add_executable(test_visual_widgets tests/test_visual_widgets.cc)
target_link_libraries(test_visual_widgets PRIVATE lvgl_cpp)

add_executable(observer_test tests/observer_test.cc)
target_link_libraries(observer_test PRIVATE lvgl_cpp)
