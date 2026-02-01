cmake_minimum_required(VERSION 3.10)

set(SOURCES
    core/object.cpp
    core/event.cpp
    core/event_proxy.cpp
    core/group_proxy.cpp
    misc/color.cpp
    misc/style.cpp
    font/font.cpp
    font/owned_font.cpp
    misc/file_system.cpp
    core/observer.cpp
    core/interaction_proxy.cpp
    core/tree_proxy.cpp
)

set(DRAW_SOURCES
    draw/draw.cpp
    draw/draw_buf.cpp
    draw/draw_task.cpp
    draw/image_decoder.cpp
    draw/image_descriptor.cpp
)

list(APPEND SOURCES ${DRAW_SOURCES})

set(WIDGET_SOURCES
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
    widgets/lottie.cpp
    
    core/group.cpp
    display/display.cpp
    indev/input_device.cpp
    misc/timer.cpp
    misc/animation.cpp
    misc/animation_timeline.cpp
    misc/async.cpp
    misc/log.cpp
    misc/theme.cpp
    misc/vector.cpp
)
list(APPEND SOURCES ${WIDGET_SOURCES})

if(IDF_TARGET)
    idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS "." ".."
                        REQUIRES lvgl)
    target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-cast-function-type)
    target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_20)
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
        if(source_file MATCHES "\\.cpp$")
            target_link_libraries(${target_name} PRIVATE lvgl_cpp)
        else()
            target_link_libraries(${target_name} PRIVATE lvgl)
        endif()
        
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

    add_executable(test_style_reconciliation tests/test_style_reconciliation.cpp)
target_link_libraries(test_style_reconciliation PRIVATE lvgl_cpp)

add_executable(test_issue_161 tests/test_issue_161.cpp)
target_link_libraries(test_issue_161 PRIVATE lvgl_cpp)

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
    add_benchmark(bench_churn_stability tests/bench_churn_stability.cpp) # Stability Test
    add_benchmark(bench_widgets tests/bench_widgets.cpp) # Generic Widget Test
    add_benchmark(bench_widgets_c tests/bench_widgets.c) # C version

    # Soak Test
    add_custom_target(bench_soak
        COMMAND bench_churn --duration 3600
        DEPENDS bench_churn
        COMMENT "Running Churn Benchmark for 1 hour..."
        USES_TERMINAL
    )
    add_executable(test_rotation tests/test_rotation.cpp)
    target_link_libraries(test_rotation PRIVATE lvgl_cpp)

    add_executable(test_event_system tests/test_event_system.cpp)
    target_link_libraries(test_event_system PRIVATE -Wl,--start-group lvgl_cpp lvgl -Wl,--end-group)

    add_executable(test_draw_task tests/test_draw_task.cpp)
    target_link_libraries(test_draw_task PRIVATE lvgl_cpp)



    add_executable(test_chart_table tests/test_chart_table.cpp)
    target_link_libraries(test_chart_table PRIVATE lvgl_cpp)
    add_test(NAME test_chart_table COMMAND test_chart_table)

    add_executable(test_input_device tests/test_input_device.cpp)
    target_link_libraries(test_input_device PRIVATE lvgl_cpp)

    add_executable(test_file_system tests/test_file_system.cpp)
    target_link_libraries(test_file_system PRIVATE lvgl_cpp)

    add_executable(test_geometry tests/test_geometry.cpp)
    target_link_libraries(test_geometry PRIVATE lvgl_cpp)

    
    add_executable(test_callbacks tests/test_callbacks.cpp)
    target_link_libraries(test_callbacks PRIVATE lvgl_cpp)

    add_executable(test_screen tests/test_screen.cpp)
    target_link_libraries(test_screen PRIVATE lvgl_cpp)
    # add_test(NAME test_screen COMMAND test_screen) <--- Already added above effectively? 
    # Actually, looking at the file content in previous turns, it seems I appended duplicates.
    # The safest way is to replace the whole block I added/messed up.
    
    add_executable(test_tabview tests/test_tabview.cpp)
    target_link_libraries(test_tabview PRIVATE lvgl_cpp)
    add_test(NAME test_tabview COMMAND test_tabview)

    add_executable(test_span tests/test_span.cpp)
    target_link_libraries(test_span PRIVATE lvgl_cpp)
    add_test(NAME test_span COMMAND test_span)

    add_executable(test_api_hardening tests/test_api_hardening.cpp)
    target_link_libraries(test_api_hardening PRIVATE lvgl_cpp)
    add_test(NAME test_api_hardening COMMAND test_api_hardening)

    

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
    add_test(NAME test_screen COMMAND test_screen)
    add_test(NAME test_input_device COMMAND test_input_device)

    add_executable(test_lottie tests/test_lottie.cpp)
    target_link_libraries(test_lottie PRIVATE lvgl_cpp)
    add_test(NAME test_lottie COMMAND test_lottie)

    add_executable(test_vector tests/test_vector.cpp)
    target_link_libraries(test_vector PRIVATE lvgl_cpp)
    add_test(NAME test_vector COMMAND test_vector)

    add_executable(test_api_expansion    tests/test_api_expansion.cpp
)
    target_link_libraries(test_api_expansion PRIVATE lvgl_cpp)
    add_test(NAME test_api_expansion COMMAND test_api_expansion)

    add_executable(test_obj_proxies tests/test_obj_proxies.cpp)
    target_link_libraries(test_obj_proxies PRIVATE lvgl_cpp)
    add_test(NAME test_obj_proxies COMMAND test_obj_proxies)


    add_executable(test_fluent_styles tests/test_fluent_styles.cpp widgets/button.cpp core/object.cpp misc/timer.cpp display/display.cpp font/font.cpp)
    target_link_libraries(test_fluent_styles PRIVATE lvgl_cpp)
    add_test(NAME test_fluent_styles COMMAND test_fluent_styles)

    add_executable(test_enums tests/test_enums.cpp widgets/button.cpp widgets/label.cpp core/object.cpp misc/timer.cpp display/display.cpp)
    target_link_libraries(test_enums PRIVATE lvgl_cpp)
    add_test(NAME test_enums COMMAND test_enums)

    add_executable(test_font tests/test_font.cpp font/font.cpp font/owned_font.cpp core/object.cpp misc/timer.cpp)
    target_link_libraries(test_font PRIVATE lvgl_cpp)
    add_test(NAME test_font COMMAND test_font)

    add_executable(test_async tests/test_async.cpp)
    target_link_libraries(test_async PRIVATE lvgl_cpp)
    add_test(NAME test_async COMMAND test_async)

    add_executable(test_log tests/test_log.cpp)
    target_link_libraries(test_log PRIVATE lvgl_cpp)
    add_test(NAME test_log COMMAND test_log)

    add_executable(test_timer tests/test_timer.cpp)
    target_link_libraries(test_timer PRIVATE lvgl_cpp)
    add_test(NAME test_timer COMMAND test_timer)

    add_executable(test_style_proxy tests/test_style_proxy.cpp)
    target_link_libraries(test_style_proxy PRIVATE lvgl_cpp)
    add_test(NAME test_style_proxy COMMAND test_style_proxy)

    add_executable(test_layout_proxy tests/test_layout_proxy.cpp)
    target_link_libraries(test_layout_proxy PRIVATE lvgl_cpp)
    add_test(NAME test_layout_proxy COMMAND test_layout_proxy)

    add_executable(test_scroll_proxy tests/test_scroll_proxy.cpp)
    target_link_libraries(test_scroll_proxy PRIVATE lvgl_cpp)
    add_test(NAME test_scroll_proxy COMMAND test_scroll_proxy)

    add_executable(test_scale tests/test_scale.cpp)
    target_link_libraries(test_scale PRIVATE lvgl_cpp)
    add_test(NAME test_scale COMMAND test_scale)

    add_executable(test_constants tests/test_constants.cpp)
    target_link_libraries(test_constants PRIVATE lvgl_cpp)
    add_test(NAME test_constants COMMAND test_constants)

    add_executable(test_cast tests/test_cast.cpp)
    target_link_libraries(test_cast PRIVATE lvgl_cpp)
    add_test(NAME test_cast COMMAND test_cast)

    add_executable(test_layout tests/test_layout.cpp)
    target_link_libraries(test_layout PRIVATE lvgl_cpp)
    add_test(NAME test_layout COMMAND test_layout)

    add_executable(test_coverage_sweep tests/test_coverage_sweep.cpp)
    target_link_libraries(test_coverage_sweep PRIVATE lvgl_cpp)
    add_test(NAME test_coverage_sweep COMMAND test_coverage_sweep)

    add_executable(test_size tests/test_size.cpp)
    target_link_libraries(test_size PRIVATE lvgl_cpp)
    add_test(NAME test_size COMMAND test_size)



    # --- New Benchmarking Framework v2 ---
    add_executable(bench_suite
        bench/bench_main.cpp
        bench/bench.cpp
        bench/bench_widgets.cpp
        bench/bench_expanded.cpp
    )
    target_link_libraries(bench_suite PRIVATE lvgl_cpp)
    
    if(ENABLE_PROFILING)
        target_compile_definitions(bench_suite PRIVATE ENABLE_PROFILING)
        target_link_libraries(bench_suite PRIVATE ${GPERFTOOLS_LIBRARIES})
        if(TCMALLOC_FOUND)
            target_link_libraries(bench_suite PRIVATE ${TCMALLOC_LIBRARIES})
        endif()
    endif()

    # Header Integrity Check (Regression test for self-contained headers)
    find_package(Python3 REQUIRED)
    add_test(NAME test_header_integrity 
             COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check_headers.py)
endif()
