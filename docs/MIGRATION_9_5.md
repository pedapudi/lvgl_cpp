# Migration Guide: Moving to LVGL 9.5

This guide outlines the changes and new features introduced in `lvgl_cpp` for compatibility with LVGL v9.5.

## Overview

LVGL 9.5 introduces several significant enhancements, including a new property interface, native blur and drop shadow styles, and new widgets like `ArcLabel` and `3DTexture`. `lvgl_cpp` has been updated to provide idiomatic C++ wrappers for these features while maintaining backward compatibility with v9.4 and v9.3.

## Key Changes

### 1. New Property Interface

LVGL 9.5 introduces a generic property interface (`lv_obj_set_property`). `lvgl_cpp` now leverages this internally to unify property setting across different widget types.

**Usage:**
Existing fluent setters (e.g., `btn.set_width(100)`) continue to work but may use the property interface internally when compiled against LVGL 9.5.

### 2. New Widgets

#### ArcLabel
A new widget that displays text along an arc.

```cpp
lvgl::ArcLabel al(lv_screen_active());
al.set_text("Hello Arc")
  .set_radius(100)
  .set_angles(180, 360)
  .center();
```

#### 3DTexture
A widget for rendering 3D textures.

```cpp
lvgl::ThreeDTexture texture(lv_screen_active());
texture.set_src(&my_3d_asset)
       .center();
```

### 3. Style Enhancements

#### Native Blur
LVGL 9.5 supports native blur properties.

```cpp
style.blur_radius(10)
     .blur_backdrop(true)
     .blur_quality(lvgl::BlurQuality::High);
```

#### Native Drop Shadows
Enhanced drop shadow properties are now available.

```cpp
style.drop_shadow_radius(15)
     .drop_shadow_offset_x(5)
     .drop_shadow_offset_y(5)
     .drop_shadow_color(lvgl::Color::black())
     .drop_shadow_opa(lvgl::Opacity::cover())
     .drop_shadow_quality(lvgl::BlurQuality::Medium);
```

### 4. Input Device Updates

New configuration options for gestures and key remapping.

```cpp
lvgl::InputDevice indev = lvgl::InputDevice::get_active();
indev->set_gesture_min_velocity(5);
indev->set_gesture_min_distance(30);

// Key remapping for keypads
indev->set_key_remap_cb([](lv_indev_t* indev, uint32_t key) {
    if (key == LV_KEY_ENTER) return (uint32_t)LV_KEY_RIGHT;
    return key;
});
```

### 5. Chart Updates

#### Curved Lines and Stacked Bars
The `Chart` widget now supports curved line connections and stacked bar types.

```cpp
chart.set_type(lvgl::Chart::Type::Curve);
// or
chart.set_type(lvgl::Chart::Type::Stacked);
```

## Compatibility Notes

### LVGL 9.3 (ZMK Compatibility)
`lvgl_cpp` remains compatible with LVGL 9.3. Features introduced in 9.4 and 9.5 are conditionally compiled. If you are using LVGL 9.3, new widgets like `ArcLabel` and new style properties like `blur_radius` will not be available.

### LVGL 9.4
`lvgl_cpp` continues to support LVGL 9.4. 9.5-specific features will be disabled when compiling against 9.4.

## Recommended Steps for Upgrading

1. Update your LVGL submodule or library to v9.5.0.
2. Recompile `lvgl_cpp`. The build system will automatically detect the new version and enable 9.5 features.
3. Review your custom widgets or style definitions to leverage the new blur and drop shadow properties where appropriate.
4. If using `Chart`, consider using the new `Type::Curve` for smoother data visualization.
