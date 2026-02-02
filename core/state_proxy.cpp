#include "state_proxy.h"

#include "object.h"

namespace lvgl {

StateProxy& StateProxy::add(State state) {
  if (obj_ && obj_->raw())
    lv_obj_add_state(obj_->raw(), static_cast<lv_state_t>(state));
  return *this;
}

StateProxy& StateProxy::remove(State state) {
  if (obj_ && obj_->raw())
    lv_obj_remove_state(obj_->raw(), static_cast<lv_state_t>(state));
  return *this;
}

bool StateProxy::is(State state) const {
  return (obj_ && obj_->raw())
             ? lv_obj_has_state(obj_->raw(), static_cast<lv_state_t>(state))
             : false;
}

StateProxy& StateProxy::set(State state, bool value) {
  if (value)
    add(state);
  else
    remove(state);
  return *this;
}

}  // namespace lvgl
