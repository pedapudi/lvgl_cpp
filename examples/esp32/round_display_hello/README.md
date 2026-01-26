# Setting up Select Hello on Seeed Studio XIAO ESP32C6

This tutorial will guide you through setting up a "Select Hello" demo on the Seeed Studio Round Display with XIAO ESP32C6. Unlike other tutorials that use the Arduino IDE, this guide focuses on using the **Espressif IoT Development Framework (ESP-IDF)**, which provides more control and is the industry standard for professional ESP32 development.

We will also leverage **LVGL 9**, a powerful graphics library, and **`lvgl_cpp`**, a modern C++ wrapper that simplifies using LVGL in C++ projects.

## Overview

**Goal**: Display a scrollable menu on the round display where you can select a "Hello World" message, demonstrating touch input and smooth UI rendering.

**Hardware**:
*   Seeed Studio XIAO ESP32C6
*   Seeed Studio Round Display for XIAO (GC9A01 Display, CHSC6X Touch)

**Software Stack**:
*   **ESP-IDF**: The core OS and build system.
*   **LVGL 9**: Light and Versatile Graphics Library.
*   **lvgl_cpp**: A C++17 wrapper for LVGL, offering RAII-style memory management and a more idiomatic C++ API.

## 1. Prerequisites: ESP-IDF

Before proceeding, you must have the **ESP-IDF** installed on your system. This is the official development framework for ESP32 chips.

*   **Setup Guide**: Follow the official [ESP-IDF Get Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/get-started/index.html) to install the tools.
*   **VS Code Extension**: Alternatively, you can use the [Espressif IDF Extension for VS Code](https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/install.md) for a more integrated experience.

Ensure you can run `idf.py --version` in your terminal before continuing.

## 2. Getting the Code

Clone the playground repository which contains the demo code. Be sure to use `--recursive` to fetch necessary submodules involving the `lvgl_cpp` wrapper.

```bash
git clone --recursive https://github.com/someuser/esp32-playground.git
cd esp32-playground/round_display/hello_world
```

*(Note: Replace the URL with the actual repository URL if different)*

## 3. Project Structure

The project follows a standard ESP-IDF component structure:

*   `main/`: Contains the application source code.
    *   `main.cpp`: The entry point of the application.
    *   `hw/`: Hardware drivers (GC9A01 display, CHSC6X touch).
    *   `sys/`: System-level integrations (LVGL port).
    *   `ui/`: User Interface logic (`SelectHello` class).
*   `components/`: Custom components, including `lvgl_cpp`.
*   `idf_component.yml`: Manages managed components like `lvgl/lvgl` and drivers.

## 4. Understanding the Code

This demo uses C++ to interact with the hardware and LVGL. Here is how it all fits together.

### Main Entry Point (`main/main.cpp`)

The `app_main` function handles initialization in three stages:

1.  **Hardware Initialization**: Sets up the SPI bus for the display and I2C for the touch screen.
2.  **LVGL Setup**: Initializes the LVGL library via a helper class `LvglPort`.
3.  **UI Launch**: Creates and shows the `SelectHello` screen.

```cpp
extern "C" void app_main(void) {
  // 1. Initialize Display (GC9A01)
  Gc9a01::Config display_cfg = { /* SPI Pins & Settings */ };
  Gc9a01 display_hw(display_cfg);
  display_hw.init();

  // 2. Initialize Touch (CHSC6X)
  Chsc6x::Config touch_cfg = { /* I2C Pins & Settings */ };
  Chsc6x chsc6x(touch_cfg);
  ESP_ERROR_CHECK(chsc6x.init());

  {
    // 3. Initialize LVGL Port
    LvglPort::Config lvgl_config;
    lvgl_config.task_priority = 4;
    LvglPort lvgl_port(lvgl_config);
    lvgl_port.init(display_hw.get_panel_handle(), display_hw.get_io_handle());

    // Register touch input
    lvgl_port.register_touch_driver(&chsc6x);

    // 4. Show UI
    std::shared_ptr<SelectHello> hello_world = std::make_shared<SelectHello>();
    
    // Lock LVGL while modifying UI
    if (lvgl_port.lock(-1)) {
      hello_world->show_menu(*lvgl_port.get_display());
      lvgl_port.unlock();
    }

    // Keep the task alive
    while (1) {
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
}
```

### The UI Logic (`main/ui/select_hello.cpp`)

The `SelectHello` class utilizes `lvgl_cpp` to create widgets. Notice how C++ wrappers allow for cleaner code compared to raw C LVGL calls.

```cpp
void SelectHello::show_menu(lv_display_t &display) {
    // Create a screen
    auto screen = lv::Screen::create(display);
    
    // Create a generic Object (container)
    auto obj = lv::Object::create(screen);
    obj.set_size(lv::pct(100), lv::pct(100));
    obj.center();
    
    // The actual implementation interacts with specific LVGL widgets
}
```

## 5. Building and Flashing

Now, let's build the project and flash it to the board.

1.  **Set Target**: Tell ESP-IDF we are using the ESP32-C6.
    ```bash
    idf.py set-target esp32c6
    ```

2.  **Build**: Compile the project. This may take a few minutes the first time as it compiles LVGL and other dependencies.
    ```bash
    idf.py build
    ```

3.  **Flash and Monitor**: Flash the firmware and open the serial monitor to see logs.
    *   Make sure your XIAO is connected via USB.
    *   You might need to hold the **BOOT** button on the XIAO while plugging it in to enter download mode if flashing fails.
    
    ```bash
    idf.py flash monitor
    ```

## 6. Result

Once flashed, the round display should light up. You will see the "Select Hello" interface. You can tap on the screen (if touch is working) or verify the visual layout.

### Troubleshooting
*   **Touch not working?** Ensure the I2C lines are correctly mapped and the device is powered. The CHSC6X driver relies on I2C; look for "I2C initialized" logs in the monitor.
*   **Display blank?** 
    *   **Check Power**: Verify that the power switch on the round display is turned on.
    *   **Verify Pins**: Check that pin definitions in `main.cpp` match your specific expansion board wiring. The code above assumes the standard Seeed Round Display expansion board.

---

**Next Steps**: Try modifying `main/ui/select_hello.cpp` to add your own buttons or change the styling!
