#include "vector.h"

#if LV_USE_VECTOR_GRAPHIC

namespace lvgl {

// --- VectorPath ---

VectorPath::VectorPath(lv_vector_path_quality_t quality) {
  path_ = lv_vector_path_create(quality);
}

VectorPath::~VectorPath() {
  if (path_) lv_vector_path_delete(path_);
}

VectorPath::VectorPath(VectorPath&& other) noexcept : path_(other.path_) {
  other.path_ = nullptr;
}

VectorPath& VectorPath::operator=(VectorPath&& other) noexcept {
  if (this != &other) {
    if (path_) lv_vector_path_delete(path_);
    path_ = other.path_;
    other.path_ = nullptr;
  }
  return *this;
}

void VectorPath::copy_from(const VectorPath& other) {
  if (path_ && other.path_) {
    lv_vector_path_copy(path_, other.path_);
  }
}

void VectorPath::clear() {
  if (path_) lv_vector_path_clear(path_);
}

void VectorPath::move_to(float x, float y) {
  lv_fpoint_t p = {x, y};
  if (path_) lv_vector_path_move_to(path_, &p);
}

void VectorPath::line_to(float x, float y) {
  lv_fpoint_t p = {x, y};
  if (path_) lv_vector_path_line_to(path_, &p);
}

void VectorPath::quad_to(float cx, float cy, float x, float y) {
  lv_fpoint_t p1 = {cx, cy};
  lv_fpoint_t p2 = {x, y};
  if (path_) lv_vector_path_quad_to(path_, &p1, &p2);
}

void VectorPath::cubic_to(float cx1, float cy1, float cx2, float cy2, float x,
                          float y) {
  lv_fpoint_t p1 = {cx1, cy1};
  lv_fpoint_t p2 = {cx2, cy2};
  lv_fpoint_t p3 = {x, y};
  if (path_) lv_vector_path_cubic_to(path_, &p1, &p2, &p3);
}

void VectorPath::arc_to(float rx, float ry, float angle, bool large_arc,
                        bool sweep, float x, float y) {
  lv_fpoint_t p = {x, y};
  if (path_) lv_vector_path_arc_to(path_, rx, ry, angle, large_arc, sweep, &p);
}

void VectorPath::close() {
  if (path_) lv_vector_path_close(path_);
}

void VectorPath::append_rect(float x, float y, float w, float h, float rx,
                             float ry) {
  if (path_) lv_vector_path_append_rectangle(path_, x, y, w, h, rx, ry);
}

void VectorPath::append_circle(float cx, float cy, float rx, float ry) {
  lv_fpoint_t c = {cx, cy};
  if (path_) lv_vector_path_append_circle(path_, &c, rx, ry);
}

void VectorPath::append_arc(float cx, float cy, float radius, float start_angle,
                            float sweep, bool pie) {
  lv_fpoint_t c = {cx, cy};
  if (path_)
    lv_vector_path_append_arc(path_, &c, radius, start_angle, sweep, pie);
}

void VectorPath::append_path(const VectorPath& other) {
  if (path_ && other.path_) lv_vector_path_append_path(path_, other.path_);
}

void VectorPath::transform(const lv_matrix_t& matrix) {
  if (path_) lv_matrix_transform_path(&matrix, path_);
}

void VectorPath::get_bounding_box(lv_area_t& area) const {
  if (path_) lv_vector_path_get_bounding(path_, &area);
}

// --- VectorDraw ---

VectorDraw::VectorDraw(lv_layer_t* layer) {
  dsc_ = lv_draw_vector_dsc_create(layer);
}

VectorDraw::~VectorDraw() {
  if (dsc_) lv_draw_vector_dsc_delete(dsc_);
}

VectorDraw::VectorDraw(VectorDraw&& other) noexcept : dsc_(other.dsc_) {
  other.dsc_ = nullptr;
}

VectorDraw& VectorDraw::operator=(VectorDraw&& other) noexcept {
  if (this != &other) {
    if (dsc_) lv_draw_vector_dsc_delete(dsc_);
    dsc_ = other.dsc_;
    other.dsc_ = nullptr;
  }
  return *this;
}

void VectorDraw::set_transform(const lv_matrix_t& matrix) {
  if (dsc_) lv_draw_vector_dsc_set_transform(dsc_, &matrix);
}

void VectorDraw::set_blend_mode(lv_vector_blend_t blend) {
  if (dsc_) lv_draw_vector_dsc_set_blend_mode(dsc_, blend);
}

void VectorDraw::set_fill_color(lv_color32_t color) {
  if (dsc_) lv_draw_vector_dsc_set_fill_color32(dsc_, color);
}

void VectorDraw::set_fill_opa(lv_opa_t opa) {
  if (dsc_) lv_draw_vector_dsc_set_fill_opa(dsc_, opa);
}

void VectorDraw::set_fill_rule(lv_vector_fill_t rule) {
  if (dsc_) lv_draw_vector_dsc_set_fill_rule(dsc_, rule);
}

void VectorDraw::set_fill_image(const lv_draw_image_dsc_t& img_dsc) {
  if (dsc_) lv_draw_vector_dsc_set_fill_image(dsc_, &img_dsc);
}

void VectorDraw::set_fill_linear_gradient(float x1, float y1, float x2,
                                          float y2) {
  if (dsc_) lv_draw_vector_dsc_set_fill_linear_gradient(dsc_, x1, y1, x2, y2);
}

void VectorDraw::set_fill_radial_gradient(float cx, float cy, float radius) {
  if (dsc_) lv_draw_vector_dsc_set_fill_radial_gradient(dsc_, cx, cy, radius);
}

void VectorDraw::set_fill_gradient_stops(
    const std::vector<lv_grad_stop_t>& stops) {
  if (dsc_ && !stops.empty()) {
    lv_draw_vector_dsc_set_fill_gradient_color_stops(dsc_, stops.data(),
                                                     stops.size());
  }
}

void VectorDraw::set_fill_gradient_spread(lv_vector_gradient_spread_t spread) {
  if (dsc_) lv_draw_vector_dsc_set_fill_gradient_spread(dsc_, spread);
}

void VectorDraw::set_fill_transform(const lv_matrix_t& matrix) {
  if (dsc_) lv_draw_vector_dsc_set_fill_transform(dsc_, &matrix);
}

void VectorDraw::set_stroke_color(lv_color32_t color) {
  if (dsc_) lv_draw_vector_dsc_set_stroke_color32(dsc_, color);
}

void VectorDraw::set_stroke_opa(lv_opa_t opa) {
  if (dsc_) lv_draw_vector_dsc_set_stroke_opa(dsc_, opa);
}

void VectorDraw::set_stroke_width(float width) {
  if (dsc_) lv_draw_vector_dsc_set_stroke_width(dsc_, width);
}

void VectorDraw::set_stroke_dash(const std::vector<float>& dash_pattern) {
  // lvgl expects non-const float*, so we might need a copy or const_cast if
  // impl does not modify. Viewing source: it usually copies or references. The
  // prototype is `float *`, implying potential modification or just legacy C.
  // Assuming it copies or reads, but we need to pass a mutable pointer.
  // If we pass a vector, we can pass data().
  if (dsc_ && !dash_pattern.empty()) {
    // Cast away constness, assuming API doesn't actually mutate pattern for
    // read-only dash setting. Ideally check implementation.
    lv_draw_vector_dsc_set_stroke_dash(
        dsc_, const_cast<float*>(dash_pattern.data()), dash_pattern.size());
  }
}

void VectorDraw::set_stroke_cap(lv_vector_stroke_cap_t cap) {
  if (dsc_) lv_draw_vector_dsc_set_stroke_cap(dsc_, cap);
}

void VectorDraw::set_stroke_join(lv_vector_stroke_join_t join) {
  if (dsc_) lv_draw_vector_dsc_set_stroke_join(dsc_, join);
}

void VectorDraw::set_stroke_miter_limit(uint16_t limit) {
  if (dsc_) lv_draw_vector_dsc_set_stroke_miter_limit(dsc_, limit);
}

void VectorDraw::set_stroke_linear_gradient(float x1, float y1, float x2,
                                            float y2) {
  if (dsc_) lv_draw_vector_dsc_set_stroke_linear_gradient(dsc_, x1, y1, x2, y2);
}

void VectorDraw::set_stroke_radial_gradient(float cx, float cy, float radius) {
  if (dsc_) lv_draw_vector_dsc_set_stroke_radial_gradient(dsc_, cx, cy, radius);
}

void VectorDraw::set_stroke_gradient_stops(
    const std::vector<lv_grad_stop_t>& stops) {
  if (dsc_ && !stops.empty()) {
    lv_draw_vector_dsc_set_stroke_gradient_color_stops(dsc_, stops.data(),
                                                       stops.size());
  }
}

void VectorDraw::set_stroke_gradient_spread(
    lv_vector_gradient_spread_t spread) {
  if (dsc_) lv_draw_vector_dsc_set_stroke_gradient_spread(dsc_, spread);
}

void VectorDraw::set_stroke_transform(const lv_matrix_t& matrix) {
  if (dsc_) lv_draw_vector_dsc_set_stroke_transform(dsc_, &matrix);
}

void VectorDraw::identity() {
  if (dsc_) lv_draw_vector_dsc_identity(dsc_);
}

void VectorDraw::scale(float x, float y) {
  if (dsc_) lv_draw_vector_dsc_scale(dsc_, x, y);
}

void VectorDraw::rotate(float degree) {
  if (dsc_) lv_draw_vector_dsc_rotate(dsc_, degree);
}

void VectorDraw::translate(float x, float y) {
  if (dsc_) lv_draw_vector_dsc_translate(dsc_, x, y);
}

void VectorDraw::skew(float x, float y) {
  if (dsc_) lv_draw_vector_dsc_skew(dsc_, x, y);
}

void VectorDraw::add_path(const VectorPath& path) {
  if (dsc_ && path.raw()) {
    lv_draw_vector_dsc_add_path(dsc_, path.raw());
  }
}

void VectorDraw::clear_area(const lv_area_t& rect) {
  if (dsc_) lv_draw_vector_dsc_clear_area(dsc_, &rect);
}

void VectorDraw::draw() {
  if (dsc_) lv_draw_vector(dsc_);
}

}  // namespace lvgl

#endif  // LV_USE_VECTOR_GRAPHIC
