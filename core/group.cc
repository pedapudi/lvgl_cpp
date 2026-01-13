#include "group.h"

namespace lvgl {

Group::Group() {
  group_ = lv_group_create();
  owned_ = true;
}

Group::Group(lv_group_t *g, bool owned) : group_(g), owned_(owned) {}

Group::~Group() {
  if (owned_ && group_) {
    lv_group_delete(group_);
  }
}

void Group::add_obj(Object *obj) {
  if (group_ && obj)
    lv_group_add_obj(group_, obj->raw());
}

void Group::remove_obj(Object *obj) {
  if (obj)
    lv_group_remove_obj(obj->raw());
}

void Group::remove_all_objs() {
  if (group_)
    lv_group_remove_all_objs(group_);
}

void Group::focus_obj(Object *obj) {
  if (obj)
    lv_group_focus_obj(obj->raw());
}

void Group::focus_next() {
  if (group_)
    lv_group_focus_next(group_);
}

void Group::focus_prev() {
  if (group_)
    lv_group_focus_prev(group_);
}

void Group::focus_freeze(bool en) {
  if (group_)
    lv_group_focus_freeze(group_, en);
}

void Group::set_default() {
  if (group_)
    lv_group_set_default(group_);
}

void Group::set_editing(bool edit) {
  if (group_)
    lv_group_set_editing(group_, edit);
}

void Group::set_wrap(bool en) {
  if (group_)
    lv_group_set_wrap(group_, en);
}

lv_obj_t *Group::get_focused() {
  return group_ ? lv_group_get_focused(group_) : nullptr;
}

bool Group::get_editing() {
  return group_ ? lv_group_get_editing(group_) : false;
}

bool Group::get_wrap() { return group_ ? lv_group_get_wrap(group_) : false; }

uint32_t Group::get_obj_count() {
  return group_ ? lv_group_get_obj_count(group_) : 0;
}

// Static method implementation needs care.
// We return a pointer to a static global wrapper or create a temporary one?
// Problem: if we return new Group(ptr), user might delete it.
// Ideally we shouldn't return Group* for get_default since we don't track it.
// I will comment it out or implement it safely later.
// For now, let's implement a 'wrap' static method if needed, but not expose
// get_default returning a convenient pointer object yet to avoid memory
// confusion. Or better, just return a non-owning Group. But who owns the
// pointer? I will skip get_default wrapper for now or return void. Attempting
// to implement it safely is out of scope for quick wrapper.

} // namespace lvgl
