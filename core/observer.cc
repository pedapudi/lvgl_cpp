#include "observer.h"

namespace lvgl {

// Subject

Subject::Subject() {
  // raw struct init done by subclasses
}

Subject::~Subject() { lv_subject_deinit(&subject_); }

void Subject::notify() { lv_subject_notify(&subject_); }

void Subject::bind_flag_if_eq(Object& obj, lv_obj_flag_t flag,
                              int32_t ref_value) {
  lv_obj_bind_flag_if_eq(obj.raw(), &subject_, flag, ref_value);
}

void Subject::bind_flag_if_not_eq(Object& obj, lv_obj_flag_t flag,
                                  int32_t ref_value) {
  lv_obj_bind_flag_if_not_eq(obj.raw(), &subject_, flag, ref_value);
}

void Subject::bind_flag_if_gt(Object& obj, lv_obj_flag_t flag,
                              int32_t ref_value) {
  lv_obj_bind_flag_if_gt(obj.raw(), &subject_, flag, ref_value);
}

void Subject::bind_flag_if_ge(Object& obj, lv_obj_flag_t flag,
                              int32_t ref_value) {
  lv_obj_bind_flag_if_ge(obj.raw(), &subject_, flag, ref_value);
}

void Subject::bind_flag_if_lt(Object& obj, lv_obj_flag_t flag,
                              int32_t ref_value) {
  lv_obj_bind_flag_if_lt(obj.raw(), &subject_, flag, ref_value);
}

void Subject::bind_flag_if_le(Object& obj, lv_obj_flag_t flag,
                              int32_t ref_value) {
  lv_obj_bind_flag_if_le(obj.raw(), &subject_, flag, ref_value);
}

void Subject::bind_state_if_eq(Object& obj, lv_state_t state,
                               int32_t ref_value) {
  lv_obj_bind_state_if_eq(obj.raw(), &subject_, state, ref_value);
}

void Subject::bind_state_if_not_eq(Object& obj, lv_state_t state,
                                   int32_t ref_value) {
  lv_obj_bind_state_if_not_eq(obj.raw(), &subject_, state, ref_value);
}

void Subject::bind_state_if_gt(Object& obj, lv_state_t state,
                               int32_t ref_value) {
  lv_obj_bind_state_if_gt(obj.raw(), &subject_, state, ref_value);
}

void Subject::bind_state_if_ge(Object& obj, lv_state_t state,
                               int32_t ref_value) {
  lv_obj_bind_state_if_ge(obj.raw(), &subject_, state, ref_value);
}

void Subject::bind_state_if_lt(Object& obj, lv_state_t state,
                               int32_t ref_value) {
  lv_obj_bind_state_if_lt(obj.raw(), &subject_, state, ref_value);
}

void Subject::bind_state_if_le(Object& obj, lv_state_t state,
                               int32_t ref_value) {
  lv_obj_bind_state_if_le(obj.raw(), &subject_, state, ref_value);
}

void Subject::bind_checked(Object& obj) {
  lv_obj_bind_checked(obj.raw(), &subject_);
}

Observer* Subject::add_observer(lv_observer_cb_t cb, void* user_data) {
  // We use add_observer_with_target to store user_data in the target field
  // if it's not a widget.
  lv_observer_t* obs =
      lv_subject_add_observer_with_target(&subject_, cb, user_data, nullptr);
  return new Observer(obs, true);
}

// IntSubject

IntSubject::IntSubject(int32_t value) { lv_subject_init_int(&subject_, value); }

void IntSubject::set(int32_t value) { lv_subject_set_int(&subject_, value); }

int32_t IntSubject::get() { return lv_subject_get_int(&subject_); }

int32_t IntSubject::get_previous() {
  return lv_subject_get_previous_int(&subject_);
}

void IntSubject::set_range(int32_t min, int32_t max) {
  lv_subject_set_min_value_int(&subject_, min);
  lv_subject_set_max_value_int(&subject_, max);
}

// FloatSubject

FloatSubject::FloatSubject(float value) {
  lv_subject_init_float(&subject_, value);
}

void FloatSubject::set(float value) { lv_subject_set_float(&subject_, value); }

float FloatSubject::get() { return lv_subject_get_float(&subject_); }

float FloatSubject::get_previous() {
  return lv_subject_get_previous_float(&subject_);
}

void FloatSubject::set_range(float min, float max) {
  lv_subject_set_min_value_float(&subject_, min);
  lv_subject_set_max_value_float(&subject_, max);
}

// StringSubject

StringSubject::StringSubject(const std::string& value, size_t buf_size) {
  buf_.resize(buf_size);
  prev_buf_.resize(buf_size);
  lv_subject_init_string(&subject_, buf_.data(), prev_buf_.data(), buf_size,
                         value.c_str());
}

StringSubject::~StringSubject() {
  // Subject destructor calls deinit, buffers are freed by vector dtor.
}

void StringSubject::set(const std::string& value) {
  lv_subject_copy_string(&subject_, value.c_str());
}

const char* StringSubject::get() { return lv_subject_get_string(&subject_); }

const char* StringSubject::get_previous() {
  return lv_subject_get_previous_string(&subject_);
}

// PointerSubject

PointerSubject::PointerSubject(void* ptr) {
  lv_subject_init_pointer(&subject_, ptr);
}

void PointerSubject::set(void* ptr) { lv_subject_set_pointer(&subject_, ptr); }

const void* PointerSubject::get() { return lv_subject_get_pointer(&subject_); }

const void* PointerSubject::get_previous() {
  return lv_subject_get_previous_pointer(&subject_);
}

// ColorSubject

ColorSubject::ColorSubject(lv_color_t color) {
  lv_subject_init_color(&subject_, color);
}

void ColorSubject::set(lv_color_t color) {
  lv_subject_set_color(&subject_, color);
}

lv_color_t ColorSubject::get() { return lv_subject_get_color(&subject_); }

lv_color_t ColorSubject::get_previous() {
  return lv_subject_get_previous_color(&subject_);
}

// GroupSubject

GroupSubject::GroupSubject(const std::vector<Subject*>& subjects) {
  raw_subjects_.reserve(subjects.size());
  wrapped_subjects_ = subjects;
  for (auto* s : subjects) {
    raw_subjects_.push_back(s->raw());
  }
  lv_subject_init_group(&subject_, raw_subjects_.data(), raw_subjects_.size());
}

Subject* GroupSubject::get_element(int32_t index) {
  if (index < 0 || index >= static_cast<int32_t>(wrapped_subjects_.size())) {
    return nullptr;
  }
  return wrapped_subjects_[index];
}

// Observer

Observer::Observer(lv_observer_t* obs, bool owned) : obs_(obs), owned_(owned) {
  if (owned)
    printf("Observer created (owned) %p\n", (void*)obs);
  else
    printf("Observer created (unowned) %p\n", (void*)obs);
}

Observer::Observer(Observer&& other) noexcept
    : obs_(other.obs_), owned_(other.owned_) {
  other.obs_ = nullptr;
  other.owned_ = false;
  printf("Observer moved %p\n", (void*)obs_);
}

Observer& Observer::operator=(Observer&& other) noexcept {
  if (this != &other) {
    remove();
    obs_ = other.obs_;
    owned_ = other.owned_;
    other.obs_ = nullptr;
    other.owned_ = false;
    printf("Observer move-assigned %p\n", (void*)obs_);
  }
  return *this;
}

Observer::~Observer() {
  if (obs_) printf("Observer dtor %p owned=%d\n", (void*)obs_, owned_);
  if (owned_) {
    remove();
  }
}

void Observer::remove() {
  if (obs_) {
    lv_observer_remove(obs_);
    obs_ = nullptr;
  }
}

void* Observer::get_target() const {
  if (!obs_) return nullptr;
  return lv_observer_get_target(obs_);
}

lv_obj_t* Observer::get_target_obj() const {
  if (!obs_) return nullptr;
  return lv_observer_get_target_obj(obs_);
}

}  // namespace lvgl
