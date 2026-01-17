#ifndef LVGL_CPP_DRAW_DRAW_H_
#define LVGL_CPP_DRAW_DRAW_H_

#include "lvgl.h"  // IWYU pragma: export

namespace lvgl {

// Enum wrapper for lv_draw_task_type_t
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
  Texture3D = LV_DRAW_TASK_TYPE_3D,
#endif
};

class DrawTask {
 public:
  explicit DrawTask(lv_draw_task_t* task);

  DrawTaskType get_type() const;
  void* get_draw_dsc() const;
  void get_area(lv_area_t* area) const;

  lv_draw_task_t* raw() const { return task_; }

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

  DrawTask add_task(const lv_area_t* coords, DrawTaskType type);
  void finalize_task_creation(DrawTask& t);

 private:
  lv_layer_t* layer_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_DRAW_DRAW_H_
