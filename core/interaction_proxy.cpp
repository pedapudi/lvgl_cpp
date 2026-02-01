#include "interaction_proxy.h"

#include "../lvgl_cpp.h"
#include "group.h"

namespace lvgl {

InteractionProxy& InteractionProxy::set_clickable(bool en) {
  if (en) {
    lv_obj_add_flag(obj_->raw(), LV_OBJ_FLAG_CLICKABLE);
  } else {
    lv_obj_remove_flag(obj_->raw(), LV_OBJ_FLAG_CLICKABLE);
  }
  return *this;
}

bool InteractionProxy::is_clickable() const {
  return lv_obj_has_flag(obj_->raw(), LV_OBJ_FLAG_CLICKABLE);
}

bool InteractionProxy::hit_test(const Point& p) const {
  lv_point_t pt = {static_cast<lv_coord_t>(p.x()),
                   static_cast<lv_coord_t>(p.y())};
  return lv_obj_hit_test(obj_->raw(), &pt);
}

bool InteractionProxy::is_focused() const {
  lv_group_t* g = lv_obj_get_group(obj_->raw());
  if (!g) return false;
  return lv_group_get_focused(g) == obj_->raw();
}

InteractionProxy& InteractionProxy::set_group(Group& group) {
  return set_group(group.raw());
}

InteractionProxy& InteractionProxy::set_group(lv_group_t* group) {
  if (obj_ && group) lv_group_add_obj(group, obj_->raw());
  return *this;
}

Group InteractionProxy::get_group() const {
  return Group(lv_obj_get_group(obj_->raw()), Group::Ownership::Unmanaged);
}

void InteractionProxy::set_ext_click_area(int32_t area) {
  lv_obj_set_ext_click_area(obj_->raw(), area);
}

}  // namespace lvgl
