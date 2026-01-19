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
    draw/image_descriptor.cpp
    
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

    # Profiling Support
    option(ENABLE_PROFILING "Enable gperftools profiling" OFF)
    if(ENABLE_PROFILING)
        find_library(LIBPROFILER profiler)
        find_library(LIBTCMALLOC tcmalloc)
        
        if(LIBPROFILER)
            message(STATUS "Found libprofiler: ${LIBPROFILER}")
            set(GPERFTOOLS_LIBRARIES ${LIBPROFILER})
            
            if(LIBTCMALLOC)
                 message(STATUS "Found libtcmalloc: ${LIBTCMALLOC}")
                 set(TCMALLOC_FOUND ON)
                 set(TCMALLOC_LIBRARIES ${LIBTCMALLOC})
            endif()
        else()
            message(WARNING "libprofiler not found. Profiling disabled.")
            set(ENABLE_PROFILING OFF)
        endif()
    endif()

    function(add_benchmark target_name source_file)
        add_executable(${target_name} ${source_file})
        target_link_libraries(${target_name} PRIVATE lvgl_cpp)
        if(ENABLE_PROFILING)
            target_compile_definitions(${target_name} PRIVATE ENABLE_PROFILING)
            target_link_libraries(${target_name} PRIVATE ${GPERFTOOLS_LIBRARIES})
            if(TCMALLOC_FOUND)
                target_link_libraries(${target_name} PRIVATE ${TCMALLOC_LIBRARIES})
            endif()
        endif()
    endfunction()

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
    add_benchmark(bench_baseline tests/bench_baseline.cpp)
    add_benchmark(bench_baseline_c tests/bench_baseline.c)
    add_benchmark(bench_events tests/bench_events.cpp)
    add_benchmark(bench_events_c tests/bench_events.c)
    add_benchmark(bench_churn tests/bench_churn.cpp)
    add_benchmark(bench_churn_c tests/bench_churn.c) # New C Baseline
    add_benchmark(bench_fragmentation tests/bench_fragmentation.cpp)
    add_benchmark(bench_fragmentation_c tests/bench_fragmentation.c) # New C Baseline
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
    # add_test(NAME test_screen COMMAND test_screen) <--- Already added above effectively? 
    # Actually, looking at the file content in previous turns, it seems I appended duplicates.
    # The safest way is to replace the whole block I added/messed up.
    
    add_executable(test_tabview tests/test_tabview.cpp)
    target_link_libraries(test_tabview PRIVATE lvgl_cpp)
    add_test(NAME test_tabview COMMAND test_tabview)

    

add_executable(test_image tests/test_image.cpp widgets/image.cpp draw/image_descriptor.cpp core/object.cpp display/display.cpp)
target_link_libraries(test_image PRIVATE lvgl_cpp)
add_test(NAME test_image COMMAND test_image)



add_executable(test_msgbox_win tests/test_msgbox_win.cpp widgets/msgbox.cpp widgets/win.cpp widgets/button.cpp widgets/label.cpp core/object.cpp misc/timer.cpp)
target_link_libraries(test_msgbox_win PRIVATE lvgl_cpp)
add_test(NAME test_msgbox_win COMMAND test_msgbox_win)


add_executable(test_animation_timeline tests/test_animation_timeline.cpp misc/animation_timeline.cpp misc/animation.cpp widgets/button.cpp core/object.cpp misc/timer.cpp)
target_link_libraries(test_animation_timeline PRIVATE lvgl_cpp)
add_test(NAME test_animation_timeline COMMAND test_animation_timeline)

add_executable(test_menu tests/test_menu.cpp widgets/menu.cpp widgets/button.cpp widgets/label.cpp core/object.cpp display/display.cpp)
target_link_libraries(test_menu PRIVATE lvgl_cpp)
add_test(NAME test_menu COMMAND test_menu)







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
    add_test(NAME test_callbacks COMMAND test_callbacks)
    add_test(NAME test_input_device COMMAND test_input_device)
endif()
