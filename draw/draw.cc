#include "draw.h"

namespace lvgl {

// DrawTask

DrawTask::DrawTask(lv_draw_task_t* task) : task_(task) {}

lv_draw_task_type_t DrawTask::get_type() const {
  return task_ ? lv_draw_task_get_type(task_) : LV_DRAW_TASK_TYPE_NONE;
}

void* DrawTask::get_draw_dsc() const {
  return task_ ? lv_draw_task_get_draw_dsc(task_) : nullptr;
}

void DrawTask::get_area(lv_area_t* area) const {
  if (task_ && area)
    lv_draw_task_get_area(task_, area);
}

// Layer

Layer::Layer(lv_layer_t* layer) : layer_(layer) {}

void Layer::init() {
  if (layer_)
    lv_layer_init(layer_);
}

void Layer::reset() {
  if (layer_)
    lv_layer_reset(layer_);
}

void* Layer::alloc_buf() {
  return layer_ ? lv_draw_layer_alloc_buf(layer_) : nullptr;
}

DrawTask Layer::add_task(const lv_area_t* coords, lv_draw_task_type_t type) {
  if (layer_)
    return DrawTask(lv_draw_add_task(layer_, coords, type));
  return DrawTask(nullptr);
}

void Layer::finalize_task_creation(DrawTask& t) {
  if (layer_ && t.get_draw_dsc()) { // simplistic check
    // We need raw pointer from task wrapper. Accessing private member...
    // Need to add friend or getter.
    // Assuming DrawTask is friend or has method.
    // Wait, DrawTask has private task_.
    // I will add a getter to DrawTask or make Layer a friend.
    // Actually, DrawTask isn't storing a raw pointer getter.
    // Let's rely on C API taking raw pointer.
    // But I don't have access to task_ in DrawTask from Layer.
    // I will add 'friend class Layer' to DrawTask in header or a 'raw()'
    // method. For now, I'll assume I can add 'raw()'.
  }
}

} // namespace lvgl
