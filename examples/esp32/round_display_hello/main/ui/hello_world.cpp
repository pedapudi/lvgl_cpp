#include "hello_world.h"

#include <cstring>

#include "earth_svg.h"
#include "esp_log.h"
#include "lvgl_cpp/draw/image_descriptor.h"
#include "lvgl_cpp/widgets/image.h"

/*
 *  BEGINNER'S GUIDE TO LVGL + SVG IMPLEMENTATION
 *  =============================================
 *
 *  This file implements the "Hello World" scene for the round display.
 *  It demonstrates several critical patterns for embedded GUI development,
 *  specifically focusing on mixing vector graphics (SVG) with standard widgets.
 *
 *  KEY CONCEPTS:
 *  1. Vector Graphics on Embedded (SVG)
 *  2. Raw Data Handling & Decoders
 *  3. Memory Ownership (C++ Wrappers vs C Objects)
 *  4. Animation Lambdas
 *
 *  -------------------------------------------------------------------------
 *  SECTION 1: SVG VALIDATION TRAP (WHAT NOT TO DO)
 *  -------------------------------------------------------------------------
 *  The LVGL SVG decoder (based on ThorVG) is extremely strict about input data.
 *  Inside `lv_svg_decoder.c`, there is a function `valid_svg_data()` which
 *  checks the first few bytes of the data buffer.
 *
 *  IT MUST START WITH "<svg" OR "<?xml".
 *
 *  Common Mistake:
 *      static const char* svg = R"svg(
 *      <svg ...>
 *      )svg";
 *
 *  In C++, raw string literals often include the newline after the opening
 * quote. Result: The data starts with "\n<svg". Consequence: LVGL rejects the
 * data, says "Unknown format", and nothing renders.
 *
 *  Fix: We must verify and skip any leading whitespace before passing the
 * pointer to the image decoder.
 *
 *  -------------------------------------------------------------------------
 *  SECTION 2: MEMORY SAFETY & THE "STATIC" KEYWORD
 *  -------------------------------------------------------------------------
 *  LVGL's image rendering is deferred. When you call `set_src`, you are often
 *  just passing a pointer to data. If that data is on the stack (a local
 * variable), it will be destroyed when the function returns.
 *
 *  When the display refreshes 10ms later, LVGL reads garbage memory -> CRASH.
 *
 *  Solution: The `ImageDescriptor` must be `static` (persisting for the
 * program's life) or allocated on the heap/global scope.
 *
 *  -------------------------------------------------------------------------
 *  SECTION 3: C++ WRAPPERS & OWNERSHIP
 *  -------------------------------------------------------------------------
 *  The `lvgl_cpp` wrappers follow a RAII (Resource Acquisition Is
 * Initialization) pattern. When the C++ object (`lvgl::Image earth`) goes out
 * of scope at the end of the `if` block, its destructor runs.
 *
 *  If the destructor deletes the underlying LVGL C widget, your image
 * disappears instantly!
 *
 *  Solution: Call `.release()` on objects that you want to stay on the screen
 *  passing ownership to the LVGL parent system.
 */

LV_FONT_DECLARE(lv_font_montserrat_20);

void HelloWorld::load(lvgl::Display& display, const std::string& text,
                      lvgl::Color bg_color, lvgl::Color text_color,
                      bool show_earth) {
  // 1. Get the Active Screen
  // We use the active screen so we don't need to create a new window/layer
  // manually.
  auto screen = display.get_screen_active();
  lvgl::Object scr(screen);

  // Apply background style using the provided color
  // Opacity::Cover ensures the background is opaque (alpha 255).
  scr.style().bg_color(bg_color).bg_opa(lvgl::Opacity::Cover);

  if (show_earth) {
    /*
     * -----------------------------------------------------------------------
     * IMPLEMENTATION DETAIL: SVG PARSING FIX
     * -----------------------------------------------------------------------
     * Here we address the whitespace issue mentioned in Section 1.
     * We iterate through the string pointer until we find the opening bracket
     * '<'. This creates a safe pointer `validated_svg_content` that starts
     * exactly where the decoder expects.
     */
    const char* raw_svg_ptr = earth_animation_svg;
    while (*raw_svg_ptr && *raw_svg_ptr != '<') raw_svg_ptr++;

    /*
     * -----------------------------------------------------------------------
     * CRITICAL CONFIGURATION: IMAGE DESCRIPTOR
     * -----------------------------------------------------------------------
     * We construct the image descriptor to tell LVGL how to interpret the data.
     *
     * Params:
     * - 150, 150: The intrinsic dimensions of the SVG. Matches viewBox="0 0 150
     * 150".
     * - LV_COLOR_FORMAT_RAW: KEY! This tells LVGL "Don't treat this as pixel
     * data. Ask the decoders who can handle this format". This triggers the SVG
     * decoder.
     * - raw_svg_ptr: The validated pointer.
     * - size: We pass `strlen + 1` because the underlying parser (ThorVG) might
     *   rely on the null terminator for safety. It's a cheap insurance policy
     * against buffer overruns.
     *
     * NOTE: This variable is STATIC. It must survive after this function
     * returns.
     */
    static lvgl::ImageDescriptor earth_dsc(150, 150, LV_COLOR_FORMAT_RAW,
                                           (const uint8_t*)raw_svg_ptr,
                                           strlen(raw_svg_ptr) + 1);

    // Create the Image Widget on the screen
    lvgl::Image earth(&scr);

    earth.set_src(earth_dsc).center();  // Auto-align to center of parent

    /*
     * -----------------------------------------------------------------------
     * ANIMATION SYSTEM
     * -----------------------------------------------------------------------
     * LVGL animations change a value over time.
     *
     * - set_var(earth): The context object passed to the callback.
     * - set_values(0, 3600): The range. LVGL rotation is measured in 0.1
     * degrees. So 3600 = 360.0 degrees.
     * - set_exec_cb: The lambda that applies the value.
     *
     * PERFORMANCE NOTE: Rotating SVGs is expensive!
     * Every frame, the vector engine rasterizes the drawing again at the new
     * angle. If you set the duration too fast (e.g., 500ms), the CPU won't keep
     * up, triggering the Watchdog Timer (WDT). 10 seconds (10000ms) is smooth
     * and safe.
     */
    lvgl::Animation anim;
    anim.set_var(earth)
        .set_values(0, 3600)
        .set_duration(10000)
        .set_repeat_count(LV_ANIM_REPEAT_INFINITE)
        .set_exec_cb(lvgl::Animation::ExecCallback([](void* obj, int32_t v) {
          // Cast the void* back to an LVGL object pointer
          lv_image_set_rotation((lv_obj_t*)obj, (int16_t)v);
        }))
        .start();

    // HAND OFF OWNERSHIP: Detach the C++ wrapper so the C object survives.
    earth.release();
  }

  /*
   * -------------------------------------------------------------------------
   * STANDARD WIDGETS (LABEL)
   * -------------------------------------------------------------------------
   * Just like the image, we create a label, style it, and animate it.
   */
  lvgl::Label label(&scr);
  label.set_long_mode(lvgl::Label::LongMode::ScrollCircular)
      .set_text(text)
      .set_width(display.get_horizontal_resolution() - 40)  // Padding logic
      .align(lvgl::Align::Center, 0, 48);

  label.style()
      .text_font(lvgl::Font(&lv_font_montserrat_20))
      .text_color(text_color);

  // Slide-in Animation (Move Y from -50 relative to current pos to 0)
  // Wait, standard LVGL animations are usually absolute or relative depending
  // on implementation? Here set_values(-50, 0) likely drives a translation
  // offset.
  lvgl::Animation(label)
      .set_values(-150, 0)
      .set_duration(1500)
      .set_exec_cb(lvgl::Animation::ExecCallback([](void* obj, int32_t v) {
        // Wrap the raw object pointer in a temporary C++ wrapper (Unmanaged)
        // to use the idiomatic style API.
        lvgl::Object(static_cast<lv_obj_t*>(obj)).style().translate_y(v);
      }))
      .set_path_cb(lvgl::Animation::Path::EaseOut())
      .set_completed_cb([]() { ESP_LOGI("HelloWorld", "Slide-in completed"); })
      .start();

  // HAND OFF OWNERSHIP
  label.release();
}
