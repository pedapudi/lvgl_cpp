#include "group_proxy.h"

#include "lvgl.h"

namespace lvgl {

GroupProxy& GroupProxy::add() {
  if (obj_) {
    lv_group_t* group = lv_group_get_default();
    if (group) lv_group_add_obj(group, obj_);
  }
  return *this;
}

GroupProxy& GroupProxy::add(lv_group_t* group) {
  if (obj_ && group) lv_group_add_obj(group, obj_);
  return *this;
}

GroupProxy& GroupProxy::remove() {
  if (obj_) {
    lv_group_t* group = lv_obj_get_group(obj_);
    if (group) lv_group_remove_obj(obj_);
  }
  return *this;
}

GroupProxy& GroupProxy::focus() {
  if (obj_) lv_group_focus_obj(obj_);
  return *this;
}

bool GroupProxy::is_focused() const {
  if (!obj_) return false;
  lv_group_t* group = lv_obj_get_group(obj_);
  if (!group) return false;
  return lv_group_get_focused(group) == obj_;
}

}  // namespace lvgl
