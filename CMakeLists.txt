cmake_minimum_required(VERSION 3.10)

set(SOURCES
    core/object.cpp
    core/event.cpp
    core/style.cpp
    misc/color.cpp
    font/font.cpp
    misc/file_system.cpp
    core/observer.cpp
    draw/image_decoder.cpp
    draw/draw.cpp
    draw/draw_task.cpp
    
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
    widgets/screen.cpp
    
    core/group.cpp
    display/display.cpp
    indev/input_device.cpp
    misc/timer.cpp
    misc/animation.cpp
    misc/animation_timeline.cpp
)

if(IDF_TARGET)
    idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS "." ".."
                        REQUIRES lvgl)
else()
    project(lvgl_cpp)

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    # Find LVGL (Assuming it's in the parent directory)
    if(NOT TARGET lvgl)
        # Use the test configuration for internal builds/tests
        set(LV_BUILD_CONF_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tests/lv_conf_test.h CACHE STRING "Path to lv_conf.h" FORCE)
        set(CONFIG_LV_BUILD_EXAMPLES ON CACHE BOOL "Build examples" FORCE)
        set(CONFIG_LV_BUILD_DEMOS ON CACHE BOOL "Build demos" FORCE)
        set(CONFIG_LV_USE_THORVG_INTERNAL OFF CACHE BOOL "Use ThorVG" FORCE)

        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../lvgl/CMakeLists.txt")
            add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lvgl lvgl_build)
        else()
            message(WARNING "LVGL not found at ../lvgl. Tests might fail to build.")
        endif()
    endif()

    add_library(lvgl_cpp STATIC ${SOURCES})

    target_include_directories(lvgl_cpp PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/.. 
    )

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

    add_executable(test_group tests/test_group.cpp)
    target_link_libraries(test_group PRIVATE lvgl_cpp)

    add_executable(test_rotation tests/test_rotation.cpp)
    target_link_libraries(test_rotation PRIVATE lvgl_cpp)

    add_executable(test_event_system tests/test_event_system.cpp)
    target_link_libraries(test_event_system PRIVATE -Wl,--start-group lvgl_cpp lvgl -Wl,--end-group)

    add_executable(test_draw_task tests/test_draw_task.cpp)
    target_link_libraries(test_draw_task PRIVATE lvgl_cpp)



    add_executable(test_input_device tests/test_input_device.cpp)
    target_link_libraries(test_input_device PRIVATE lvgl_cpp)

    add_executable(test_file_system tests/test_file_system.cpp)
    target_link_libraries(test_file_system PRIVATE lvgl_cpp)

    add_executable(test_geometry tests/test_geometry.cpp)
    target_link_libraries(test_geometry PRIVATE lvgl_cpp)

    add_executable(test_enums tests/test_enums.cpp)
    target_link_libraries(test_enums PRIVATE lvgl_cpp)

    add_executable(test_screen tests/test_screen.cpp)
    target_link_libraries(test_screen PRIVATE lvgl_cpp)

    add_executable(test_animation_timeline tests/test_animation_timeline.cpp)
    target_link_libraries(test_animation_timeline PRIVATE lvgl_cpp)

    enable_testing()
    add_test(NAME test_core_widgets COMMAND test_core_widgets)
    add_test(NAME test_input_widgets COMMAND test_input_widgets)
    add_test(NAME test_complex_widgets COMMAND test_complex_widgets)
    add_test(NAME test_visual_widgets COMMAND test_visual_widgets)
    add_test(NAME observer_test COMMAND observer_test)
    add_test(NAME test_ownership COMMAND test_ownership)
    add_test(NAME test_animation COMMAND test_animation)
    add_test(NAME test_fluent_api COMMAND test_fluent_api)
    add_test(NAME test_group COMMAND test_group)
    add_test(NAME test_rotation COMMAND test_rotation)
    add_test(NAME test_event_system COMMAND test_event_system)
    add_test(NAME test_draw_task COMMAND test_draw_task)
    add_test(NAME test_file_system COMMAND test_file_system)
    add_test(NAME test_geometry COMMAND test_geometry)
    add_test(NAME test_enums COMMAND test_enums)
    add_test(NAME test_screen COMMAND test_screen)
    add_test(NAME test_input_device COMMAND test_input_device)
    add_test(NAME test_animation_timeline COMMAND test_animation_timeline)
endif()

# ==============================================================================
# Benchmarks & Profiling
# ==============================================================================
option(ENABLE_PROFILING "Enable gperftools profiling linkage" OFF)

if(ENABLE_PROFILING)
    find_library(PROFILER_LIB profiler)
    find_library(TCMALLOC_LIB tcmalloc)
    if(PROFILER_LIB AND TCMALLOC_LIB)
        message(STATUS "Profiling enabled: Found ${PROFILER_LIB} and ${TCMALLOC_LIB}")
    else()
        message(WARNING "Profiling requested but libraries not found.")
    endif()
endif()

# Helper to add a benchmark executable
function(add_benchmark NAME SOURCES)
    add_executable(${NAME} ${SOURCES})
    target_link_libraries(${NAME} PRIVATE lvgl_cpp) # Links lvgl internally
    
    # Pure C benchmarks might need direct lvgl link if they don't use the C++ lib,
    # but linking lvgl_cpp pulls in lvgl transitively, so it should be fine.
    # However, strictly C benchmarks should probably link `lvgl` directly to avoid C++ runtime overlap if possible,
    # though for comparison we want the environments to be similar.
    # Actually, for C benchmarks, let's link just lvgl to be "Pure".
    
    if(NAME MATCHES "_c$")
        target_link_libraries(${NAME} PRIVATE lvgl)
    endif()

    if(ENABLE_PROFILING AND PROFILER_LIB)
        target_link_libraries(${NAME} PRIVATE ${PROFILER_LIB} ${TCMALLOC_LIB})
    endif()
endfunction()

# 1. Baseline (creation cost)
add_benchmark(bench_baseline_c   tests/bench_baseline.c)
add_benchmark(bench_baseline_cpp tests/bench_baseline.cpp)

# 2. Events (callback cost)
add_benchmark(bench_events_c   tests/bench_events.c)
add_benchmark(bench_events_cpp tests/bench_events.cpp)

# 3. Stability (churn)
add_benchmark(bench_churn tests/bench_churn.cpp)

# 4. Fragmentation
add_benchmark(bench_fragmentation tests/bench_fragmentation.cpp)

