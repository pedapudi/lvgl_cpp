#pragma once

#include "event.h"
#include "lvgl.h"
#include "object.h"
#include "observer.h"

namespace lvgl {

/**
 * @brief The base class for all concrete widgets.
 * Combines the fundamental Object lifecycle with functional Mixins.
 * Uses CRTP to allow fluent method chaining that returns the Derived type.
 *
 * @tparam Derived The concrete widget class (e.g., Button, Label).
 */
template <typename Derived>
class Widget : public Object {
 public:
  using Object::Object;  // Inherit constructors

  Widget() : Object() {}

  explicit Widget(Object* parent, Ownership ownership = Ownership::Default)
      : Object(parent, ownership) {}

  explicit Widget(Object& parent, Ownership ownership = Ownership::Default)
      : Object(&parent, ownership) {}

  explicit Widget(lv_obj_t* obj, Ownership ownership = Ownership::Default)
      : Object(obj, ownership) {}

  // --- Geometric Properties (Fluent Proxies) ---

  Derived& set_x(int32_t value) {
    Object::set_x(value);
    return self();
  }

  Derived& set_y(int32_t value) {
    Object::set_y(value);
    return self();
  }

  Derived& set_pos(int32_t x, int32_t y) {
    Object::set_pos(x, y);
    return self();
  }

  Derived& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0) {
    Object::align(align, x_ofs, y_ofs);
    return self();
  }

  Derived& align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0) {
    Object::align(align, x_ofs, y_ofs);
    return self();
  }

  Derived& align_to(const Object& base, Align align, int32_t x_ofs = 0,
                    int32_t y_ofs = 0) {
    Object::align_to(base, align, x_ofs, y_ofs);
    return self();
  }

  Derived& align_to(const Object& base, lv_align_t align, int32_t x_ofs = 0,
                    int32_t y_ofs = 0) {
    Object::align_to(base, align, x_ofs, y_ofs);
    return self();
  }

  Derived& center() {
    Object::center();
    return self();
  }

  // --- Sizable Properties (Fluent Proxies) ---

  Derived& set_width(int32_t value) {
    Object::set_width(value);
    return self();
  }

  Derived& set_height(int32_t value) {
    Object::set_height(value);
    return self();
  }

  Derived& set_size(int32_t width, int32_t height) {
    Object::set_size(width, height);
    return self();
  }

  // --- Object Lifecycle (Fluent Proxies) ---

  Derived& add_flag(lv_obj_flag_t f) {
    Object::add_flag(f);
    return self();
  }

  Derived& remove_flag(lv_obj_flag_t f) {
    Object::remove_flag(f);
    return self();
  }

  Derived& add_flag(ObjFlag f) {
    Object::add_flag(f);
    return self();
  }

  Derived& remove_flag(ObjFlag f) {
    Object::remove_flag(f);
    return self();
  }

  Derived& add_state(State state) {
    Object::state().add(state);
    return self();
  }

  Derived& remove_state(State state) {
    Object::state().remove(state);
    return self();
  }

  Derived& add_state(lv_state_t state) {
    if (obj_) lv_obj_add_state(obj_, state);
    return self();
  }

  Derived& remove_state(lv_state_t state) {
    if (obj_) lv_obj_remove_state(obj_, state);
    return self();
  }

  // --- Observer Bindings (Fluent Proxies) ---

  Observer bind_flag_if_eq(Subject& subject, ObjFlag flag, int32_t ref_value) {
    return subject.bind_flag_if_eq(*this, flag, ref_value);
  }

  Observer bind_flag_if_eq(Subject& subject, lv_obj_flag_t flag,
                           int32_t ref_value) {
    return subject.bind_flag_if_eq(*this, flag, ref_value);
  }

  Observer bind_state_if_eq(Subject& subject, State state, int32_t ref_value) {
    return subject.bind_state_if_eq(*this, state, ref_value);
  }

  Observer bind_state_if_eq(Subject& subject, lv_state_t state,
                            int32_t ref_value) {
    return subject.bind_state_if_eq(*this, state, ref_value);
  }

  Observer bind_checked(Subject& subject) {
    return subject.bind_checked(*this);
  }

  // --- Events (Fluent Proxies) ---

  Derived& add_event_cb(lv_event_code_t event_code, EventCallback callback) {
    Object::event().add_cb(event_code, std::move(callback));
    return self();
  }

  Derived& add_event_cb(EventCode event_code, EventCallback callback) {
    Object::event().add_cb(event_code, std::move(callback));
    return self();
  }

  Derived& on_click(EventCallback cb) {
    Object::event().on_clicked(std::move(cb));
    return self();
  }

  Derived& on_event(EventCallback cb) {
    Object::event().on_all(std::move(cb));
    return self();
  }

  Derived& on_clicked(EventCallback cb) {
    Object::event().on_clicked(std::move(cb));
    return self();
  }

  Derived& on_pressed(EventCallback cb) {
    Object::event().on_pressed(std::move(cb));
    return self();
  }

  Derived& on_released(EventCallback cb) {
    Object::event().on_released(std::move(cb));
    return self();
  }

  Derived& on_long_pressed(EventCallback cb) {
    Object::event().on_long_pressed(std::move(cb));
    return self();
  }

  /**
   * @brief Returns a reference to the derived type.
   * Useful if generic access to the concrete type is needed.
   */
  Derived& self() { return *static_cast<Derived*>(this); }

  const Derived& self() const { return *static_cast<const Derived*>(this); }
};

}  // namespace lvgl
