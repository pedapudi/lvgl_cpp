#include "switch.h"

#if LV_USE_SWITCH

namespace lvgl {

Switch::Switch() : Switch(static_cast<Object*>(nullptr), Ownership::Managed) {}

Switch::Switch(Object* parent, Ownership ownership)
    : Widget(lv_switch_create(parent ? parent->raw() : nullptr), ownership) {}

Switch::Switch(Object& parent) : Switch(&parent) {}

Switch::Switch(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Switch& Switch::set_orientation(Orientation orientation) {
  if (raw())
    lv_switch_set_orientation(
        raw(), static_cast<lv_switch_orientation_t>(orientation));
  return *this;
}

Switch::Orientation Switch::get_orientation() const {
  return static_cast<Orientation>(raw() ? lv_switch_get_orientation(raw())
                                        : LV_SWITCH_ORIENTATION_AUTO);
}

Switch& Switch::on_value_changed(std::function<void(lvgl::Event&)> cb) {
  return add_event_cb(EventCode::ValueChanged, std::move(cb));
}

}  // namespace lvgl

#endif  // LV_USE_SWITCH
