#include "tree_proxy.h"

#include "object.h"

namespace lvgl {

TreeProxy::TreeProxy(lv_obj_t* obj) : obj_(obj) {}

TreeProxy& TreeProxy::swap(Object& other) {
  if (obj_ && other.raw()) {
    lv_obj_swap(obj_, other.raw());
  }
  return *this;
}

TreeProxy& TreeProxy::move_to_index(int32_t index) {
  if (obj_) {
    lv_obj_move_to_index(obj_, index);
  }
  return *this;
}

}  // namespace lvgl
