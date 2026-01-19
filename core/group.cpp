#include "group.h"

#include <utility>

namespace lvgl {

Group::Group(Ownership ownership)
    : group_(lv_group_create()), ownership_(ownership) {}

Group::Group(lv_group_t* group, Ownership ownership)
    : group_(group), ownership_(ownership) {}

Group::~Group() {
  if (ownership_ == Ownership::Managed && group_) {
    lv_group_delete(group_);
  }
}

Group::Group(Group&& other) noexcept
    : group_(other.group_), ownership_(other.ownership_) {
  other.group_ = nullptr;
  other.ownership_ = Ownership::Unmanaged;
}

Group& Group::operator=(Group&& other) noexcept {
  if (this != &other) {
    if (ownership_ == Ownership::Managed && group_) {
      lv_group_delete(group_);
    }
    group_ = other.group_;
    ownership_ = other.ownership_;
    other.group_ = nullptr;
    other.ownership_ = Ownership::Unmanaged;
  }
  return *this;
}

void Group::add_obj(Object& obj) {
  if (group_) lv_group_add_obj(group_, obj.raw());
}

void Group::add_obj(lv_obj_t* obj) {
  if (group_) lv_group_add_obj(group_, obj);
}

void Group::remove_obj(Object& obj) {
  if (group_) lv_group_remove_obj(obj.raw());
}

void Group::remove_obj(lv_obj_t* obj) {
  if (group_) lv_group_remove_obj(obj);
}

void Group::remove_all_objs() {
  if (group_) lv_group_remove_all_objs(group_);
}

void Group::focus_obj(Object& obj) {
  if (group_) lv_group_focus_obj(obj.raw());
}

void Group::focus_obj(lv_obj_t* obj) {
  if (group_) lv_group_focus_obj(obj);
}

void Group::focus_next() {
  if (group_) lv_group_focus_next(group_);
}

void Group::focus_prev() {
  if (group_) lv_group_focus_prev(group_);
}

void Group::focus_freeze(bool en) {
  if (group_) lv_group_focus_freeze(group_, en);
}

void Group::set_editing(bool edit) {
  if (group_) lv_group_set_editing(group_, edit);
}

lv_obj_t* Group::get_focused() const {
  return group_ ? lv_group_get_focused(group_) : nullptr;
}

lv_group_t* Group::raw() const { return group_; }

// --- Static Helpers ---

Group Group::get_default() {
  return Group(lv_group_get_default(), Ownership::Unmanaged);
}

void Group::set_default(Group& group) { lv_group_set_default(group.raw()); }

void Group::swap(Group& other) {
  std::swap(group_, other.group_);
  std::swap(ownership_, other.ownership_);
}

}  // namespace lvgl
