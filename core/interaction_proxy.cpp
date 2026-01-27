#include "interaction_proxy.h"

namespace lvgl {

InteractionProxy::InteractionProxy(lv_obj_t* obj) : obj_(obj) {}

InteractionProxy& InteractionProxy::set_ext_click_area(int32_t size) {
  if (obj_) lv_obj_set_ext_click_area(obj_, size);
  return *this;
}

void InteractionProxy::get_click_area(Area& area) const {
  if (obj_) lv_obj_get_click_area(obj_, area.raw());
}

bool InteractionProxy::hit_test(const Point& point) const {
  return obj_ ? lv_obj_hit_test(obj_, point.raw()) : false;
}

bool InteractionProxy::is_editable() const {
  return obj_ ? lv_obj_is_editable(obj_) : false;
}

bool InteractionProxy::is_group_def() const {
  return obj_ ? lv_obj_is_group_def(obj_) : false;
}

}  // namespace lvgl
