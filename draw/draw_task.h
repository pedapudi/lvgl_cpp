#pragma once

#include "lvgl.h"

namespace lvgl::draw {

/**
 * @brief C++ wrapper for lv_draw_task_type_t.
 */
enum class DrawTaskType {
  None = LV_DRAW_TASK_TYPE_NONE,
  Fill = LV_DRAW_TASK_TYPE_FILL,
  Border = LV_DRAW_TASK_TYPE_BORDER,
  BoxShadow = LV_DRAW_TASK_TYPE_BOX_SHADOW,
  Letter = LV_DRAW_TASK_TYPE_LETTER,
  Label = LV_DRAW_TASK_TYPE_LABEL,
  Image = LV_DRAW_TASK_TYPE_IMAGE,
  Layer = LV_DRAW_TASK_TYPE_LAYER,
  Line = LV_DRAW_TASK_TYPE_LINE,
  Arc = LV_DRAW_TASK_TYPE_ARC,
  Triangle = LV_DRAW_TASK_TYPE_TRIANGLE,
  MaskRectangle = LV_DRAW_TASK_TYPE_MASK_RECTANGLE,
  MaskBitmap = LV_DRAW_TASK_TYPE_MASK_BITMAP,
#if LV_USE_VECTOR_GRAPHIC
  Vector = LV_DRAW_TASK_TYPE_VECTOR,
#endif
#if LV_USE_3DTEXTURE
  Task3D = LV_DRAW_TASK_TYPE_3D,
#endif
};

/**
 * @brief C++ wrapper for lv_draw_task_t.
 */
class DrawTask {
 public:
  explicit DrawTask(lv_draw_task_t* task);

  DrawTaskType get_type() const;
  void* get_draw_dsc() const;
  void get_area(lv_area_t* area) const;

  lv_draw_task_t* raw() const { return task_; }

 private:
  lv_draw_task_t* task_ = nullptr;
};

}  // namespace lvgl::draw
