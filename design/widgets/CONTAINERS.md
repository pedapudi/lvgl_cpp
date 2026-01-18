# Container and Layout Widgets Design

## 1. Scope
This document covers widgets that primarily host other widgets:
*   `Window` (`lv_win`)
*   `TabView` (`lv_tabview`)
*   `TileView` (`lv_tileview`)
*   `Table` (`lv_table`)
*   `List` (`lv_list`)

## 2. Widget Specifications

### 2.1. TabView (`TabView`)
*   **Base**: `Widget<TabView>`
*   **Methods**:
    *   `Object add_tab(const char* name)` (Returns the page object)
    *   `TabView& set_active(uint32_t index, bool anim = true)`
    *   `uint32_t get_tab_count()`
*   **Design Note**: `add_tab` returns a proxy/container.

### 2.2. TileView (`TileView`)
*   **Base**: `Widget<TileView>`
*   **Methods**:
    *   `Object add_tile(uint8_t col, uint8_t row, lv_dir_t dir)`
    *   `TileView& set_tile(Object tile, bool anim)`
    *   `TileView& set_tile_id(uint8_t col, uint8_t row, bool anim)`

### 2.3. Window (`Window`)
*   **Base**: `Widget<Window>`
*   **Methods**:
    *   `Object add_title(const char* txt)`
    *   `Object add_button(const void* icon, int32_t btn_w)`
    *   `Object content()` (Get the content area)

### 2.4. Table (`Table`)
*   **Base**: `Widget<Table>`
*   **Methods**:
    *   `Table& cell_value(uint16_t row, uint16_t col, const char* txt)`
    *   `Table& row_count(uint16_t cnt)`
    *   `Table& col_count(uint16_t cnt)`
    *   `Table& col_width(uint16_t col, int32_t w)`

### 2.5. List (`List`)
*   **Base**: `Widget<List>`
*   **Methods**:
    *   `Object add_text(const char* txt)`
    *   `Button add_button(const void* icon, const char* txt)`

## 3. Container Logic
Containers often effectively shadow the "Screen" behavior.
The `Widget<T>` base must ensure `get_content()` helpers are easy to use for layouts.

```cpp
Window win(parent, 50);
win.title("Settings");
auto content = win.content();
Button(content).center().text("OK");
```
