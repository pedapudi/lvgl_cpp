# Input Device Design for `lvgl_cpp`

## 1. Executive Summary
This document analyzes the current state of Input Device (`indev`) support in `lvgl_cpp` and proposes a comprehensive design to bring idiomatic C++ capabilities to LVGL's input subsystem.

The goal is to move beyond simple wrappers for *existing* devices and enable the *creation* and *management* of input drivers (Touch, Mouse, Keypad, Encoder) directly from C++.

## 2. LVGL Input Device Concepts
The `lv_indev` module abstracts hardware input differences into four logical types.

| Type | Description | Usage Pattern |
| :--- | :--- | :--- |
| **Pointer** | Mouse, Touchpad, Touchscreen | Clicks, drags, and absolute positioning. Uses a cursor object (optional). |
| **Keypad** | Keyboard, Physical Keypad | Sends characters or control keys (Arrows, Enter). Requires a `Group` for focus. |
| **Encoder** | Rotary Encoder | Turns left/right, click, long-press. Navigates `Group` or edits values. |
| **Button** | External Hardware Buttons | Mapped to specific screen coordinates. "virtual press". |

### 2.1. Critical Workflows
1.  **Creation & Registration**: `lv_indev_create()` returns a handle.
2.  **Configuration**: Set type (`lv_indev_set_type`) and `read_cb` (`lv_indev_set_read_cb`).
3.  **Data Reading**:
    *   **Polling (Default)**: LVGL calls `read_cb` periodically (30ms).
    *   **Event-Driven**: Driver calls `lv_indev_read()` explicitly when data is ready.
4.  **Groups & Navigation**: Keypad and Encoder devices **must** be associated with an `lv_group_t` to function effectively.

## 3. Current State of `lvgl_cpp`
*   **Existing Files**: `indev/input_device.h`, `indev/input_device.cpp`.
*   **Functionality**:
    *   Wraps an *existing* `lv_indev_t*`.
    *   Provides accessors (`get_type`, `get_state`).
    *   Provides associating helpers (`set_group`, `set_cursor`).
*   **Gaps**:
    *   **No Creation Support**: Cannot create a new input device from C++.
    *   **No Callback Support**: Cannot define the `read_cb` using C++ lambdas or functions.
    *   **No Driver Logic**: Requires the user to write C code for the driver and pass the handle to C++.

## 4. Design Proposal
We propose extending `lvgl_cpp` to support the full lifecycle of input devices.

### 4.1. `InputDevice` Class Evolution
The `InputDevice` class should support both wrapping existing devices and creating new ones.

```cpp
// Proposed usage
auto mouse_drv = InputDevice::create(LV_INDEV_TYPE_POINTER);
mouse_drv.set_read_callback([](lv_indev_data_t* data) {
    // C++ logic to read hardware
    data->point.x = ...;
    data->state = LV_INDEV_STATE_PRESSED;
});
```

### 4.2. Callback Dispatcher
To bridge C function pointers to C++ std::function, we need a user_data dispatch mechanism.
*   `lv_indev_set_user_data(indev, wrapper_instance)` (if supported) or a static registry map `std::map<lv_indev_t*, std::function>`.
*   **Note**: `lv_indev_t` has `user_data`. We can store the pointer to the `InputDevice` C++ instance there.

### 4.3. Specific Device Subclasses
While a generic `InputDevice` is useful, subclasses can enforce type safety:
*   `PointerDevice` (adds `set_cursor`)
*   `KeypadDevice` (adds `set_group`)
*   `EncoderDevice` (adds `set_group`)

### 4.4. The `Group` Abstraction
`lv_group_t` is critical. The C++ `Group` class (already present in `core/group.h`) needs to be robust.

```cpp
Group g;
g.add(button1);
g.add(slider1);
// g.set_default(); // utility

EncoderDevice encoder;
encoder.set_group(g);
```

### 4.5. Usage Recipes

#### Scenario A: Adding a Custom Touchscreen
```cpp
// 1. Create Device
auto touch = InputDevice::create(LV_INDEV_TYPE_POINTER);

// 2. Define Driver Logic
touch.set_read_cb([&](lv_indev_data_t* data){
    if (hardware.is_touched()) {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = hardware.x;
        data->point.y = hardware.y;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
});
```

#### Scenario B: Keyboard Navigation
```cpp
// 1. Setup Group
Group nav_group;
nav_group.set_default(); // Widgets added hereafter join this group automatically? 
// Or manual:
nav_group.add(my_button);

// 2. Setup Keypad
auto kb = InputDevice::create(LV_INDEV_TYPE_KEYPAD);
kb.set_group(nav_group);
kb.set_read_cb(read_keyboard_matrix);
```

## 5. Recommendation
1.  **Refactor `InputDevice`**: Add `create()` static factory.
2.  **Implement `InputDriver`**: A helper/mixin to manage the `read_cb` bridging.
3.  **Enhance `Group`**: Ensure it exposes all `lv_group` operations (focus, freeze, edit mode).
4.  **Utilities**: Add `Cursor` helper class for Pointer devices.

This approach ensures `lvgl_cpp` is not just a UI toolkit but a full application framework capable of managing the hardware layer in idiomatic C++.
