#ifndef LVGL_CPP_DRAW_DRAW_H_
#define LVGL_CPP_DRAW_DRAW_H_

#include "lvgl.h"  // IWYU pragma: export

namespace lvgl {

class DrawTask {
 public:
  explicit DrawTask(lv_draw_task_t* task);

  lv_draw_task_type_t get_type() const;
  void* get_draw_dsc() const;
  void get_area(lv_area_t* area) const;

  // Note: Modifying draw task generally happens via the generic dsc void*
  // casted to specific structs. We can provide helpers like:
  // lv_draw_fill_dsc_t* as_fill_dsc();

 private:
  lv_draw_task_t* task_;
};

class Layer {
 public:
  explicit Layer(lv_layer_t* layer);

  void init();
  void reset();
  void* alloc_buf();

  DrawTask add_task(const lv_area_t* coords, lv_draw_task_type_t type);
  void finalize_task_creation(DrawTask& t);

 private:
  lv_layer_t* layer_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_DRAW_DRAW_H_
