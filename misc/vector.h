#ifndef LVGL_CPP_MISC_VECTOR_H_
#define LVGL_CPP_MISC_VECTOR_H_

#include "lvgl.h"

#if LV_USE_VECTOR_GRAPHIC

#include "../core/object.h"

namespace lvgl {

class VectorPath {
 public:
  explicit VectorPath(
      lv_vector_path_quality_t quality = LV_VECTOR_PATH_QUALITY_MEDIUM);
  virtual ~VectorPath();

  // Non-copyable due to ownership, move-only
  VectorPath(const VectorPath&) = delete;
  VectorPath& operator=(const VectorPath&) = delete;
  VectorPath(VectorPath&& other) noexcept;
  VectorPath& operator=(VectorPath&& other) noexcept;

  // Modifiers
  void copy_from(const VectorPath& other);
  void clear();

  // Building
  void move_to(float x, float y);
  void line_to(float x, float y);
  void quad_to(float cx, float cy, float x, float y);
  void cubic_to(float cx1, float cy1, float cx2, float cy2, float x, float y);
  void arc_to(float rx, float ry, float angle, bool large_arc, bool sweep,
              float x, float y);
  void close();

  // Shapes
  void append_rect(float x, float y, float w, float h, float rx, float ry);
  void append_circle(float cx, float cy, float rx, float ry);
  void append_arc(float cx, float cy, float radius, float start_angle,
                  float sweep, bool pie);
  void append_path(const VectorPath& other);

  // Transform
  void transform(const lv_matrix_t& matrix);
  void get_bounding_box(lv_area_t& area) const;

  lv_vector_path_t* raw() const { return path_; }

 private:
  lv_vector_path_t* path_ = nullptr;
};

class VectorDraw {
 public:
  explicit VectorDraw(lv_layer_t* layer);
  virtual ~VectorDraw();

  // Non-copyable, move-only
  VectorDraw(const VectorDraw&) = delete;
  VectorDraw& operator=(const VectorDraw&) = delete;
  VectorDraw(VectorDraw&& other) noexcept;
  VectorDraw& operator=(VectorDraw&& other) noexcept;

  // Properties
  void set_transform(const lv_matrix_t& matrix);
  void set_blend_mode(lv_vector_blend_t blend);

  // Fill
  void set_fill_color(lv_color32_t color);
  void set_fill_opa(lv_opa_t opa);
  void set_fill_rule(lv_vector_fill_t rule);
  void set_fill_image(const lv_draw_image_dsc_t& dsc);
  void set_fill_linear_gradient(float x1, float y1, float x2, float y2);
  void set_fill_radial_gradient(float cx, float cy, float radius);
  void set_fill_gradient_stops(const std::vector<lv_grad_stop_t>& stops);
  void set_fill_gradient_spread(lv_vector_gradient_spread_t spread);
  void set_fill_transform(const lv_matrix_t& matrix);

  // Stroke
  void set_stroke_color(lv_color32_t color);
  void set_stroke_opa(lv_opa_t opa);
  void set_stroke_width(float width);
  void set_stroke_dash(const std::vector<float>& dash_pattern);
  void set_stroke_cap(lv_vector_stroke_cap_t cap);
  void set_stroke_join(lv_vector_stroke_join_t join);
  void set_stroke_miter_limit(uint16_t limit);
  void set_stroke_linear_gradient(float x1, float y1, float x2, float y2);
  void set_stroke_radial_gradient(float cx, float cy, float radius);
  void set_stroke_gradient_stops(const std::vector<lv_grad_stop_t>& stops);
  void set_stroke_gradient_spread(lv_vector_gradient_spread_t spread);
  void set_stroke_transform(const lv_matrix_t& matrix);

  // Transformations
  void identity();
  void scale(float x, float y);
  void rotate(float degree);
  void translate(float x, float y);
  void skew(float x, float y);

  // Draw Operations
  void add_path(const VectorPath& path);
  void clear_area(const lv_area_t& rect);
  void draw();  // Execute drawing

  lv_draw_vector_dsc_t* raw() const { return dsc_; }

 private:
  lv_draw_vector_dsc_t* dsc_ = nullptr;
};

}  // namespace lvgl

#endif  // LV_USE_VECTOR_GRAPHIC
#endif  // LVGL_CPP_MISC_VECTOR_H_
