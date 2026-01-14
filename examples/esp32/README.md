# ESP32 C++ Example

This directory contains an example implementation of `lvgl_cpp` for the ESP32 platform, specifically updated for recent C++ Wrapper APIs along with ESP-IDF v5.x.

## Description

`hello_world.cpp` demonstrates:
-   Initialization of an SSD1306 (or similar) OLED display using `esp_lcd`.
-   Integration of `lvgl_cpp` wrappers within an ESP-IDF `app_main` context.
-   Use of the `lvgl::Display` class to set up buffers, color formats, and flush callbacks (using C++ lambdas).
-   Use of `lvgl::Label` and `lvgl::Animation` (with builder pattern) to create a simple animated "Hello, world!" label.
-   Proper memory management of C++ wrappers with explicit ownership transfer (`release()`).

## Usage

This file is intended to replace the `main.c` (or `main.cpp`) of a standard ESP-IDF project. It requires:
1.  **ESP-IDF v5.x**
2.  **lvgl_cpp** component registered in `CMakeLists.txt`.
3.  **lvgl** managed component.

## Key Features

-   **Lambda Callbacks**: Shows how to capture context (like `esp_lcd_panel_handle_t`) in a C++ lambda and pass it to the library's `set_flush_cb`.
-   **Wrapper Lifecycle**: Demonstrates how to create `lvgl::Display` on the heap to survive the main task scope, and how to use stack-allocated widgets with `.release()`.
-   **Animation**: Uses the fluent builder API for animation setup.
