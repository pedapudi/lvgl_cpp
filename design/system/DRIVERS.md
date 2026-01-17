# Drivers Design

## 1. Overview
This module covers wrappers for `lv_display_t` and `lv_indev_t`, critical for platform integration (like ESP32).

## 2. Display Driver (`Display`)
*   **Encapsulates**: `lv_display_t*`
*   **Methods**:
    *   `Display::create(int32_t w, int32_t h)`
    *   `Display& buffers(void* buf1, void* buf2, uint32_t size, lv_display_render_mode_t mode)`
    *   `Display& flush_cb(lv_display_flush_cb_t cb)`
    *   `Display& resolution(int32_t w, int32_t h)`

## 3. Input Device Driver (`InputDevice`)
*   **Encapsulates**: `lv_indev_t*`
*   **Methods**:
    *   `InputDevice::create(lv_indev_type_t type)`
    *   `InputDevice& read_cb(lv_indev_read_cb_t cb)`
    *   `InputDevice& group(Group& g)`
