#include "textarea.h"

#include "label.h"

#if LV_USE_TEXTAREA

namespace lvgl {

Textarea::Textarea()
    : Textarea(static_cast<Object*>(nullptr), Ownership::Managed) {}

Textarea::Textarea(Object* parent, Ownership ownership)
    : Widget(lv_textarea_create(parent ? parent->raw() : nullptr), ownership) {}

Textarea::Textarea(Object& parent) : Textarea(&parent) {}

Textarea::Textarea(Object& parent, const char* text) : Textarea(&parent) {
  set_text(text);
}

Textarea::Textarea(Object& parent, const std::string& text)
    : Textarea(&parent) {
  set_text(text.c_str());
}

Textarea::Textarea(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

Textarea& Textarea::add_char(uint32_t c) {
  if (raw()) lv_textarea_add_char(raw(), c);
  return *this;
}

Textarea& Textarea::add_text(const char* txt) {
  if (raw()) lv_textarea_add_text(raw(), txt);
  return *this;
}

Textarea& Textarea::delete_char() {
  if (raw()) lv_textarea_delete_char(raw());
  return *this;
}

Textarea& Textarea::delete_char_forward() {
  if (raw()) lv_textarea_delete_char_forward(raw());
  return *this;
}

Textarea& Textarea::set_text(const char* txt) {
  if (raw()) lv_textarea_set_text(raw(), txt);
  return *this;
}

Textarea& Textarea::set_placeholder_text(const char* txt) {
  if (raw()) lv_textarea_set_placeholder_text(raw(), txt);
  return *this;
}

Textarea& Textarea::set_cursor_pos(int32_t pos) {
  if (raw()) lv_textarea_set_cursor_pos(raw(), pos);
  return *this;
}

Textarea& Textarea::set_cursor_click_pos(bool en) {
  if (raw()) lv_textarea_set_cursor_click_pos(raw(), en);
  return *this;
}

Textarea& Textarea::set_password_mode(bool en) {
  if (raw()) lv_textarea_set_password_mode(raw(), en);
  return *this;
}

Textarea& Textarea::set_password_bullet(const char* bullet) {
  if (raw()) lv_textarea_set_password_bullet(raw(), bullet);
  return *this;
}

Textarea& Textarea::set_one_line(bool en) {
  if (raw()) lv_textarea_set_one_line(raw(), en);
  return *this;
}

Textarea& Textarea::set_accepted_chars(const char* list) {
  if (raw()) lv_textarea_set_accepted_chars(raw(), list);
  return *this;
}

Textarea& Textarea::set_max_length(uint32_t num) {
  if (raw()) lv_textarea_set_max_length(raw(), num);
  return *this;
}

Textarea& Textarea::set_insert_replace(const char* txt) {
  if (raw()) lv_textarea_set_insert_replace(raw(), txt);
  return *this;
}

Textarea& Textarea::set_text_selection(bool en) {
  if (raw()) lv_textarea_set_text_selection(raw(), en);
  return *this;
}

Textarea& Textarea::set_password_show_time(uint32_t time) {
  if (raw()) lv_textarea_set_password_show_time(raw(), time);
  return *this;
}

Textarea& Textarea::set_align(TextAlign align) {
  if (raw()) lv_textarea_set_align(raw(), static_cast<lv_text_align_t>(align));
  return *this;
}

const char* Textarea::get_text() const {
  return raw() ? lv_textarea_get_text(raw()) : nullptr;
}

const char* Textarea::get_placeholder_text() {
  return raw() ? lv_textarea_get_placeholder_text(raw()) : nullptr;
}

Label Textarea::get_label() {
  return Label(raw() ? lv_textarea_get_label(raw()) : nullptr);
}

uint32_t Textarea::get_cursor_pos() {
  return raw() ? lv_textarea_get_cursor_pos(raw()) : 0;
}

bool Textarea::get_cursor_click_pos() {
  return raw() ? lv_textarea_get_cursor_click_pos(raw()) : false;
}

bool Textarea::get_password_mode() const {
  return raw() ? lv_textarea_get_password_mode(raw()) : false;
}

const char* Textarea::get_password_bullet() {
  return raw() ? lv_textarea_get_password_bullet(raw()) : nullptr;
}

bool Textarea::get_one_line() const {
  return raw() ? lv_textarea_get_one_line(raw()) : false;
}

const char* Textarea::get_accepted_chars() {
  return raw() ? lv_textarea_get_accepted_chars(raw()) : nullptr;
}

uint32_t Textarea::get_max_length() {
  return raw() ? lv_textarea_get_max_length(raw()) : 0;
}

bool Textarea::text_is_selected() {
  return raw() ? lv_textarea_text_is_selected(raw()) : false;
}

bool Textarea::get_text_selection() {
  return raw() ? lv_textarea_get_text_selection(raw()) : false;
}

uint32_t Textarea::get_password_show_time() {
  return raw() ? lv_textarea_get_password_show_time(raw()) : 0;
}

uint32_t Textarea::get_current_char() {
  return raw() ? lv_textarea_get_current_char(raw()) : 0;
}

void Textarea::clear_selection() {
  if (raw()) lv_textarea_clear_selection(raw());
}

void Textarea::cursor_right() {
  if (raw()) lv_textarea_cursor_right(raw());
}

void Textarea::cursor_left() {
  if (raw()) lv_textarea_cursor_left(raw());
}

void Textarea::cursor_down() {
  if (raw()) lv_textarea_cursor_down(raw());
}

void Textarea::cursor_up() {
  if (raw()) lv_textarea_cursor_up(raw());
}

Textarea& Textarea::on_value_changed(std::function<void(lvgl::Event&)> cb) {
  add_event_cb(EventCode::ValueChanged, std::move(cb));
  return *this;
}

}  // namespace lvgl

#endif  // LV_USE_TEXTAREA
