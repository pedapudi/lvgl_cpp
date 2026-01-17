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

  // --- Basic Properties ---

  /**
   * @brief Set the position relative to the parent.
   * @param x X coordinate.
   * @param y Y coordinate.
   */
  void set_pos(int32_t x, int32_t y);

  /**
   * @brief Set the x coordinate.
   * @param x X coordinate.
   */
  void set_x(int32_t x);

  /**
   * @brief Set the y coordinate.
   * @param y Y coordinate.
   */
  void set_y(int32_t y);

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
   * @brief Alignment options (wrapper for lv_align_t).
   */
  enum class Align {
    Default = LV_ALIGN_DEFAULT,
    TopLeft = LV_ALIGN_TOP_LEFT,
    TopMid = LV_ALIGN_TOP_MID,
    TopRight = LV_ALIGN_TOP_RIGHT,
    BottomLeft = LV_ALIGN_BOTTOM_LEFT,
    BottomMid = LV_ALIGN_BOTTOM_MID,
    BottomRight = LV_ALIGN_BOTTOM_RIGHT,
    LeftMid = LV_ALIGN_LEFT_MID,
    RightMid = LV_ALIGN_RIGHT_MID,
    Center = LV_ALIGN_CENTER,
    OutTopLeft = LV_ALIGN_OUT_TOP_LEFT,
    OutTopMid = LV_ALIGN_OUT_TOP_MID,
    OutTopRight = LV_ALIGN_OUT_TOP_RIGHT,
    OutBottomLeft = LV_ALIGN_OUT_BOTTOM_LEFT,
    OutBottomMid = LV_ALIGN_OUT_BOTTOM_MID,
    OutBottomRight = LV_ALIGN_OUT_BOTTOM_RIGHT,
    OutLeftTop = LV_ALIGN_OUT_LEFT_TOP,
    OutLeftMid = LV_ALIGN_OUT_LEFT_MID,
    OutLeftBottom = LV_ALIGN_OUT_LEFT_BOTTOM,
    OutRightTop = LV_ALIGN_OUT_RIGHT_TOP,
    OutRightMid = LV_ALIGN_OUT_RIGHT_MID,
    OutRightBottom = LV_ALIGN_OUT_RIGHT_BOTTOM,
  };

  /**
   * @brief Align the object within its parent.
   * @param align One of `Align` constants.
   * @param x_ofs X offset from the alignment point.
   * @param y_ofs Y offset from the alignment point.
   */
  void align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);

  /**
   * @brief Align the object to another object (base).
   * @param base The reference object to align to.
   * @param align One of `Align` constants.
   * @param x_ofs X offset.
   * @param y_ofs Y offset.
   */
  void align_to(const Object& base, Align align, int32_t x_ofs = 0,
                int32_t y_ofs = 0);

  /**
   * @brief Center the object in its parent.
   * Equivalent to `align(LV_ALIGN_CENTER, 0, 0)`.
   */
  void center();

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

  // --- Layouts ---

  /**
   * @brief Set the layout of the object.
   * @param layout The layout descriptor (e.g. `LV_LAYOUT_FLEX`,
   * `LV_LAYOUT_GRID`).
   */
  void set_layout(uint32_t layout);

  /**
   * @brief Check if the object is positioned by a layout.
   * @return true if the object's position is set by a layout.
   */
  bool is_layout_positioned() const;

  /**
   * @brief Mark the layout as dirty to trigger a refresh.
   */
  void mark_layout_as_dirty();

  /**
   * @brief Update the layout immediately.
   */
  void update_layout();

  // Flex Layout

  /**
   * @brief Set the flex flow direction.
   * @param flow The flow direction (e.g. `LV_FLEX_FLOW_ROW`,
   * `LV_FLEX_FLOW_COLUMN`).
   */
  void set_flex_flow(lv_flex_flow_t flow);

  /**
   * @brief Set the flex alignment.
   * @param main_place Alignment on the main axis.
   * @param cross_place Alignment on the cross axis.
   * @param track_cross_place Alignment of the tracks (if wrapped).
   */
  void set_flex_align(lv_flex_align_t main_place, lv_flex_align_t cross_place,
                      lv_flex_align_t track_cross_place);

  /**
   * @brief Set the flex grow factor.
   * @param grow The grow factor (0: none, 1+: share remaining space).
   */
  void set_flex_grow(uint8_t grow);

  // Grid Layout

  /**
   * @brief Set the grid descriptor arrays.
   * Arrays must be static or global as LVGL keeps a pointer to them (unless
   * local style copy is used, but typically static).
   * @param col_dsc Array of column descriptors (ending with
   * `LV_GRID_TEMPLATE_LAST`).
   * @param row_dsc Array of row descriptors (ending with
   * `LV_GRID_TEMPLATE_LAST`).
   */
  void set_grid_dsc_array(const int32_t col_dsc[], const int32_t row_dsc[]);

  /**
   * @brief Set the grid alignment.
   * @param column_align Alignment of columns.
   * @param row_align Alignment of rows.
   */
  void set_grid_align(lv_grid_align_t column_align, lv_grid_align_t row_align);

  /**
   * @brief Set the grid cell placement for this object.
   * @param column_align Alignment in the column.
   * @param col_pos Column index.
   * @param col_span Number of columns to span.
   * @param row_align Alignment in the row.
   * @param row_pos Row index.
   * @param row_span Number of rows to span.
   */
  void set_grid_cell(lv_grid_align_t column_align, int32_t col_pos,
                     int32_t col_span, lv_grid_align_t row_align,
                     int32_t row_pos, int32_t row_span);

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
