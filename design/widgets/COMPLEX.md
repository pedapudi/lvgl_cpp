# Complex and menu widgets design

## 1. Scope
This document covers complex widgets that don't fit into simple Input/Output categories:
*   `Menu` (`lv_menu`)
*   `Calendar` (`lv_calendar`)
*   `Chart` (`lv_chart`)
*   `Roller` (`lv_roller`)
*   `Dropdown` (`lv_dropdown`)

## 2. Widget specifications

### 2.1. Menu (`Menu`)
*   **Base**: `Widget<Menu>`
*   **Methods**:
    *   `Menu& set_page(Object page)`
    *   `Menu& set_sidebar_page(Object page)`
    *   `Object page_create(Object parent, const char* title)`

### 2.2. Calendar (`Calendar`)
*   **Base**: `Widget<Calendar>`
*   **Methods**:
    *   `Calendar& set_today_date(uint32_t y, uint32_t m, uint32_t d)`
    *   `Calendar& set_showed_date(uint32_t y, uint32_t m)`
    *   `Calendar& header_dropdown()` (Install header)
    *   `Calendar& header_arrow()` (Install header)

### 2.3. Chart (`Chart`)
*   **Base**: `Widget<Chart>`
*   **Proxies**:
    *   `ChartSeries`: Wraps `lv_chart_series_t`. Created via `add_series()`.
    *   `ChartCursor`: Wraps `lv_chart_cursor_t`. Created via `add_cursor()`.
*   **Methods**:
    *   `Chart& type(Type)`
    *   `Chart& point_count(uint16_t)`
    *   `ChartSeries add_series(Color c, Axis axis)`
    *   `ChartCursor add_cursor(Color c, Dir dir)`
    *   `Chart& set_next_value(ChartSeries& s, int32_t v)`

### 2.4. Scale (`Scale`)
*   **Base**: `Widget<Scale>`
*   **Proxies**:
    *   `ScaleSection`: Wraps `lv_scale_section_t`. Created via `add_section()`.
*   **Methods**:
    *   `Scale& set_range(int32_t min, int32_t max)`
    *   `Scale& set_mode(lv_scale_mode_t mode)`
    *   `ScaleSection add_section()`
    *   `ScaleSection& set_range(int32_t min, int32_t max)`

### 2.5. Roller (`Roller`)
*   **Base**: `Widget<Roller>`
*   **Methods**:
    *   `Roller& options(const char* opts, lv_roller_mode_t mode)`
    *   `Roller& selected(uint16_t sel, bool anim)`
    *   `uint16_t get_selected()`
    *   `std::string get_selected_str()`

### 2.6. Dropdown (`Dropdown`)
*   **Base**: `Widget<Dropdown>`
*   **Methods**:
    *   `Dropdown& options(const char* opts)`
    *   `Dropdown& add_option(const char* opt, uint32_t pos)`
    *   `Dropdown& selected(uint16_t sel)`
    *   `uint16_t get_selected()`
    *   `std::string get_selected_str()`

