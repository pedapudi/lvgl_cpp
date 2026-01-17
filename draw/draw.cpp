#include "draw.h"

namespace lvgl {

// DrawTask

DrawTask::DrawTask(lv_draw_task_t* task) : task_(task) {}

DrawTaskType DrawTask::get_type() const {
  return task_ ? static_cast<DrawTaskType>(lv_draw_task_get_type(task_))
               : DrawTaskType::None;
}

void* DrawTask::get_draw_dsc() const {
  return task_ ? lv_draw_task_get_draw_dsc(task_) : nullptr;
}

void DrawTask::get_area(lv_area_t* area) const {
  if (task_ && area) lv_draw_task_get_area(task_, area);
}

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
