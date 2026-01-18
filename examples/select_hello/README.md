# Select Hello Example

This example demonstrates a simple interactive menu using a Roller and screen transitions with `lvgl_cpp`.

## Features
- Interactive Roller with multiple "Hello" options.
- Screen transition to a detailed "Hello World" view.
- Animated label and screen cleanup.
- Support for SSD1306 I2C displays (configured in `hw/`).

## Project Structure
- `ui/`: Contains the UI logic (`SelectHello` and `HelloWorld`).
- `hw/`: Hardware specific initialization (SSD1306).
- `sys/`: LVGL porting/system interface.
- `main/`: Main entry point and build configuration.
