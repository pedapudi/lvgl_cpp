#include "tree_proxy.h"

#include "../lvgl_cpp.h"
#include "object.h"

namespace lvgl {

Object TreeProxy::get_parent() const {
  return Object(lv_obj_get_parent(obj_->raw()), Object::Ownership::Unmanaged);
}

Object TreeProxy::get_child(int32_t index) const {
  return Object(lv_obj_get_child(obj_->raw(), index),
                Object::Ownership::Unmanaged);
}

uint32_t TreeProxy::get_child_count() const {
  return lv_obj_get_child_count(obj_->raw());
}

TreeProxy& TreeProxy::clean() {
  lv_obj_clean(obj_->raw());
  return *this;
}

uint32_t TreeProxy::get_index() const { return lv_obj_get_index(obj_->raw()); }

TreeProxy& TreeProxy::move_to_foreground() {
  lv_obj_move_foreground(obj_->raw());
  return *this;
}

TreeProxy& TreeProxy::move_to_background() {
  lv_obj_move_background(obj_->raw());
  return *this;
}

TreeProxy& TreeProxy::set_parent(Object& parent) {
  lv_obj_set_parent(obj_->raw(), parent.raw());
  return *this;
}

void TreeProxy::swap(Object& other) { lv_obj_swap(obj_->raw(), other.raw()); }

void TreeProxy::move_to_index(int32_t index) {
  lv_obj_move_to_index(obj_->raw(), index);
}

}  // namespace lvgl
