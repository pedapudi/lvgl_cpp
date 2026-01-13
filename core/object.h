#ifndef LVGL_CPP_CORE_OBJECT_H_
#define LVGL_CPP_CORE_OBJECT_H_

#include "lvgl.h"
#include <functional>
#include <memory>

/**
 * @file object.h
 * @brief Base class for all LVGL C++ wrappers.
 *
 * # Usage Guide
 *
 * ## Memory Management
 * The `Object` class (and all Widget classes) uses a robust RAII-style memory
 * management model designed to work seamlessly with LVGL's parent-child
 * deletion logic.
 *
 * ### 1. Owned Objects
 * When you create an object with a parent using the C++ constructor, the C++
 * wrapper assumes ownership.
 *
 * ```cpp
 * {
 *     lvgl::Button btn(lv_screen_active());
 *     // ... use btn ...
 * } // btn is destroyed here. The underlying lv_obj_t is deleted from the
 * screen.
 * ```
 *
 * ### 2. Wrappers / Proxies
 * When you wrap an existing `lv_obj_t*` (e.g., returned by a helper function),
 * the C++ wrapper acts as a non-owning proxy.
 *
 * ```cpp
 * lvgl::Object tab = tabview.add_tab("Settings"); // Returns a proxy object
 * // 'tab' wrapper can go out of scope without deleting the actual tab page.
 * ```
 *
 * ### 3. Safety Mechanism
 * The wrapper listens for the `LV_EVENT_DELETE` event.
 * - If the *parent* deletes the child (e.g., screen clear), the C++ wrapper is
 * notified and marks itself as invalid (`obj_ = nullptr`).
 * - Subsequent usage of the C++ wrapper needs `is_valid()` checks if unsure,
 * but the destructor is safe (double-free protection).
 *
 * # API Overview
 * - `Object()`: Create a new instance (usually a Screen).
 * - `Object(Object* parent)`: Create a child object.
 * - `set_pos()`, `set_size()`, `align()`: Layout control.
 * - `add_event_cb()`: Functional event callbacks.
 */

namespace lvgl {

class Style; // Forward declaration

/**
 * @brief Base class for all LVGL objects.
 * Wraps lv_obj_t and provides RAII semantics.
 */
class Object {
public:
  /**
   * @brief Create a wrapper around an existing LVGL object.
   * @param obj The LVGL object to wrap.
   * @param owned If true, the C++ object takes ownership and will delete the
   * LVGL object on destruction. Default is false (weak reference).
   *
   * @note Use this to wrap objects returned by LVGL C API or callback
   * parameters.
   */
  explicit Object(lv_obj_t *obj, bool owned = false);

  /**
   * @brief Create a new Object (Screen).
   * Creates a new `lv_obj` with no parent, effectively serving as a Screen.
   * Takes ownership.
   */
  Object();

  /**
   * @brief Create a new Object with a parent.
   * Creates a new `lv_obj` as a child of the given parent.
   * Takes ownership.
   * @param parent The parent object. Must not be null for standard widgets.
   */
  explicit Object(Object *parent);

  /**
   * @brief Destructor.
   * Unregisters callbacks. If the object is owned, it deletes the underlying
   * LVGL object.
   */
  virtual ~Object();

  // Non-copyable to prevent ambiguous ownership
  Object(const Object &) = delete;
  Object &operator=(const Object &) = delete;

  // Moveable
  Object(Object &&other) noexcept;
  Object &operator=(Object &&other) noexcept;

  /**
   * @brief Release ownership of the underlying LVGL object.
   * The C++ object will no longer delete the LVGL object on destruction.
   * Useful when passing ownership to another system.
   * @return The raw pointer (same as raw()).
   */
  lv_obj_t *release();

  /**
   * @brief Get the raw LVGL object pointer.
   * @return lv_obj_t* pointer or nullptr if invalid.
   */
  lv_obj_t *raw() const { return obj_; }

  /**
   * @brief Check if the object is valid.
   * @return true if the underlying LVGL object exists, false if it has been
   * deleted.
   */
  bool is_valid() const { return obj_ != nullptr; }

  // --- Basic Properties ---

  /**
   * @brief Set the position relative to the parent.
   * @param x X coordinate.
   * @param y Y coordinate.
   */
  void set_pos(int32_t x, int32_t y);

  /**
   * @brief Set the size of the object.
   * @param w Width in pixels.
   * @param h Height in pixels.
   */
  void set_size(int32_t w, int32_t h);

  /**
   * @brief Set the width of the object.
   * @param w Width in pixels.
   */
  void set_width(int32_t w);

  /**
   * @brief Set the height of the object.
   * @param h Height in pixels.
   */
  void set_height(int32_t h);

  /**
   * @brief Get the x position relative to the parent.
   */
  int32_t get_x() const;

  /**
   * @brief Get the y position relative to the parent.
   */
  int32_t get_y() const;

  /**
   * @brief Get the width of the object.
   */
  int32_t get_width() const;

  /**
   * @brief Get the height of the object.
   */
  int32_t get_height() const;

  /**
   * @brief Align the object within its parent.
   * @param align One of `LV_ALIGN_...` constants.
   * @param x_ofs X offset from the alignment point.
   * @param y_ofs Y offset from the alignment point.
   */
  void align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0);

  /**
   * @brief Align the object to another object (base).
   * @param base The reference object to align to.
   * @param align One of `LV_ALIGN_...` constants.
   * @param x_ofs X offset.
   * @param y_ofs Y offset.
   */
  void align_to(const Object *base, lv_align_t align, int32_t x_ofs = 0,
                int32_t y_ofs = 0);

  /**
   * @brief Center the object in its parent.
   * Equivalent to `align(LV_ALIGN_CENTER, 0, 0)`.
   */
  void center();

  // --- Flags & States ---

  /**
   * @brief Add a flag to the object.
   * @param f The flag to add (e.g., `LV_OBJ_FLAG_HIDDEN`).
   */
  void add_flag(lv_obj_flag_t f);

  /**
   * @brief Remove a flag from the object.
   * @param f The flag to remove.
   */
  void remove_flag(lv_obj_flag_t f);

  /**
   * @brief Check if a flag is set.
   * @param f The flag to check.
   */
  bool has_flag(lv_obj_flag_t f) const;

  /**
   * @brief Add a state to the object.
   * @param state The state to add (e.g., `LV_STATE_PRESSED`).
   */
  void add_state(lv_state_t state);

  /**
   * @brief Remove a state from the object.
   * @param state The state to remove.
   */
  void remove_state(lv_state_t state);

  /**
   * @brief Check if a state is active.
   * @param state The state to check.
   */
  bool has_state(lv_state_t state) const;

  // --- Events ---

  /**
   * @brief Functional event callback type.
   */
  using EventCallback = std::function<void(lv_event_t *)>;

  /**
   * @brief Add a functional event callback.
   * Uses `std::function` to allow lambdas with captures.
   * @param event_code The event code to listen for (e.g., `LV_EVENT_CLICKED`).
   * @param callback The callable to execute.
   */
  void add_event_cb(lv_event_code_t event_code, EventCallback callback);

  // --- Styles ---

  /**
   * @brief Add a style to the object.
   * @param style The C++ style wrapper.
   * @param selector The part/state selector (default `LV_PART_MAIN`).
   */
  void add_style(Style &style, lv_style_selector_t selector = LV_PART_MAIN);

  /**
   * @brief Remove a style from the object.
   * @param style The C++ style wrapper.
   * @param selector The part/state selector.
   */
  void remove_style(Style *style, lv_style_selector_t selector = LV_PART_MAIN);

  // Local style properties (common subset)
  void set_style_anim_duration(uint32_t value,
                               lv_style_selector_t selector = LV_PART_MAIN);
  void set_style_text_align(lv_text_align_t value,
                            lv_style_selector_t selector = LV_PART_MAIN);
  void set_style_bg_color(lv_color_t value,
                          lv_style_selector_t selector = LV_PART_MAIN);
  void set_style_bg_opa(lv_opa_t value,
                        lv_style_selector_t selector = LV_PART_MAIN);
  void set_style_image_recolor_opa(lv_opa_t value,
                                   lv_style_selector_t selector = LV_PART_MAIN);
  void set_style_image_recolor(lv_color_t value,
                               lv_style_selector_t selector = LV_PART_MAIN);
  void set_style_bg_image_src(const void *value,
                              lv_style_selector_t selector = LV_PART_MAIN);

  // --- Layouts ---

  void set_layout(uint32_t layout);
  bool is_layout_positioned() const;
  void mark_layout_as_dirty();
  void update_layout();

  // Flex Layout
  void set_flex_flow(lv_flex_flow_t flow);
  void set_flex_align(lv_flex_align_t main_place, lv_flex_align_t cross_place,
                      lv_flex_align_t track_cross_place);
  void set_flex_grow(uint8_t grow);

  // Grid Layout
  void set_grid_dsc_array(const int32_t col_dsc[], const int32_t row_dsc[]);
  void set_grid_align(lv_grid_align_t column_align, lv_grid_align_t row_align);
  void set_grid_cell(lv_grid_align_t column_align, int32_t col_pos,
                     int32_t col_span, lv_grid_align_t row_align,
                     int32_t row_pos, int32_t row_span);

protected:
  lv_obj_t *obj_ = nullptr;
  bool owned_ = false;

  struct CallbackNode {
    EventCallback callback;
  };

  // Keep track of event callback closures
  std::vector<std::unique_ptr<CallbackNode>> callback_nodes_;

  static void event_proxy(lv_event_t *e);

  /**
   * @brief Internal hook to handle LVGL deletion event.
   * Updates safety flag when external deletion happens.
   */
  static void on_delete_event(lv_event_t *e);

  /**
   * @brief Install the safety event hook.
   */
  void install_delete_hook();
};

} // namespace lvgl

#endif // LVGL_CPP_CORE_OBJECT_H_
