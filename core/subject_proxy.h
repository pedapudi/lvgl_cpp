#ifndef LVGL_CPP_CORE_SUBJECT_PROXY_H_
#define LVGL_CPP_CORE_SUBJECT_PROXY_H_

#include "../misc/enums.h"
#include "lvgl.h"

namespace lvgl {

class Subject;  // Forward declaration

/**
 * @brief Proxy class for fluent subject-object interaction binding.
 * Returned by Object::on_subject().
 */
class SubjectProxy {
 public:
  struct IncrementProxy {
    IncrementProxy(lv_obj_t* obj, lv_subject_t* subject,
                   lv_event_code_t trigger)
        : obj_(obj), subject_(subject), trigger_(trigger) {}

    IncrementProxy& min(int32_t val) {
      if (obj_ && subject_)
        lv_obj_set_subject_increment_min_value(obj_, subject_, trigger_, val);
      return *this;
    }

    IncrementProxy& max(int32_t val) {
      if (obj_ && subject_)
        lv_obj_set_subject_increment_max_value(obj_, subject_, trigger_, val);
      return *this;
    }

    IncrementProxy& rollover(bool en) {
      if (obj_ && subject_)
        lv_obj_set_subject_increment_rollover(obj_, subject_, trigger_, en);
      return *this;
    }

   private:
    lv_obj_t* obj_;
    lv_subject_t* subject_;
    lv_event_code_t trigger_;
  };

  SubjectProxy(lv_obj_t* obj, lv_subject_t* subject)
      : obj_(obj), subject_(subject) {}

  /**
   * @brief Toggle the subject's value when the object is clicked.
   * @param trigger The event code that triggers the toggle (default:
   * LV_EVENT_CLICKED).
   */
  void toggle(lv_event_code_t trigger = LV_EVENT_CLICKED) {
    if (obj_ && subject_)
      lv_obj_add_subject_toggle_event(obj_, subject_, trigger);
  }

  void toggle(EventCode trigger) {
    toggle(static_cast<lv_event_code_t>(trigger));
  }

  /**
   * @brief Increment the subject's value when the object is clicked.
   * @param step The amount to increment.
   * @param trigger The event code that triggers the increment (default:
   * LV_EVENT_CLICKED).
   */
  IncrementProxy increment(int32_t step,
                           lv_event_code_t trigger = LV_EVENT_CLICKED) {
    if (obj_ && subject_)
      lv_obj_add_subject_increment_event(obj_, subject_, trigger, step);
    return IncrementProxy(obj_, subject_, trigger);
  }

  IncrementProxy increment(int32_t step, EventCode trigger) {
    return increment(step, static_cast<lv_event_code_t>(trigger));
  }

  /**
   * @brief Set the subject's integer value when the object is clicked.
   * @param value The value to set.
   * @param trigger The event code that triggers the set (default:
   * LV_EVENT_CLICKED).
   */
  void set_int(int32_t value, lv_event_code_t trigger = LV_EVENT_CLICKED) {
    if (obj_ && subject_)
      lv_obj_add_subject_set_int_event(obj_, subject_, trigger, value);
  }

  void set_int(int32_t value, EventCode trigger) {
    set_int(value, static_cast<lv_event_code_t>(trigger));
  }

  /**
   * @brief Set the subject's string value when the object is clicked.
   * @param value The value to set.
   * @param trigger The event code that triggers the set (default:
   * LV_EVENT_CLICKED).
   */
  void set_string(const char* value,
                  lv_event_code_t trigger = LV_EVENT_CLICKED) {
    if (obj_ && subject_)
      lv_obj_add_subject_set_string_event(obj_, subject_, trigger, value);
  }

  void set_string(const char* value, EventCode trigger) {
    set_string(value, static_cast<lv_event_code_t>(trigger));
  }

 private:
  lv_obj_t* obj_;
  lv_subject_t* subject_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_SUBJECT_PROXY_H_
