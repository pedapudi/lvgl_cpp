# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.22)

# Point to the lvgl_cpp component (root of this repo)
get_filename_component(PROJECT_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
set(EXTRA_COMPONENT_DIRS "${PROJECT_DIR}/../../..")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
# "Trim" the build. Include the minimal set of components, main, and anything it depends on.
idf_build_set_property(MINIMAL_BUILD ON)

project(hello_world)

# Fix for missing type in LVGL headers (lv_draw_dsc_base_t)
# We apply this ONLY to the lvgl component target to avoid breaking other components
if(TARGET __idf_lvgl__lvgl)
    target_compile_options(__idf_lvgl__lvgl PUBLIC "-DLV_CONF_SKIP" "-include" "src/draw/lv_draw.h")
endif()
