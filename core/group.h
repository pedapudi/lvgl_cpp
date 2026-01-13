#ifndef LVGL_CPP_CORE_GROUP_H_
#define LVGL_CPP_CORE_GROUP_H_

#include "lvgl.h" // IWYU pragma: export
#include "object.h" // IWYU pragma: export

namespace lvgl {

class Group {
public:
  Group();
  ~Group();

  void add_obj(Object *obj);
  void remove_obj(Object *obj);
  void remove_all_objs();
  void focus_obj(Object *obj);
  void focus_next();
  void focus_prev();
  void focus_freeze(bool en);

  void set_default();
  static Group* get_default(); // Wraps lv_group_get_default(). Note: this might return a
                 // Group* that we don't own if we are not careful.
  // Actually, lv_group_get_default return lv_group_t*. We can't return Group*
  // unless we have a map or we return a temporary/lightweight wrapper. For now,
  // let's treat Group as an owning wrapper. If we wrap an existing group, we
  // need to be careful about deletion.

  void set_editing(bool edit);
  void set_wrap(bool en);

  lv_obj_t *get_focused();
  bool get_editing();
  bool get_wrap();
  uint32_t get_obj_count();

  lv_group_t *raw() const { return group_; }

private:
  lv_group_t *group_;
  bool owned_;

  // Private constructor for wrapping existing group without taking ownership
  // (or maybe default group shouldn't be deleted)
  explicit Group(lv_group_t *g, bool owned = false);
};

} // namespace lvgl

#endif // LVGL_CPP_CORE_GROUP_H_
