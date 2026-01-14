#include "dropdown.h"

#include "../core/observer.h"

#if LV_USE_DROPDOWN

namespace lvgl {

Dropdown::Dropdown() : Dropdown((Object*)nullptr) {}

Dropdown::Dropdown(Object* parent)
    : Object(lv_dropdown_create(parent ? parent->raw() : nullptr)) {}

Dropdown::Dropdown(lv_obj_t* obj) : Object(obj) {}

void Dropdown::set_text(const char* txt) {
  if (obj_) lv_dropdown_set_text(obj_, txt);
}

void Dropdown::set_options(const char* options) {
  if (obj_) lv_dropdown_set_options(obj_, options);
}

void Dropdown::set_options_static(const char* options) {
  if (obj_) lv_dropdown_set_options_static(obj_, options);
}

void Dropdown::add_option(const char* option, uint32_t pos) {
  if (obj_) lv_dropdown_add_option(obj_, option, pos);
}

void Dropdown::clear_options() {
  if (obj_) lv_dropdown_clear_options(obj_);
}

void Dropdown::set_selected(uint32_t sel_opt) {
  if (obj_) lv_dropdown_set_selected(obj_, sel_opt);
}

void Dropdown::set_dir(lv_dir_t dir) {
  if (obj_) lv_dropdown_set_dir(obj_, dir);
}

void Dropdown::set_symbol(const void* symbol) {
  if (obj_) lv_dropdown_set_symbol(obj_, symbol);
}

void Dropdown::set_selected_highlight(bool en) {
  if (obj_) lv_dropdown_set_selected_highlight(obj_, en);
}

lv_obj_t* Dropdown::get_list() {
  return obj_ ? lv_dropdown_get_list(obj_) : nullptr;
}

const char* Dropdown::get_text() {
  return obj_ ? lv_dropdown_get_text(obj_) : nullptr;
}

const char* Dropdown::get_options() {
  return obj_ ? lv_dropdown_get_options(obj_) : nullptr;
}

uint32_t Dropdown::get_selected() {
  return obj_ ? lv_dropdown_get_selected(obj_) : 0;
}

uint32_t Dropdown::get_option_count() {
  return obj_ ? lv_dropdown_get_option_count(obj_) : 0;
}

void Dropdown::get_selected_str(char* buf, uint32_t buf_size) {
  if (obj_) lv_dropdown_get_selected_str(obj_, buf, buf_size);
}

int32_t Dropdown::get_option_index(const char* option) {
  return obj_ ? lv_dropdown_get_option_index(obj_, option) : -1;
}

const char* Dropdown::get_symbol() {
  return obj_ ? lv_dropdown_get_symbol(obj_) : nullptr;
}

bool Dropdown::get_selected_highlight() {
  return obj_ ? lv_dropdown_get_selected_highlight(obj_) : false;
}

lv_dir_t Dropdown::get_dir() {
  return obj_ ? lv_dropdown_get_dir(obj_) : LV_DIR_BOTTOM;
}

void Dropdown::open() {
  if (obj_) lv_dropdown_open(obj_);
}

void Dropdown::close() {
  if (obj_) lv_dropdown_close(obj_);
}

bool Dropdown::is_open() { return obj_ ? lv_dropdown_is_open(obj_) : false; }

Observer Dropdown::bind_value(Subject& subject) {
  return Observer(lv_dropdown_bind_value(raw(), subject.raw()));
}

}  // namespace lvgl

#endif // LV_USE_DROPDOWN
