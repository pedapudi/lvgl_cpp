#include "dropdown.h"

#include "../core/observer.h"

#if LV_USE_DROPDOWN

namespace lvgl {

Dropdown::Dropdown()
    : Dropdown(static_cast<Object*>(nullptr), Ownership::Managed) {}

Dropdown::Dropdown(Object* parent, Ownership ownership)
    : Widget(lv_dropdown_create(parent ? parent->raw() : nullptr), ownership) {}

Dropdown::Dropdown(Object& parent) : Dropdown(&parent) {}

Dropdown::Dropdown(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

Dropdown& Dropdown::set_text(const char* txt) {
  if (raw()) lv_dropdown_set_text(raw(), txt);
  return *this;
}

Dropdown& Dropdown::set_options(const char* options) {
  if (raw()) lv_dropdown_set_options(raw(), options);
  return *this;
}

Dropdown& Dropdown::set_options_static(const char* options) {
  if (raw()) lv_dropdown_set_options_static(raw(), options);
  return *this;
}

Dropdown& Dropdown::add_option(const char* option, uint32_t pos) {
  if (raw()) lv_dropdown_add_option(raw(), option, pos);
  return *this;
}

Dropdown& Dropdown::clear_options() {
  if (raw()) lv_dropdown_clear_options(raw());
  return *this;
}

Dropdown& Dropdown::set_selected(uint32_t sel_opt) {
  if (raw()) lv_dropdown_set_selected(raw(), sel_opt);
  return *this;
}

Dropdown& Dropdown::set_dir(Dir dir) {
  if (raw()) lv_dropdown_set_dir(raw(), static_cast<lv_dir_t>(dir));
  return *this;
}

Dropdown& Dropdown::set_symbol(const void* symbol) {
  if (raw()) lv_dropdown_set_symbol(raw(), symbol);
  return *this;
}

Dropdown& Dropdown::set_selected_highlight(bool en) {
  if (raw()) lv_dropdown_set_selected_highlight(raw(), en);
  return *this;
}

Dropdown& Dropdown::on_value_changed(std::function<void(lvgl::Event&)> cb) {
  return add_event_cb(LV_EVENT_VALUE_CHANGED, cb);
}

lv_obj_t* Dropdown::get_list() {
  return raw() ? lv_dropdown_get_list(raw()) : nullptr;
}

const char* Dropdown::get_text() {
  return raw() ? lv_dropdown_get_text(raw()) : nullptr;
}

const char* Dropdown::get_options() {
  return raw() ? lv_dropdown_get_options(raw()) : nullptr;
}

uint32_t Dropdown::get_selected() {
  return raw() ? lv_dropdown_get_selected(raw()) : 0;
}

uint32_t Dropdown::get_option_count() {
  return raw() ? lv_dropdown_get_option_count(raw()) : 0;
}

void Dropdown::get_selected_str(char* buf, uint32_t buf_size) {
  if (raw()) lv_dropdown_get_selected_str(raw(), buf, buf_size);
}

int32_t Dropdown::get_option_index(const char* option) {
  return raw() ? lv_dropdown_get_option_index(raw(), option) : -1;
}

const char* Dropdown::get_symbol() {
  return raw() ? lv_dropdown_get_symbol(raw()) : nullptr;
}

bool Dropdown::get_selected_highlight() {
  return raw() ? lv_dropdown_get_selected_highlight(raw()) : false;
}

Dir Dropdown::get_dir() {
  return raw() ? static_cast<Dir>(lv_dropdown_get_dir(raw())) : Dir::Bottom;
}

void Dropdown::open() {
  if (raw()) lv_dropdown_open(raw());
}

void Dropdown::close() {
  if (raw()) lv_dropdown_close(raw());
}

bool Dropdown::is_open() { return raw() ? lv_dropdown_is_open(raw()) : false; }

Observer Dropdown::bind_value(Subject& subject) {
  return Observer(lv_dropdown_bind_value(raw(), subject.raw()), false);
}

}  // namespace lvgl

#endif  // LV_USE_DROPDOWN
