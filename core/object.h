#ifndef LVGL_CPP_CORE_OBJECT_H_
#define LVGL_CPP_CORE_OBJECT_H_

#include <cstdint>

// Fix for 'noreturn' macro collision from C headers (e.g. stdnoreturn.h
// included by LVGL)
#if defined(noreturn)
#undef noreturn
#endif

#include <functional>
#include <memory>

#include "event.h"
#include "lvgl.h"  // IWYU pragma: export

/**
 * @file object.h
 * @brief Base class for all LVGL C++ wrappers.
 *
 * # Usage Guide
 *
 * ## Memory Management* The `Object` class (and all Widget classes) uses a
 * robust RAII-style memory management model designed to work seamlessly with
 * LVGL's parent-child deletion logic.
 *
 * ### 1. Owned Objects* When you create an object with a parent using the C++
 * constructor, the C++ wrapper assumes ownership.
 *
 * ```cpp
 * {
 *     lvgl::Button btn(lv_screen_active());
 *     // ... use btn ...
 * } // btn is destroyed here. The underlying lv_obj_t is deleted from the
 * screen.
 * ```
 *
 * ### 2. Wrappers / Proxies* When you wrap an existing `lv_obj_t*` (e.g.,
 * returned by a helper function), the C++ wrapper acts as a non-owning proxy.
 *
 * ```cpp
 * lvgl::Object tab = tabview.add_tab("Settings"); // Returns a proxy object
 * // 'tab' wrapper can go out of scope without deleting the actual tab page.
 * ```
 *
 * ### 3. Safety Mechanism* The wrapper listens for the `LV_EVENT_DELETE` event.
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

class Style;  // Forward declaration
class Event;  // Forward declaration

/**
 * @brief Base class for all LVGL objects.
 * Wraps lv_obj_t and provides RAII semantics.
 */
class Object {
 public:
  /**
   * @brief Ownership policy for LVGL objects.
   * Defines how the C++ wrapper manages the lifetime of the underlying LVGL
   * object.
   */
  enum class Ownership {
    Default,    ///< Owned if parent is set, Unmanaged if wrapping pointer.
    Managed,    ///< The C++ object owns the LVGL object and will delete it.
    Unmanaged,  ///< The C++ object is a weak reference (view) and will NOT
                ///< delete.
  };

  /**
   * @brief Alignment types.
   */
  using Align = lv_align_t;

  /**
   * @brief Create a wrapper around an existing LVGL object.
   * @param obj The LVGL object to wrap.
   * @param ownership The ownership policy.
   *        - `Ownership::Managed`: Wrapper deletes `obj` on destruction.
   *        - `Ownership::Unmanaged`: Wrapper does NOT delete `obj`.
   *        - `Ownership::Default`: Defaults to `Unmanaged`.
   *
   * @note Use this to wrap objects returned by LVGL C API or callback
   * parameters.
   */
  explicit Object(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Create a wrapper around an existing LVGL object
   * (Legacy/Convenience).
   * @param obj The LVGL object to wrap.
   * @param owned If true, equivalent to `Ownership::Managed`.
   * @deprecated Use the `Ownership` enum constructor instead.
   */
  explicit Object(lv_obj_t* obj, bool owned);

  /**
   * @brief Create a new Object (Screen).
   * Creates a new `lv_obj` with no parent, effectively serving as a Screen.
   * Takes ownership.
   */
  Object();

  /**
   * @brief Create a new Object with a parent.
   * Creates a new `lv_obj` as a child of the given parent.
   *
   * @param parent The parent object. Must not be null for standard widgets.
   * @param ownership The ownership policy.
   *        - `Ownership::Managed`: Wrapper deletes the child (Default).
   *        - `Ownership::Unmanaged`: Wrapper acts as a temporary handle.
   */
  explicit Object(Object* parent, Ownership ownership = Ownership::Default);

  /**
   * @brief Destructor.
   * Unregisters callbacks. If the object is owned, it deletes the underlying
   * LVGL object.
   */
  virtual ~Object();

  // Non-copyable to prevent ambiguous ownership
  Object(const Object&) = delete;
  Object& operator=(const Object&) = delete;

  // Moveable
  Object(Object&& other) noexcept;
  Object& operator=(Object&& other) noexcept;

  /**
   * @brief Release ownership of the underlying LVGL object.
   * The C++ object will no longer delete the LVGL object on destruction.
   * Useful when passing ownership to another system.
   * @return The raw pointer (same as raw()).
   */
  lv_obj_t* release();

  /**
   * @brief Get the raw LVGL object pointer.
   * @return lv_obj_t* pointer or nullptr if invalid.
   */
  lv_obj_t* raw() const { return obj_; }

  /**
   * @brief Check if the object is valid.
   * @return true if the underlying LVGL object exists, false if it has been
   * deleted.
   */
  bool is_valid() const { return obj_ != nullptr; }

  // --- Flags& States ---

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
  using EventCallback = std::function<void(Event&)>;

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
   * @note The style object must remain valid as long as it is used by the
   * object, unless it's a static/global style.
   */
  void add_style(Style& style, lv_style_selector_t selector = LV_PART_MAIN);

  /**
   * @brief Remove a style from the object.
   * @param style The C++ style wrapper.
   * @param selector The part/state selector.
   */
  void remove_style(Style* style, lv_style_selector_t selector = LV_PART_MAIN);

  // Local style properties (common subset)

  /**
   * @brief Set the animation duration for state changes.
   * @param value Duration in milliseconds.
   * @param selector The part/state selector.
   */
  void set_style_anim_duration(uint32_t value,
                               lv_style_selector_t selector = LV_PART_MAIN);

  /**
   * @brief Set the text alignment.
   * @param value The alignment (e.g. `LV_TEXT_ALIGN_CENTER`).
   * @param selector The part/state selector.
   */
  void set_style_text_align(lv_text_align_t value,
                            lv_style_selector_t selector = LV_PART_MAIN);

  /**
   * @brief Set the background color.
   * @param value The color.
   * @param selector The part/state selector.
   */
  void set_style_bg_color(lv_color_t value,
                          lv_style_selector_t selector = LV_PART_MAIN);

  /**
   * @brief Set the background opacity.
   * @param value The opacity (0..255 or `LV_OPA_...`).
   * @param selector The part/state selector.
   */
  void set_style_bg_opa(lv_opa_t value,
                        lv_style_selector_t selector = LV_PART_MAIN);

  /**
   * @brief Set the opacity of image recoloring.
   * @param value The opacity.
   * @param selector The part/state selector.
   */
  void set_style_image_recolor_opa(lv_opa_t value,
                                   lv_style_selector_t selector = LV_PART_MAIN);

  /**
   * @brief Set the image recolor value.
   * @param value The color to mix with the image.
   * @param selector The part/state selector.
   */
  void set_style_image_recolor(lv_color_t value,
                               lv_style_selector_t selector = LV_PART_MAIN);

  /**
   * @brief Set the background image source.
   * @param value Pointer to an `lv_image_dsc_t` or path string.
   * @param selector The part/state selector.
   */
  void set_style_bg_image_src(const void* value,
                              lv_style_selector_t selector = LV_PART_MAIN);

 protected:
  lv_obj_t* obj_ = nullptr;
  bool owned_ = false;

  struct CallbackNode {
    EventCallback callback;
  };

  // Keep track of event callback closures
  std::vector<std::unique_ptr<CallbackNode>> callback_nodes_;

  static void event_proxy(lv_event_t* e);

  /**
   * @brief Internal hook to handle LVGL deletion event.
   * Updates safety flag when external deletion happens.
   */
  static void on_delete_event(lv_event_t* e);

  /**
   * @brief Install the safety event hook.
   */
  void install_delete_hook();
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_OBJECT_H_
