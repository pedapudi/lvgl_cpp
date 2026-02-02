#ifndef LVGL_CPP_CORE_WIDGET_H_
#define LVGL_CPP_CORE_WIDGET_H_

#include <cstdint>
#include <functional>
#include <memory>

#include "lvgl.h"
#include "object.h"
#include "observer.h"

/**
 * @file widget.h
 * @brief User Guide:
 * The `Widget<Derived>` template is the basis for all concrete UI elements
 * (e.g., Button, Label). It uses the Curiously Recurring Template Pattern
 * (CRTP) to provide a fluent API where every `set_*` method returns a reference
 * to the actual `Derived` type.
 *
 * Key Features:
 * - **Fluent Chaining**: Allows writing expressive UI code:
 *   `auto btn = Button(parent).set_size(100, 50).on_click([](auto& e){...});`
 * - **Mixins**: Automatically provides common functionality like geometry,
 * styles, and event handlers to all derived widgets.
 * - **Type Safety**: Methods like `on_click` take type-safe callbacks and
 * return the concrete class.
 */

namespace lvgl {

/**
 * @brief The base class for all concrete widgets.
 *
 * Combines the fundamental Object lifecycle with functional Mixins.
 * Most properties are inherited from Object but wrapped to return the Derived
 * type.
 *
 * @tparam Derived The concrete widget class (e.g., Button, Label).
 */
template <typename Derived>
class Widget : public Object {
 public:
  /** @brief Reference to the concrete derived object. */
  Derived& self() { return *static_cast<Derived*>(this); }
  /** @brief Reference to the concrete derived object (const). */
  const Derived& self() const { return *static_cast<const Derived*>(this); }

  /** @cond */
  using Object::Object;  // Inherit constructors
  /** @endcond */

  /** @brief Default constructor. */
  Widget() : Object() {}

  /**
   * @brief Creates a widget as a child of another object.
   * @param parent The parent object.
   * @param ownership Lifecycle management strategy.
   */
  explicit Widget(Object* parent, Ownership ownership = Ownership::Default)
      : Object(parent, ownership) {}

  /**
   * @brief Creates a widget as a child of another object.
   * @param parent Reference to the parent object.
   * @param ownership Lifecycle management strategy.
   */
  explicit Widget(Object& parent, Ownership ownership = Ownership::Default)
      : Object(&parent, ownership) {}

  /**
   * @brief Wraps an existing raw LVGL object.
   * @param obj Raw pointer.
   * @param ownership Lifecycle management strategy.
   */
  explicit Widget(lv_obj_t* obj, Ownership ownership = Ownership::Default)
      : Object(obj, ownership) {}

  // --- Geometric Properties (Fluent Proxies) ---

  /** @brief Fluent wrapper for Object::set_x. */
  Derived& set_x(int32_t value) {
    Object::set_x(value);
    return self();
  }

  /** @brief Fluent wrapper for Object::set_y. */
  Derived& set_y(int32_t value) {
    Object::set_y(value);
    return self();
  }

  /** @brief Fluent wrapper for Object::set_pos. */
  Derived& set_pos(int32_t x, int32_t y) {
    Object::set_pos(x, y);
    return self();
  }

  /** @brief Fluent wrapper for Object::align. */
  Derived& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0) {
    Object::align(align, x_ofs, y_ofs);
    return self();
  }

  /** @brief Fluent wrapper for Object::align_to. */
  Derived& align_to(const Object& base, Align align, int32_t x_ofs = 0,
                    int32_t y_ofs = 0) {
    Object::align_to(base, align, x_ofs, y_ofs);
    return self();
  }

  /** @brief Fluent wrapper for Object::center. */
  Derived& center() {
    Object::center();
    return self();
  }

  // --- Sizable Properties (Fluent Proxies) ---

  /** @brief Fluent wrapper for Object::set_width. */
  Derived& set_width(int32_t value) {
    Object::set_width(value);
    return self();
  }

  /** @brief Fluent wrapper for Object::set_height. */
  Derived& set_height(int32_t value) {
    Object::set_height(value);
    return self();
  }

  /** @brief Fluent wrapper for Object::set_size. */
  Derived& set_size(int32_t width, int32_t height) {
    Object::set_size(width, height);
    return self();
  }

  // --- Object Lifecycle (Fluent Proxies) ---

  /** @brief Fluent wrapper for Object::add_flag. */
  /** @brief Fluent wrapper for Object::add_flag. */
  Derived& add_flag(ObjFlag f) {
    Object::add_flag(f);
    return self();
  }

  /** @brief Fluent wrapper for Object::remove_flag. */
  Derived& remove_flag(ObjFlag f) {
    Object::remove_flag(f);
    return self();
  }

  /** @brief Fluent wrapper for Object::add_state. */
  Derived& add_state(State state) {
    Object::add_state(state);
    return self();
  }

  /** @brief Fluent wrapper for Object::remove_state. */
  Derived& remove_state(State state) {
    Object::remove_state(state);
    return self();
  }

  // --- Observer Bindings (Fluent Proxies) ---

  /** @brief Conditional flag binding. */
  /** @brief Conditional flag binding. */
  Observer bind_flag_if_eq(Subject& subject, ObjFlag flag, int32_t ref_value) {
    return subject.bind_flag_if_eq(*this, flag, ref_value);
  }

  /** @brief Conditional state binding. */
  Observer bind_state_if_eq(Subject& subject, State state, int32_t ref_value) {
    return subject.bind_state_if_eq(*this, state, ref_value);
  }

  /** @brief Binds the Checked state. */
  Observer bind_checked(Subject& subject) {
    return subject.bind_checked(*this);
  }

  // --- Events (Fluent Proxies) ---

  /** @brief Fluent wrapper for Object::add_event_cb. */
  Derived& add_event_cb(EventCode event_code, EventCallback callback) {
    Object::add_event_cb(event_code, std::move(callback));
    return self();
  }

  /** @brief Shortcut for clicked event. */
  Derived& on_click(EventCallback cb) { return on_clicked(std::move(cb)); }

  /** @brief Observer for all events. */
  Derived& on_event(EventCallback cb) {
    return add_event_cb(EventCode::All, std::move(cb));
  }

  /** @brief Shortcut for clicked event. */
  Derived& on_clicked(EventCallback cb) {
    return add_event_cb(EventCode::Clicked, std::move(cb));
  }

  /** @brief Shortcut for pressed event. */
  Derived& on_pressed(EventCallback cb) {
    return add_event_cb(EventCode::Pressed, std::move(cb));
  }

  /** @brief Shortcut for released event. */
  Derived& on_released(EventCallback cb) {
    return add_event_cb(EventCode::Released, std::move(cb));
  }

  /** @brief Shortcut for long pressed event. */
  Derived& on_long_pressed(EventCallback cb) {
    return add_event_cb(EventCode::LongPressed, std::move(cb));
  }

  // --- Layout Shortcuts ---

  Derived& set_flex_flow(FlexFlow flow) {
    Object::set_flex_flow(flow);
    return self();
  }

  Derived& set_flex_align(FlexAlign main_place, FlexAlign cross_place,
                          FlexAlign track_place) {
    Object::set_flex_align(main_place, cross_place, track_place);
    return self();
  }

  Derived& set_flex_grow(uint8_t grow) {
    Object::set_flex_grow(grow);
    return self();
  }

  Derived& set_grid_dsc_array(const GridLayout& grid) {
    Object::set_grid_dsc_array(grid);
    return self();
  }

  Derived& set_grid_align(GridAlign column_align, GridAlign row_align) {
    Object::set_grid_align(column_align, row_align);
    return self();
  }

  Derived& set_grid_cell(GridAlign column_align, int32_t col_pos,
                         int32_t col_span, GridAlign row_align, int32_t row_pos,
                         int32_t row_span) {
    Object::set_grid_cell(column_align, col_pos, col_span, row_align, row_pos,
                          row_span);
    return self();
  }

  // --- Scroll Shortcuts ---

  Derived& scroll_to_view(AnimEnable anim_en) {
    Object::scroll_to_view(anim_en);
    return self();
  }

  Derived& scroll_by(int32_t x, int32_t y, AnimEnable anim_en) {
    Object::scroll_by(x, y, anim_en);
    return self();
  }
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_WIDGET_H_
