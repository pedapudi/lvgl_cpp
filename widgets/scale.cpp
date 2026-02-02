#include "scale.h"

#if LV_USE_SCALE
#include "../misc/style.h"

namespace lvgl {

Scale::Scale() : Scale(static_cast<Object*>(nullptr), Ownership::Managed) {}

Scale::Scale(Object* parent, Ownership ownership)
    : Widget(lv_scale_create(parent ? parent->raw() : nullptr), ownership) {}

Scale::Scale(Object& parent) : Scale(&parent) {}

Scale::Scale(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Scale& Scale::set_mode(Mode mode) {
  if (raw()) lv_scale_set_mode(raw(), static_cast<lv_scale_mode_t>(mode));
  return *this;
}

Scale& Scale::set_total_tick_count(uint32_t total_tick_count) {
  if (raw()) lv_scale_set_total_tick_count(raw(), total_tick_count);
  return *this;
}

Scale& Scale::set_major_tick_every(uint32_t major_tick_every) {
  if (raw()) lv_scale_set_major_tick_every(raw(), major_tick_every);
  return *this;
}

Scale& Scale::set_label_show(bool show_label) {
  if (raw()) lv_scale_set_label_show(raw(), show_label);
  return *this;
}

Scale& Scale::set_range(int32_t min, int32_t max) {
  if (raw()) lv_scale_set_range(raw(), min, max);
  return *this;
}

Scale& Scale::set_angle_range(uint32_t angle_range) {
  if (raw()) lv_scale_set_angle_range(raw(), angle_range);
  return *this;
}

Scale& Scale::set_rotation(int32_t rotation) {
  if (raw()) lv_scale_set_rotation(raw(), rotation);
  return *this;
}

Scale& Scale::set_line_needle_value(lv_obj_t* needle_line,
                                    int32_t needle_length, int32_t value) {
  if (raw())
    lv_scale_set_line_needle_value(raw(), needle_line, needle_length, value);
  return *this;
}

Scale& Scale::set_image_needle_value(lv_obj_t* needle_img, int32_t value) {
  if (raw()) lv_scale_set_image_needle_value(raw(), needle_img, value);
  return *this;
}

Scale& Scale::set_text_src(const char* txt_src[]) {
  if (raw()) lv_scale_set_text_src(raw(), txt_src);
  return *this;
}

Scale& Scale::set_post_draw(bool en) {
  if (raw()) lv_scale_set_post_draw(raw(), en);
  return *this;
}

Scale& Scale::set_draw_ticks_on_top(bool en) {
  if (raw()) lv_scale_set_draw_ticks_on_top(raw(), en);
  return *this;
}

Scale::Mode Scale::get_mode() {
  return raw() ? static_cast<Mode>(lv_scale_get_mode(raw()))
               : Mode::HorizontalBottom;
}

// --- ScaleSection ---

void ScaleSection::set_range(int32_t min, int32_t max) {
  if (section_) {
    lv_scale_section_set_range(section_, min, max);
  }
}

void ScaleSection::set_style(Part part, Style& style) {
  if (section_) {
    lv_scale_section_set_style(section_, static_cast<lv_part_t>(part),
                               style.raw());
  }
}

void ScaleSection::set_style(lv_part_t part, lv_style_t* style) {
  if (section_) {
    lv_scale_section_set_style(section_, part, style);
  }
}

ScaleSection Scale::add_section() {
  if (raw()) {
    return ScaleSection(this, lv_scale_add_section(raw()));
  }
  return ScaleSection(this, nullptr);
}

}  // namespace lvgl

#endif  // LV_USE_SCALE
