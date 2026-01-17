#include "object.h"

#include <vector>

#include "event.h"
#include "style.h"

namespace lvgl {

Object::Object(lv_obj_t* obj, Ownership ownership) : obj_(obj) {
  if (ownership == Ownership::Default) {
    owned_ = false;  // Wrapping an existing pointer defaults to view/weak
  } else {
    owned_ = (ownership == Ownership::Managed);
  }
  if (obj_) {
    install_delete_hook();
  }
}

Object::Object(lv_obj_t* obj, bool owned)
    : Object(obj, owned ? Ownership::Managed : Ownership::Unmanaged) {}

Object::Object() : Object((Object*)nullptr) {}

Object::Object(Object* parent, Ownership ownership) {
  lv_obj_t* parent_obj = parent ? parent->raw() : nullptr;
  obj_ = lv_obj_create(parent_obj);
  if (ownership == Ownership::Default) {
    owned_ = true;  // Creating a new child defaults to owned
  } else {
    owned_ = (ownership == Ownership::Managed);
  }
  install_delete_hook();
}

Object::~Object() {
  if (obj_) {
    // Always remove our specific callback to avoid re-entry or interfering with
    // other wrappers
    lv_obj_remove_event_cb_with_user_data(obj_, on_delete_event, this);
    if (owned_) {
      lv_obj_delete(obj_);
    }
    obj_ = nullptr;
  }
}

Object::Object(Object&& other) noexcept
    : obj_(other.obj_), owned_(other.owned_) {
  other.obj_ = nullptr;
  other.owned_ = false;
  if (obj_) {
    // Easier way: Remove old callback and add new one with new 'this'
    lv_obj_remove_event_cb_with_user_data(obj_, on_delete_event, &other);
    install_delete_hook();
  }
}

Object& Object::operator=(Object&& other) noexcept {
  if (this != &other) {
    if (obj_) {
      // ALWAYS remove our callback first.
      lv_obj_remove_event_cb_with_user_data(obj_, on_delete_event, this);
      if (owned_) {
        lv_obj_delete(obj_);
      }
    }

    obj_ = other.obj_;
    owned_ = other.owned_;
    other.obj_ = nullptr;
    other.owned_ = false;

    if (obj_) {
      // Remove the old wrapper's callback and install ours
      lv_obj_remove_event_cb_with_user_data(obj_, on_delete_event, &other);
      install_delete_hook();
    }
  }
  return *this;
}

lv_obj_t* Object::release() {
  lv_obj_t* ptr = obj_;
  owned_ = false;
  return ptr;
}

void Object::install_delete_hook() {
  lv_obj_add_event_cb(obj_, on_delete_event, LV_EVENT_DELETE, this);
}

void Object::on_delete_event(lv_event_t* e) {
  Object* self = static_cast<Object*>(lv_event_get_user_data(e));
  if (self) {
    self->obj_ = nullptr;  // Invalidate wrapper
  }
}

void Object::set_pos(int32_t x, int32_t y) {
  if (obj_) lv_obj_set_pos(obj_, x, y);
}

void Object::set_x(int32_t x) {
  if (obj_) lv_obj_set_x(obj_, x);
}

void Object::set_y(int32_t y) {
  if (obj_) lv_obj_set_y(obj_, y);
}

void Object::set_size(int32_t w, int32_t h) {
  if (obj_) lv_obj_set_size(obj_, w, h);
}

void Object::set_width(int32_t w) {
  if (obj_) lv_obj_set_width(obj_, w);
}

void Object::set_height(int32_t h) {
  if (obj_) lv_obj_set_height(obj_, h);
}

int32_t Object::get_x() const { return obj_ ? lv_obj_get_x(obj_) : 0; }

int32_t Object::get_y() const { return obj_ ? lv_obj_get_y(obj_) : 0; }

int32_t Object::get_width() const { return obj_ ? lv_obj_get_width(obj_) : 0; }

int32_t Object::get_height() const {
  return obj_ ? lv_obj_get_height(obj_) : 0;
}

void Object::align(Align align, int32_t x_ofs, int32_t y_ofs) {
  if (obj_) lv_obj_align(obj_, static_cast<lv_align_t>(align), x_ofs, y_ofs);
}

void Object::align_to(const Object& base, Align align, int32_t x_ofs,
                      int32_t y_ofs) {
  if (obj_ && base.raw())
    lv_obj_align_to(obj_, base.raw(), static_cast<lv_align_t>(align), x_ofs,
                    y_ofs);
}

void Object::center() {
  if (obj_) lv_obj_center(obj_);
}

void Object::add_flag(lv_obj_flag_t f) {
  if (obj_) lv_obj_add_flag(obj_, f);
}

void Object::remove_flag(lv_obj_flag_t f) {
  if (obj_) lv_obj_remove_flag(obj_, f);
}

bool Object::has_flag(lv_obj_flag_t f) const {
  return obj_ ? lv_obj_has_flag(obj_, f) : false;
}

void Object::add_state(lv_state_t state) {
  if (obj_) lv_obj_add_state(obj_, state);
}

void Object::remove_state(lv_state_t state) {
  if (obj_) lv_obj_remove_state(obj_, state);
}

bool Object::has_state(lv_state_t state) const {
  return obj_ ? lv_obj_has_state(obj_, state) : false;
}

void Object::add_event_cb(lv_event_code_t event_code, EventCallback callback) {
  if (!obj_) return;
  auto node = std::make_unique<CallbackNode>();
  node->callback = std::move(callback);
  lv_obj_add_event_cb(obj_, event_proxy, event_code, node.get());
  callback_nodes_.push_back(std::move(node));
}

void Object::add_style(Style& style, lv_style_selector_t selector) {
  if (obj_) lv_obj_add_style(obj_, style.raw(), selector);
}

void Object::remove_style(Style* style, lv_style_selector_t selector) {
  if (obj_) lv_obj_remove_style(obj_, style ? style->raw() : nullptr, selector);
}

void Object::set_style_anim_duration(uint32_t value,
                                     lv_style_selector_t selector) {
  if (obj_) lv_obj_set_style_anim_duration(obj_, value, selector);
}

void Object::set_style_text_align(lv_text_align_t value,
                                  lv_style_selector_t selector) {
  if (obj_) lv_obj_set_style_text_align(obj_, value, selector);
}

void Object::set_style_bg_color(lv_color_t value,
                                lv_style_selector_t selector) {
  if (obj_) lv_obj_set_style_bg_color(obj_, value, selector);
}

void Object::set_style_bg_opa(lv_opa_t value, lv_style_selector_t selector) {
  if (obj_) lv_obj_set_style_bg_opa(obj_, value, selector);
}

void Object::set_style_image_recolor_opa(lv_opa_t value,
                                         lv_style_selector_t selector) {
  if (obj_) lv_obj_set_style_image_recolor_opa(obj_, value, selector);
}

void Object::set_style_image_recolor(lv_color_t value,
                                     lv_style_selector_t selector) {
  if (obj_) lv_obj_set_style_image_recolor(obj_, value, selector);
}

void Object::set_style_bg_image_src(const void* value,
                                    lv_style_selector_t selector) {
  if (obj_) lv_obj_set_style_bg_image_src(obj_, value, selector);
}

void Object::event_proxy(lv_event_t* e) {
  CallbackNode* node = static_cast<CallbackNode*>(lv_event_get_user_data(e));
  if (node && node->callback) {
    Event evt(e);
    node->callback(evt);
  }
}

void Object::set_layout(uint32_t layout) {
  if (obj_) lv_obj_set_layout(obj_, layout);
}

bool Object::is_layout_positioned() const {
  return obj_ ? lv_obj_is_layout_positioned(obj_) : false;
}

void Object::mark_layout_as_dirty() {
  if (obj_) lv_obj_mark_layout_as_dirty(obj_);
}

void Object::update_layout() {
  if (obj_) lv_obj_update_layout(obj_);
}

void Object::set_flex_flow(lv_flex_flow_t flow) {
  if (obj_) lv_obj_set_flex_flow(obj_, flow);
}

void Object::set_flex_align(lv_flex_align_t main_place,
                            lv_flex_align_t cross_place,
                            lv_flex_align_t track_cross_place) {
  if (obj_)
    lv_obj_set_flex_align(obj_, main_place, cross_place, track_cross_place);
}

void Object::set_flex_grow(uint8_t grow) {
  if (obj_) lv_obj_set_flex_grow(obj_, grow);
}

void Object::set_grid_dsc_array(const int32_t col_dsc[],
                                const int32_t row_dsc[]) {
  if (obj_) lv_obj_set_grid_dsc_array(obj_, col_dsc, row_dsc);
}

void Object::set_grid_align(lv_grid_align_t column_align,
                            lv_grid_align_t row_align) {
  if (obj_) lv_obj_set_grid_align(obj_, column_align, row_align);
}

void Object::set_grid_cell(lv_grid_align_t column_align, int32_t col_pos,
                           int32_t col_span, lv_grid_align_t row_align,
                           int32_t row_pos, int32_t row_span) {
  if (obj_)
    lv_obj_set_grid_cell(obj_, column_align, col_pos, col_span, row_align,
                         row_pos, row_span);
}

}  // namespace lvgl
