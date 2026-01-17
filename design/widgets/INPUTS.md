# Input Widgets Design

## 1. Scope
This document covers widgets primarily used for user input:
*   `Button` (`lv_button`)
*   `Checkbox` (`lv_checkbox`)
*   `Switch` (`lv_switch`)
*   `Slider` (`lv_slider`)
*   `Roller` (`lv_roller`)
*   `Dropdown` (`lv_dropdown`)
*   `Textarea` (`lv_textarea`)

## 2. Common Patterns
All input widgets inherit from `Widget<T>`. Most inputs share "Value" semantics.

### `Valued<T, ValueType>` Mixin?
We might consider a `Valued` mixin for `set_value` / `get_value`, but LVGL behavior varies (int32 for slider, bool for switch, string for text area). It is better to implement specific methods.

## 3. Widget Specifications

### 3.1. Button (`Button`)
*   **Base**: `Widget<Button>`
*   **Extra Features**: None (Standard container).
*   **Usage**:
    ```cpp
    Button(parent).size(100,50).center().on_click(cb);
    ```

### 3.2. Checkbox (`Checkbox`)
*   **Base**: `Widget<Checkbox>`
*   **Methods**:
    *   `Checkbox& text(const char*)` (Fluent setter for label).
    *   `Checkbox& checked(bool)` (Set state).
    *   `bool is_checked()`
*   **Usage**:
    ```cpp
    Checkbox(parent).text("Enable WiFi").checked(true).on_change(cb);
    ```

### 3.3. Switch (`Switch`)
*   **Base**: `Widget<Switch>`
*   **Methods**:
    *   `Switch& on(bool anim = true)`
    *   `Switch& off(bool anim = true)`
    *   `Switch& toggle(bool anim = true)`
    *   `bool is_on()`

### 3.4. Slider (`Slider`)
*   **Base**: `Widget<Slider>`
*   **Methods**:
    *   `Slider& range(int32_t min, int32_t max)`
    *   `Slider& value(int32_t val, bool anim = false)`
    *   `int32_t value()`
    *   `Slider& mode(lv_slider_mode_t)`

### 3.5. Textarea (`TextArea`)
*   **Base**: `Widget<TextArea>`
*   **Methods**:
    *   `TextArea& text(const std::string&)`
    *   `TextArea& placeholder(const char*)`
    *   `TextArea& password_mode(bool)`
    *   `TextArea& one_line(bool)`
    *   `TextArea& accepted_chars(const char*)`
    *   `TextArea& max_length(uint32_t)`
    *   `TextArea& add_text(const char*)` (Append)

## 4. Implementation Strategy
These widgets will be the first wave of migration after Core. `Button` is the simplest verification target for `Widget<T>`.
