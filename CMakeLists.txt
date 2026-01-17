cmake_minimum_required(VERSION 3.10)
project(lvgl_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LVGL (Assuming it's in the parent directory)
# In a real scenario, this might need more robust finding logic
# or be added as a subdirectory.

    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../lvgl/CMakeLists.txt")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lvgl lvgl_build)
    else()
        message(WARNING "LVGL not found at ../lvgl. Tests might fail to build.")
    endif()
endif()

add_executable(test_group tests/test_group.cpp)
target_link_libraries(test_group PRIVATE lvgl_cpp)


add_library(lvgl_cpp STATIC
    core/object.cpp
    core/style.cpp
    misc/color.cpp
    font/font.cpp
    misc/file_system.cpp
    core/observer.cpp
    draw/image_decoder.cpp
    draw/draw.cpp
    
    widgets/button.cpp
    widgets/label.cpp
    widgets/bar.cpp
    widgets/slider.cpp
    widgets/checkbox.cpp
    widgets/switch.cpp
    widgets/line.cpp
    widgets/arc.cpp
    widgets/image.cpp
    widgets/textarea.cpp
    widgets/spinbox.cpp
    widgets/dropdown.cpp
    widgets/roller.cpp
    widgets/button_matrix.cpp
    widgets/keyboard.cpp
    widgets/table.cpp
    widgets/canvas.cpp
    widgets/chart.cpp
    widgets/calendar.cpp
    widgets/msgbox.cpp
    widgets/menu.cpp
    widgets/tileview.cpp
    widgets/tabview.cpp
    widgets/win.cpp
    widgets/scale.cpp
    widgets/anim_image.cpp
    widgets/span.cpp
    widgets/list.cpp
    widgets/led.cpp
    widgets/image_button.cpp
    widgets/spinner.cpp
    
    core/group.cpp
    display/display.cpp
    indev/input_device.cpp
    misc/timer.cpp
    misc/animation.cpp
)

target_include_directories(lvgl_cpp PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/.. # Allow including via lvgl_cpp/...
)

# Link against LVGL
# We assume the target 'lvgl' exists (e.g. from the main LVGL CMakeLists.txt)
# If not, the user will need to link it manually.
target_link_libraries(lvgl_cpp PUBLIC lvgl)

add_executable(test_core_widgets tests/test_core_widgets.cpp)
target_link_libraries(test_core_widgets PRIVATE lvgl_cpp)

add_executable(test_input_widgets tests/test_input_widgets.cpp)
target_link_libraries(test_input_widgets PRIVATE lvgl_cpp)

add_executable(test_complex_widgets tests/test_complex_widgets.cpp)
target_link_libraries(test_complex_widgets PRIVATE lvgl_cpp)

add_executable(test_visual_widgets tests/test_visual_widgets.cpp)
target_link_libraries(test_visual_widgets PRIVATE lvgl_cpp)

add_executable(observer_test tests/observer_test.cpp)
target_link_libraries(observer_test PRIVATE lvgl_cpp)
add_executable(test_ownership tests/test_ownership.cpp)
target_link_libraries(test_ownership PRIVATE lvgl_cpp)

add_executable(test_animation tests/test_animation.cpp)
target_link_libraries(test_animation PRIVATE lvgl_cpp)

add_executable(test_fluent_api tests/test_fluent_api.cpp)
target_link_libraries(test_fluent_api PRIVATE lvgl_cpp)

add_executable(test_rotation tests/test_rotation.cpp)
target_link_libraries(test_rotation PRIVATE lvgl_cpp)
