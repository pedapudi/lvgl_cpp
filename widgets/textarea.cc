#include "textarea.h"
#include "label.h"

namespace lvgl {

Textarea::Textarea() : Textarea((Object *)nullptr) {}

Textarea::Textarea(Object *parent)
    : Object(lv_textarea_create(parent ? parent->raw() : nullptr)) {}

Textarea::Textarea(lv_obj_t *obj) : Object(obj) {}

void Textarea::add_char(uint32_t c) {
  if (obj_)
    lv_textarea_add_char(obj_, c);
}

void Textarea::add_text(const char *txt) {
  if (obj_)
    lv_textarea_add_text(obj_, txt);
}

void Textarea::delete_char() {
  if (obj_)
    lv_textarea_delete_char(obj_);
}

void Textarea::delete_char_forward() {
  if (obj_)
    lv_textarea_delete_char_forward(obj_);
}

void Textarea::set_text(const char *txt) {
  if (obj_)
    lv_textarea_set_text(obj_, txt);
}

void Textarea::set_placeholder_text(const char *txt) {
  if (obj_)
    lv_textarea_set_placeholder_text(obj_, txt);
}

void Textarea::set_cursor_pos(int32_t pos) {
  if (obj_)
    lv_textarea_set_cursor_pos(obj_, pos);
}

void Textarea::set_cursor_click_pos(bool en) {
  if (obj_)
    lv_textarea_set_cursor_click_pos(obj_, en);
}

void Textarea::set_password_mode(bool en) {
  if (obj_)
    lv_textarea_set_password_mode(obj_, en);
}

void Textarea::set_password_bullet(const char *bullet) {
  if (obj_)
    lv_textarea_set_password_bullet(obj_, bullet);
}

void Textarea::set_one_line(bool en) {
  if (obj_)
    lv_textarea_set_one_line(obj_, en);
}

void Textarea::set_accepted_chars(const char *list) {
  if (obj_)
    lv_textarea_set_accepted_chars(obj_, list);
}

void Textarea::set_max_length(uint32_t num) {
  if (obj_)
    lv_textarea_set_max_length(obj_, num);
}

void Textarea::set_insert_replace(const char *txt) {
  if (obj_)
    lv_textarea_set_insert_replace(obj_, txt);
}

void Textarea::set_text_selection(bool en) {
  if (obj_)
    lv_textarea_set_text_selection(obj_, en);
}

void Textarea::set_password_show_time(uint32_t time) {
  if (obj_)
    lv_textarea_set_password_show_time(obj_, time);
}

void Textarea::set_align(lv_text_align_t align) {
  if (obj_)
    lv_textarea_set_align(obj_, align);
}

const char *Textarea::get_text() const {
  return obj_ ? lv_textarea_get_text(obj_) : nullptr;
}

const char *Textarea::get_placeholder_text() {
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

const char *Textarea::get_password_bullet() {
  return obj_ ? lv_textarea_get_password_bullet(obj_) : nullptr;
}

bool Textarea::get_one_line() const {
  return obj_ ? lv_textarea_get_one_line(obj_) : false;
}

const char *Textarea::get_accepted_chars() {
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
  if (obj_)
    lv_textarea_clear_selection(obj_);
}

void Textarea::cursor_right() {
  if (obj_)
    lv_textarea_cursor_right(obj_);
}

void Textarea::cursor_left() {
  if (obj_)
    lv_textarea_cursor_left(obj_);
}

void Textarea::cursor_down() {
  if (obj_)
    lv_textarea_cursor_down(obj_);
}

void Textarea::cursor_up() {
  if (obj_)
    lv_textarea_cursor_up(obj_);
}

} // namespace lvgl
