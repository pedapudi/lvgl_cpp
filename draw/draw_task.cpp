#include "draw_task.h"

namespace lvgl::draw {

// DrawTask Implementation

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

}  // namespace lvgl::draw
