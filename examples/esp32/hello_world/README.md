# ESP32-C3 LVGL C++ Example

This is a complete ESP-IDF project template demonstrating how to use `lvgl_cpp` with an ESP32-C3 and SSD1306 display.

## Hardware
-   **Board**: ESP32-C3 (e.g., Seeed Studio XIAO)
-   **Display**: SSD1306 I2C (SDA: GPIO 6, SCL: GPIO 7)

## Architecture
This example helps demonstrate a clean separation of concerns:
-   `main/hw`: Hardware initialization (I2C, SSD1306).
-   `main/sys`: System layer (LVGL port, Ticks, Task).
-   `main/ui`: User Interface logic using `lvgl_cpp`.

## Build & Flash
1.  Set up ESP-IDF environment.
2.  Run `idf.py build`.
3.  Run `idf.py flash monitor`.

## Logic
The example displays scrolling text and executes a simple vertical animation using the C++ wrapper classes.
