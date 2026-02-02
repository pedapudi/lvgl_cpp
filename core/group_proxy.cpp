#include "group_proxy.h"

#include "group.h"
#include "lvgl.h"
#include "object.h"

namespace lvgl {

GroupProxy& GroupProxy::add() {
  if (obj_ && obj_->raw()) {
    lv_group_t* group = lv_group_get_default();
    if (group) lv_group_add_obj(group, obj_->raw());
  }
  return *this;
}

GroupProxy& GroupProxy::add(lv_group_t* group) {
  if (obj_ && obj_->raw() && group) lv_group_add_obj(group, obj_->raw());
  return *this;
}

GroupProxy& GroupProxy::add(Group& group) { return add(group.raw()); }

GroupProxy& GroupProxy::remove() {
  if (obj_ && obj_->raw()) {
    lv_group_t* group = lv_obj_get_group(obj_->raw());
    if (group) lv_group_remove_obj(obj_->raw());
  }
  return *this;
}

GroupProxy& GroupProxy::focus() {
  if (obj_ && obj_->raw()) lv_group_focus_obj(obj_->raw());
  return *this;
}

bool GroupProxy::is_focused() const {
  if (!obj_ || !obj_->raw()) return false;
  lv_group_t* group = lv_obj_get_group(obj_->raw());
  if (!group) return false;
  return lv_group_get_focused(group) == obj_->raw();
}

}  // namespace lvgl
