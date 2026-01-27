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
#include <vector>

#include "../misc/enums.h"
#include "event.h"
#include "event_proxy.h"
#include "group_proxy.h"
#include "layout_proxy.h"
#include "lvgl.h"  // IWYU pragma: export
#include "scroll_proxy.h"
#include "state_proxy.h"
#include "style_proxy.h"

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
  using Align = lvgl::Align;

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
   * @brief Functional event callback type.
   */
  using EventCallback = std::function<void(Event&)>;

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

  // --- Object Tree Management ---

  /**
   * @brief Delete all children of this object.
   * Useful for rebuilding container contents.
   */
  void clean();

  /**
   * @brief Get the parent object.
   * @return An unmanaged Object wrapper for the parent, or invalid if no
   * parent.
   */
  Object get_parent() const;

  /**
   * @brief Get a child object by index.
   * @param index The child index (0-based). Negative indexes count from the
   * end.
   * @return An unmanaged Object wrapper for the child, or invalid if not found.
   */
  Object get_child(int32_t index) const;

  /**
   * @brief Get the number of children.
   * @return The child count.
   */
  uint32_t get_child_count() const;

  /**
   * @brief Set the parent of this object.
   * @param parent The new parent.
   */
  void set_parent(Object& parent);

  /**
   * @brief Set the parent of this object.
   * @param parent The new parent (raw pointer).
   */
  void set_parent(lv_obj_t* parent);

  /**
   * @brief Get the index of this object among its siblings.
   * @return The index (0-based).
   */
  int32_t get_index() const;

  /**
   * @brief Move this object to the foreground (on top of siblings).
   */
  void move_foreground();

  /**
   * @brief Move this object to the background (behind siblings).
   */
  void move_background();

  /**
   * @brief Schedule this object for deletion on the next timer handler cycle.
   * Safe to call from callbacks to avoid use-after-free.
   */
  void delete_async();

  // --- Geometric Properties ---

  /**
   * @brief Set the x coordinate of the object.
   * @param x The x coordinate.
   * @return Reference to this object.
   */
  Object& set_x(int32_t x);

  /**
   * @brief Set the y coordinate of the object.
   * @param y The y coordinate.
   * @return Reference to this object.
   */
  Object& set_y(int32_t y);

  /**
   * @brief Set the position of the object.
   * @param x The x coordinate.
   * @param y The y coordinate.
   * @return Reference to this object.
   */
  Object& set_pos(int32_t x, int32_t y);

  /**
   * @brief Set the alignment of the object.
   * @param align The alignment type.
   * @param x_ofs The x offset.
   * @param y_ofs The y offset.
   * @return Reference to this object.
   */
  Object& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);

  /**
   * @brief Set the alignment of the object.
   * @param align The alignment type (raw LVGL enum).
   * @param x_ofs The x offset.
   * @param y_ofs The y offset.
   * @return Reference to this object.
   */
  Object& align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0);

  /**
   * @brief Align the object to another object.
   * @param base The object to align to.
   * @param align The alignment type.
   * @param x_ofs The x offset.
   * @param y_ofs The y offset.
   * @return Reference to this object.
   */
  Object& align_to(const Object& base, Align align, int32_t x_ofs = 0,
                   int32_t y_ofs = 0);

  /**
   * @brief Align the object to another object.
   * @param base The object to align to.
   * @param align The alignment type (raw LVGL enum).
   * @param x_ofs The x offset.
   * @param y_ofs The y offset.
   * @return Reference to this object.
   */
  Object& align_to(const Object& base, lv_align_t align, int32_t x_ofs = 0,
                   int32_t y_ofs = 0);

  /**
   * @brief Center the object on its parent.
   * @return Reference to this object.
   */
  Object& center();

  /**
   * @brief Get the x coordinate.
   */
  int32_t get_x() const;

  /**
   * @brief Get the y coordinate.
   */
  int32_t get_y() const;

  /**
   * @brief Set the width of the object.
   * @param w The width.
   * @return Reference to this object.
   */
  Object& set_width(int32_t w);

  /**
   * @brief Set the height of the object.
   * @param h The height.
   * @return Reference to this object.
   */
  Object& set_height(int32_t h);

  /**
   * @brief Set the size of the object.
   * @param w The width.
   * @param h The height.
   * @return Reference to this object.
   */
  Object& set_size(int32_t w, int32_t h);

  /**
   * @brief Get the width of the object.
   */
  int32_t get_width() const;

  /**
   * @brief Get the height of the object.
   */
  int32_t get_height() const;

  // --- Layout Shortcuts ---

  Object& set_flex_flow(lv_flex_flow_t flow);
  Object& set_flex_align(lv_flex_align_t main_place,
                         lv_flex_align_t cross_place,
                         lv_flex_align_t track_place);
  Object& set_flex_grow(uint8_t grow);

  Object& set_grid_dsc_array(const int32_t* col_dsc, const int32_t* row_dsc);
  Object& set_grid_align(lv_grid_align_t column_align,
                         lv_grid_align_t row_align);
  Object& set_grid_cell(lv_grid_align_t column_align, int32_t col_pos,
                        int32_t col_span, lv_grid_align_t row_align,
                        int32_t row_pos, int32_t row_span);

  // --- Scroll ---

  Object& scroll_to_view(lv_anim_enable_t anim_en);
  Object& scroll_to_view_recursive(lv_anim_enable_t anim_en);
  Object& scroll_by(int32_t x, int32_t y, lv_anim_enable_t anim_en);
  Object& scroll_to(int32_t x, int32_t y, lv_anim_enable_t anim_en);
  int32_t get_scroll_x() const;
  int32_t get_scroll_y() const;
  int32_t get_scroll_top() const;
  int32_t get_scroll_bottom() const;
  int32_t get_scroll_left() const;
  int32_t get_scroll_right() const;

  int32_t get_content_width() const;
  int32_t get_content_height() const;
  int32_t get_self_width() const;
  int32_t get_self_height() const;

  // ... existing code ...

  // --- Layout and Scroll ---

  /**
   * @brief Get a layout proxy for setting layout properties.
   * @return A LayoutProxy object supporting fluent method chaining.
   */
  LayoutProxy layout() { return LayoutProxy(obj_); }

  /**
   * @brief Update the layout of the object.
   */
  void update_layout() {
    if (obj_) lv_obj_update_layout(obj_);
  }

  /**
   * @brief Get a scroll proxy for scrolling operations.
   * @return A ScrollProxy object supporting fluent method chaining.
   */
  ScrollProxy scroll() { return ScrollProxy(obj_); }

  /**
   * @brief Get a style proxy for setting style properties.
   * @param selector The part/state selector (default LV_PART_MAIN).
   * @return A StyleProxy object supporting fluent method chaining.
   */
  StyleProxy style(lv_style_selector_t selector = LV_PART_MAIN) {
    return StyleProxy(obj_, selector);
  }

  /**
   * @brief Get a style proxy for a specific state.
   */
  StyleProxy style(State s) {
    return style(static_cast<lv_style_selector_t>(s));
  }

  /**
   * @brief Get a style proxy for a specific part.
   */
  StyleProxy style(Part p) {
    return style(static_cast<lv_style_selector_t>(p));
  }

  // --- Events ---

  /**
   * @brief Get an event proxy for managing events on this object.
   * @return An EventProxy object supporting fluent method chaining.
   */
  EventProxy event();

  /**
   * @brief Get a state proxy for managing widget states.
   * @return A StateProxy object supporting fluent method chaining.
   */
  StateProxy state();

  /**
   * @brief Get a group proxy for managing navigation groups.
   * @return A GroupProxy object supporting fluent method chaining.
   */
  GroupProxy group();

  /**
   * @brief Manually send an event to this object.
   * @param code The event code.
   * @param param Optional parameter for the event.
   */
  void send_event(lv_event_code_t code, void* param = nullptr) {
    if (obj_) lv_obj_send_event(obj_, code, param);
  }

  /**
   * @brief Add an event callback (Internal/Generic).
   * @param event_code The event code.
   * @param callback The callable to execute.
   * @return Reference to this object.
   * @note Usually accessed via event().add_cb()
   */
  Object& add_event_cb(lv_event_code_t event_code, EventCallback callback);
  Object& add_event_cb(EventCode event_code, EventCallback callback) {
    return add_event_cb(static_cast<lv_event_code_t>(event_code),
                        std::move(callback));
  }

  /**
   * @brief Remove all event callbacks from this object.
   */
  void remove_all_event_cbs();

  // --- Flags & States ---

  /**
   * @brief Add a flag to the object.
   * @param f The flag to add (e.g., `LV_OBJ_FLAG_HIDDEN`).
   */
  bool has_flag(lv_obj_flag_t f) const;

  /**
   * @brief Add a flag to the object.
   * @param f The flag to add.
   */
  void add_flag(lv_obj_flag_t f);

  /**
   * @brief Add a flag to the object.
   * @param f The flag to add.
   */
  void add_flag(ObjFlag f);

  /**
   * @brief Remove a flag from the object.
   * @param f The flag to remove.
   */
  void remove_flag(lv_obj_flag_t f);

  /**
   * @brief Remove a flag from the object.
   * @param f The flag to remove.
   */
  void remove_flag(ObjFlag f);

  /**
   * @brief Add a state to the object.
   * @param s The state to add.
   */
  void add_state(lv_state_t s);

  /**
   * @brief Add a state to the object.
   * @param s The state to add.
   */
  void add_state(State s);

  /**
   * @brief Remove a state from the object.
   * @param s The state to remove.
   */
  void remove_state(lv_state_t s);

  /**
   * @brief Remove a state from the object.
   * @param s The state to remove.
   */
  void remove_state(State s);

  /**
   * @brief Check if a state is set.
   * @param s The state to check.
   */
  bool has_state(lv_state_t s) const;

  /**
   * @brief Check if a state is set.
   * @param s The state to check.
   */
  bool has_state(State s) const;

  /**
   * @brief Check if a flag is set.
   * @param f The flag to check.
   */
  bool has_flag(ObjFlag f) const;

  // --- Scroll ---

  // --- Other Properties ---

  /**
   * @brief Set the base direction (LTR, RTL, Auto).
   * @param dir The direction.
   */
  void set_base_dir(lv_base_dir_t dir);

  /**
   * @brief Set the base direction.
   * @param dir The direction.
   */
  void set_base_dir(BaseDir dir);

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

  // Local style properties (legacy setters removed, use using style())

 protected:
  lv_obj_t* obj_ = nullptr;
  bool owned_ = false;

  struct CallbackNode {
    lv_event_code_t event_code;
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
