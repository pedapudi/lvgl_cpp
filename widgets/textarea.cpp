#include "textarea.h"

#include "label.h"

#if LV_USE_TEXTAREA

namespace lvgl {

Textarea::Textarea()
    : Object(lv_textarea_create(nullptr), Ownership::Managed) {}

Textarea::Textarea(Object& parent, Ownership ownership)
    : Object(lv_textarea_create(parent.raw()), ownership) {}

Textarea::Textarea(Object& parent, const char* text) : Textarea(parent) {
  set_text(text);
}

Textarea::Textarea(Object& parent, const std::string& text) : Textarea(parent) {
  set_text(text.c_str());
}

Textarea::Textarea(lv_obj_t* obj, Ownership ownership)
    : Object(obj, ownership) {}

Textarea& Textarea::add_char(uint32_t c) {
  if (obj_) lv_textarea_add_char(obj_, c);
  return *this;
}

Textarea& Textarea::add_text(const char* txt) {
  if (obj_) lv_textarea_add_text(obj_, txt);
  return *this;
}

Textarea& Textarea::delete_char() {
  if (obj_) lv_textarea_delete_char(obj_);
  return *this;
}

Textarea& Textarea::delete_char_forward() {
  if (obj_) lv_textarea_delete_char_forward(obj_);
  return *this;
}

Textarea& Textarea::set_text(const char* txt) {
  if (obj_) lv_textarea_set_text(obj_, txt);
  return *this;
}

Textarea& Textarea::set_placeholder_text(const char* txt) {
  if (obj_) lv_textarea_set_placeholder_text(obj_, txt);
  return *this;
}

Textarea& Textarea::set_cursor_pos(int32_t pos) {
  if (obj_) lv_textarea_set_cursor_pos(obj_, pos);
  return *this;
}

Textarea& Textarea::set_cursor_click_pos(bool en) {
  if (obj_) lv_textarea_set_cursor_click_pos(obj_, en);
  return *this;
}

Textarea& Textarea::set_password_mode(bool en) {
  if (obj_) lv_textarea_set_password_mode(obj_, en);
  return *this;
}

Textarea& Textarea::set_password_bullet(const char* bullet) {
  if (obj_) lv_textarea_set_password_bullet(obj_, bullet);
  return *this;
}

Textarea& Textarea::set_one_line(bool en) {
  if (obj_) lv_textarea_set_one_line(obj_, en);
  return *this;
}

Textarea& Textarea::set_accepted_chars(const char* list) {
  if (obj_) lv_textarea_set_accepted_chars(obj_, list);
  return *this;
}

Textarea& Textarea::set_max_length(uint32_t num) {
  if (obj_) lv_textarea_set_max_length(obj_, num);
  return *this;
}

Textarea& Textarea::set_insert_replace(const char* txt) {
  if (obj_) lv_textarea_set_insert_replace(obj_, txt);
  return *this;
}

Textarea& Textarea::set_text_selection(bool en) {
  if (obj_) lv_textarea_set_text_selection(obj_, en);
  return *this;
}

Textarea& Textarea::set_password_show_time(uint32_t time) {
  if (obj_) lv_textarea_set_password_show_time(obj_, time);
  return *this;
}

Textarea& Textarea::set_align(lv_text_align_t align) {
  if (obj_) lv_textarea_set_align(obj_, align);
  return *this;
}

Textarea& Textarea::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Textarea& Textarea::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Textarea& Textarea::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Textarea& Textarea::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Textarea& Textarea::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Textarea& Textarea::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Textarea& Textarea::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Textarea& Textarea::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

const char* Textarea::get_text() const {
  return obj_ ? lv_textarea_get_text(obj_) : nullptr;
}

const char* Textarea::get_placeholder_text() {
  return obj_ ? lv_textarea_get_placeholder_text(obj_) : nullptr;
}

Label Textarea::get_label() {
  return Label(obj_ ? lv_textarea_get_label(obj_) : nullptr);
}

uint32_t Textarea::get_cursor_pos() {
  return obj_ ? lv_textarea_get_cursor_pos(obj_) : 0;
}

bool Textarea::get_cursor_click_pos() {
  return obj_ ? lv_textarea_get_cursor_click_pos(obj_) : false;
}

bool Textarea::get_password_mode() const {
  return obj_ ? lv_textarea_get_password_mode(obj_) : false;
}

const char* Textarea::get_password_bullet() {
  return obj_ ? lv_textarea_get_password_bullet(obj_) : nullptr;
}

bool Textarea::get_one_line() const {
  return obj_ ? lv_textarea_get_one_line(obj_) : false;
}

const char* Textarea::get_accepted_chars() {
  return obj_ ? lv_textarea_get_accepted_chars(obj_) : nullptr;
}

uint32_t Textarea::get_max_length() {
  return obj_ ? lv_textarea_get_max_length(obj_) : 0;
}

bool Textarea::text_is_selected() {
  return obj_ ? lv_textarea_text_is_selected(obj_) : false;
}

bool Textarea::get_text_selection() {
  return obj_ ? lv_textarea_get_text_selection(obj_) : false;
}

uint32_t Textarea::get_password_show_time() {
  return obj_ ? lv_textarea_get_password_show_time(obj_) : 0;
}

uint32_t Textarea::get_current_char() {
  return obj_ ? lv_textarea_get_current_char(obj_) : 0;
}

void Textarea::clear_selection() {
  if (obj_) lv_textarea_clear_selection(obj_);
}

void Textarea::cursor_right() {
  if (obj_) lv_textarea_cursor_right(obj_);
}

void Textarea::cursor_left() {
  if (obj_) lv_textarea_cursor_left(obj_);
}

void Textarea::cursor_down() {
  if (obj_) lv_textarea_cursor_down(obj_);
}

void Textarea::cursor_up() {
  if (obj_) lv_textarea_cursor_up(obj_);
}

}  // namespace lvgl

#endif  // LV_USE_TEXTAREA
