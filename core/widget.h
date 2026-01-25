#pragma once

#include "event.h"
#include "lvgl.h"  // IWYU pragma: export
#include "lvgl.h"
#include "mixins/event_source.h"
#include "mixins/positionable.h"
#include "mixins/sizable.h"  // Keep this include as Sizable is still used in the class body
#include "object.h"

namespace lvgl {

/**
 * @brief The base class for all concrete widgets.
 * Combines the fundamental Object lifecycle with functional Mixins.
 * Uses CRTP to allow fluent method chaining that returns the Derived type.
 *
 * @tparam Derived The concrete widget class (e.g., Button, Label).
 */
template <typename Derived>
class Widget : public Object,
               public EventSource<Derived>,
               public Positionable<Derived>,
               public Sizable<Derived> {  // Sizable is still inherited
 public:
  using Object::Object;  // Inherit constructors
  // Explicitly forward constructors to ensure they are inherited by derived
  // classes (Double 'using' inheritance is not standard compliant for
  // constructors)
  Widget() : Object() {}

  explicit Widget(Object* parent, Ownership ownership = Ownership::Default)
      : Object(parent, ownership) {}

  explicit Widget(Object& parent, Ownership ownership = Ownership::Default)
      : Object(&parent, ownership) {}

  explicit Widget(lv_obj_t* obj, Ownership ownership = Ownership::Default)
      : Object(obj, ownership) {}

  // --- Positionable Forwarding ---
  Derived& set_x(int32_t value) { return Positionable<Derived>::set_x(value); }
  Derived& set_y(int32_t value) { return Positionable<Derived>::set_y(value); }
  Derived& set_pos(int32_t x, int32_t y) {
    return Positionable<Derived>::set_pos(x, y);
  }
  Derived& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0) {
    return Positionable<Derived>::align(align, x_ofs, y_ofs);
  }
  Derived& align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0) {
    return Positionable<Derived>::align(align, x_ofs, y_ofs);
  }

  Derived& center() { return Positionable<Derived>::center(); }
  int32_t get_x() const { return Positionable<Derived>::get_x(); }
  int32_t get_y() const { return Positionable<Derived>::get_y(); }

  // --- Sizable Forwarding ---
  Derived& set_width(int32_t value) {
    return Sizable<Derived>::set_width(value);
  }
  Derived& set_height(int32_t value) {
    return Sizable<Derived>::set_height(value);
  }
  Derived& set_size(int32_t width, int32_t height) {
    return Sizable<Derived>::set_size(width, height);
  }
  int32_t get_width() const { return Sizable<Derived>::get_width(); }
  int32_t get_height() const { return Sizable<Derived>::get_height(); }

  // --- Object Lifecycle Forwarding (Fluent) ---
  Derived& add_flag(lv_obj_flag_t f) {
    Object::add_flag(f);
    return *static_cast<Derived*>(this);
  }
  Derived& remove_flag(lv_obj_flag_t f) {
    Object::remove_flag(f);
    return *static_cast<Derived*>(this);
  }
  Derived& add_flag(ObjFlag f) {
    Object::add_flag(f);
    return *static_cast<Derived*>(this);
  }
  Derived& remove_flag(ObjFlag f) {
    Object::remove_flag(f);
    return *static_cast<Derived*>(this);
  }
  Derived& add_state(State state) {
    Object::add_state(state);
    return *static_cast<Derived*>(this);
  }
  Derived& remove_state(State state) {
    Object::remove_state(state);
    return *static_cast<Derived*>(this);
  }
  Derived& add_state(lv_state_t state) {
    Object::add_state(state);
    return *static_cast<Derived*>(this);
  }
  Derived& remove_state(lv_state_t state) {
    Object::remove_state(state);
    return *static_cast<Derived*>(this);
  }

  // --- EventSource Forwarding ---
  Derived& add_event_cb(typename EventSource<Derived>::EventCallback cb,
                        lv_event_code_t filter) {
    return EventSource<Derived>::add_event_cb(cb, filter);
  }
  Derived& on_click(typename EventSource<Derived>::EventCallback cb) {
    return EventSource<Derived>::on_click(cb);
  }
  Derived& on_event(typename EventSource<Derived>::EventCallback cb) {
    return EventSource<Derived>::on_event(cb);
  }
  Derived& on_clicked(typename EventSource<Derived>::EventCallback cb) {
    return EventSource<Derived>::on_clicked(cb);
  }
  Derived& on_pressed(typename EventSource<Derived>::EventCallback cb) {
    return EventSource<Derived>::on_pressed(cb);
  }
  Derived& on_released(typename EventSource<Derived>::EventCallback cb) {
    return EventSource<Derived>::on_released(cb);
  }
  Derived& on_long_pressed(typename EventSource<Derived>::EventCallback cb) {
    return EventSource<Derived>::on_long_pressed(cb);
  }

  /**
   * @brief Returns a reference to the derived type.
   * Useful if generic access to the concrete type is needed.
   */
  Derived& self() { return *static_cast<Derived*>(this); }

  const Derived& self() const { return *static_cast<const Derived*>(this); }
};

}  // namespace lvgl
