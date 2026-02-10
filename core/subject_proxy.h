#ifndef LVGL_CPP_CORE_SUBJECT_PROXY_H_
#define LVGL_CPP_CORE_SUBJECT_PROXY_H_

#include "../misc/enums.h"
#include "compatibility.h"
#include "lvgl.h"

namespace lvgl {

#if LV_USE_OBSERVER

class Subject;  // Forward declaration

/**
 * @brief Proxy class for fluent subject-object interaction binding.
 * Returned by Object::on_subject().
 */
class SubjectProxy {
 public:
  struct IncrementProxy {
    IncrementProxy(lv_obj_t* obj, lv_subject_increment_dsc_t* dsc)
        : obj_(obj), dsc_(dsc) {}

    IncrementProxy& min(int32_t val) {
      if (obj_ && dsc_)
        lv_obj_set_subject_increment_event_min_value(obj_, dsc_, val);
      return *this;
    }

    IncrementProxy& max(int32_t val) {
      if (obj_ && dsc_)
        lv_obj_set_subject_increment_event_max_value(obj_, dsc_, val);
      return *this;
    }

    IncrementProxy& rollover(bool en) {
      if (obj_ && dsc_)
        lv_obj_set_subject_increment_event_rollover(obj_, dsc_, en);
      return *this;
    }

   private:
    lv_obj_t* obj_;
    lv_subject_increment_dsc_t* dsc_;
  };

  SubjectProxy(lv_obj_t* obj, lv_subject_t* subject)
      : obj_(obj), subject_(subject) {}

  /**
   * @brief Toggle the subject's value when the object is clicked.
   * @param trigger The event code that triggers the toggle (default:
   * EventCode::Clicked).
   */
  void toggle(EventCode trigger = EventCode::Clicked) {
    if (obj_ && subject_)
      lv_obj_add_subject_toggle_event(obj_, subject_,
                                      static_cast<lv_event_code_t>(trigger));
  }

  /**
   * @brief Increment the subject's value when the object is clicked.
   * @param step The amount to increment.
   * @param trigger The event code that triggers the increment (default:
   * EventCode::Clicked).
   */
  IncrementProxy increment(int32_t step,
                           EventCode trigger = EventCode::Clicked) {
    lv_subject_increment_dsc_t* dsc = nullptr;
    if (obj_ && subject_)
      dsc = lv_obj_add_subject_increment_event(
          obj_, subject_, static_cast<lv_event_code_t>(trigger), step);
    return IncrementProxy(obj_, dsc);
  }

  /**
   * @brief Set the subject's integer value when the object is clicked.
   * @param value The value to set.
   * @param trigger The event code that triggers the set (default:
   * EventCode::Clicked).
   */
  void set_int(int32_t value, EventCode trigger = EventCode::Clicked) {
    if (obj_ && subject_)
      lv_obj_add_subject_set_int_event(
          obj_, subject_, static_cast<lv_event_code_t>(trigger), value);
  }

  /**
   * @brief Set the subject's string value when the object is clicked.
   * @param value The value to set.
   * @param trigger The event code that triggers the set (default:
   * EventCode::Clicked).
   */
  void set_string(const char* value, EventCode trigger = EventCode::Clicked) {
    if (obj_ && subject_)
      lv_obj_add_subject_set_string_event(
          obj_, subject_, static_cast<lv_event_code_t>(trigger), value);
  }

 private:
  lv_obj_t* obj_;
  lv_subject_t* subject_;
};

#endif  // LV_USE_OBSERVER

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_SUBJECT_PROXY_H_
