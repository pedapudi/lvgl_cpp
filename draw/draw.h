#ifndef LVGL_CPP_DRAW_DRAW_H_
#define LVGL_CPP_DRAW_DRAW_H_

#include "draw_buf.h"
#include "draw_task.h"
#include "lvgl.h"

namespace lvgl {

using draw::DrawBuf;
using draw::DrawTask;
using draw::DrawTaskType;

class Layer {
 public:
  explicit Layer(lv_layer_t* layer);

  void init();
  void reset();
  void* alloc_buf();

  DrawTask add_task(const lv_area_t* coords, DrawTaskType type);
  void finalize_task_creation(DrawTask& t);

 private:
  lv_layer_t* layer_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_DRAW_DRAW_H_
