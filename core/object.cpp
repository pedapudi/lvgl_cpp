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
    // Clean up event callbacks registered by this wrapper instance
    for (auto& node : callback_nodes_) {
      lv_obj_remove_event_cb_with_user_data(obj_, event_proxy, node.get());
    }

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

// --- Flags & States ---

void Object::add_flag(lv_obj_flag_t f) {
  if (obj_) lv_obj_add_flag(obj_, f);
}

void Object::remove_flag(lv_obj_flag_t f) {
  if (obj_) lv_obj_remove_flag(obj_, f);
}

bool Object::has_flag(lv_obj_flag_t f) const {
  return obj_ ? lv_obj_has_flag(obj_, f) : false;
}

void Object::add_state(State state) {
  if (obj_) lv_obj_add_state(obj_, static_cast<lv_state_t>(state));
}

void Object::remove_state(State state) {
  if (obj_) lv_obj_remove_state(obj_, static_cast<lv_state_t>(state));
}

bool Object::has_state(State state) const {
  return obj_ ? lv_obj_has_state(obj_, static_cast<lv_state_t>(state)) : false;
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

// --- Events ---

void Object::event_proxy(lv_event_t* e) {
  auto* node = static_cast<CallbackNode*>(lv_event_get_user_data(e));
  if (node && node->callback) {
    Event event(e);
    node->callback(event);
  }
}

void Object::add_event_cb(lv_event_code_t event_code, EventCallback callback) {
  if (!obj_) return;
  auto node = std::make_unique<CallbackNode>();
  node->event_code = event_code;
  node->callback = std::move(callback);
  lv_obj_add_event_cb(obj_, event_proxy, event_code, node.get());
  callback_nodes_.push_back(std::move(node));
}

// --- Styles ---

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

}  // namespace lvgl
