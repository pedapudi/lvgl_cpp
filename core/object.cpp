#include "object.h"

#include <vector>

#include "../misc/style.h"
#include "event.h"
#include "lvgl.h"

namespace lvgl {

Object::Object() : obj_(lv_obj_create(nullptr)), owned_(true) {
  if (obj_) {
    install_delete_hook();
  }
}

Object::Object(lv_obj_t* obj, Ownership ownership) : obj_(obj) {
  if (ownership == Ownership::Default) {
    if (obj_) owned_ = false;  // Default for wrapping existing obj is Unmanaged
  } else {
    owned_ = (ownership == Ownership::Managed);
  }
  if (obj_) {
    install_delete_hook();
  }
}

Object::Object(Object* parent, Ownership ownership) {
  obj_ = lv_obj_create(parent ? parent->raw() : nullptr);
  if (ownership == Ownership::Default) {
    owned_ = true;  // Default for new child is Owned
  } else {
    owned_ = (ownership == Ownership::Managed);
  }
  if (obj_) {
    install_delete_hook();
  }
}

Object::~Object() {
  if (obj_) {
    // Clean up event callbacks registered by this wrapper instance
    for (auto& node : callback_nodes_) {
      lv_obj_remove_event_cb_with_user_data(obj_, event_proxy, node.get());
    }
    // Remove the internal delete hook to prevent use-after-free of 'this'
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

// --- Object Tree Management ---

void Object::clean() {
  if (obj_) lv_obj_clean(obj_);
}

Object Object::get_parent() const {
  if (!obj_)
    return Object(static_cast<lv_obj_t*>(nullptr), Ownership::Unmanaged);
  lv_obj_t* parent = lv_obj_get_parent(obj_);
  return Object(parent, Ownership::Unmanaged);
}

Object Object::get_child(int32_t index) const {
  if (!obj_)
    return Object(static_cast<lv_obj_t*>(nullptr), Ownership::Unmanaged);
  lv_obj_t* child = lv_obj_get_child(obj_, index);
  return Object(child, Ownership::Unmanaged);
}

uint32_t Object::get_child_count() const {
  return obj_ ? lv_obj_get_child_count(obj_) : 0;
}

void Object::set_parent(Object& parent) {
  if (obj_ && parent.raw()) lv_obj_set_parent(obj_, parent.raw());
}

void Object::set_parent(lv_obj_t* parent) {
  if (obj_ && parent) lv_obj_set_parent(obj_, parent);
}

int32_t Object::get_index() const { return obj_ ? lv_obj_get_index(obj_) : -1; }

void Object::move_foreground() {
  if (obj_) lv_obj_move_foreground(obj_);
}

void Object::move_background() {
  if (obj_) lv_obj_move_background(obj_);
}

void Object::delete_async() {
  if (obj_) {
    owned_ = false;  // Prevent double-delete
    lv_obj_delete_async(obj_);
  }
}

void Object::install_delete_hook() {
  fprintf(stderr, "Install hook: obj=%p, wrapper=%p\n", obj_, this);
  lv_obj_add_event_cb(obj_, on_delete_event, LV_EVENT_DELETE, this);
}

void Object::on_delete_event(lv_event_t* e) {
  Object* self = static_cast<Object*>(lv_event_get_user_data(e));
  fprintf(stderr, "On delete event trigger: obj=%p, wrapper=%p\n",
          lv_event_get_target(e), self);
  if (self) {
    self->obj_ = nullptr;  // Invalidate wrapper
  }
}

// --- Flags & States ---

void Object::add_flag(lv_obj_flag_t f) {
  if (obj_) lv_obj_add_flag(obj_, f);
}

void Object::add_flag(ObjFlag f) { add_flag(static_cast<lv_obj_flag_t>(f)); }

void Object::remove_flag(lv_obj_flag_t f) {
  if (obj_) lv_obj_remove_flag(obj_, f);
}

void Object::remove_flag(ObjFlag f) {
  remove_flag(static_cast<lv_obj_flag_t>(f));
}

bool Object::has_flag(lv_obj_flag_t f) const {
  return obj_ ? lv_obj_has_flag(obj_, f) : false;
}

bool Object::has_flag(ObjFlag f) const {
  return has_flag(static_cast<lv_obj_flag_t>(f));
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

// --- Scroll ---

// --- Other Properties ---

void Object::set_base_dir(lv_base_dir_t dir) {
  if (obj_) lv_obj_set_style_base_dir(obj_, dir, LV_PART_MAIN);
}

void Object::set_base_dir(BaseDir dir) {
  set_base_dir(static_cast<lv_base_dir_t>(dir));
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

void Object::add_event_cb(EventCode event_code, EventCallback callback) {
  add_event_cb(static_cast<lv_event_code_t>(event_code), std::move(callback));
}

// --- Styles ---

void Object::add_style(Style& style, lv_style_selector_t selector) {
  if (obj_) lv_obj_add_style(obj_, style.raw(), selector);
}

void Object::remove_style(Style* style, lv_style_selector_t selector) {
  if (obj_) lv_obj_remove_style(obj_, style ? style->raw() : nullptr, selector);
}

// --- Layout & Scroll ---

}  // namespace lvgl
