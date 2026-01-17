#include "draw.h"

namespace lvgl {

// DrawTask implementation moved to draw_task.cpp
// Layer

Layer::Layer(lv_layer_t* layer) : layer_(layer) {}

void Layer::init() {
  if (layer_) lv_layer_init(layer_);
}

void Layer::reset() {
  if (layer_) lv_layer_reset(layer_);
}

void* Layer::alloc_buf() {
  return layer_ ? lv_draw_layer_alloc_buf(layer_) : nullptr;
}

DrawTask Layer::add_task(const lv_area_t* coords, DrawTaskType type) {
  if (layer_)
    return DrawTask(lv_draw_add_task(layer_, coords,
                                     static_cast<lv_draw_task_type_t>(type)));
  return DrawTask(nullptr);
}

void Layer::finalize_task_creation(DrawTask& t) {
  if (layer_ && t.raw()) {
    lv_draw_finalize_task_creation(layer_, t.raw());
  }
}

}  // namespace lvgl
