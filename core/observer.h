#ifndef LVGL_CPP_CORE_OBSERVER_H_
#define LVGL_CPP_CORE_OBSERVER_H_

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include "lvgl.h"    // IWYU pragma: export
#include "object.h"  // For bindings // IWYU pragma: export

namespace lvgl {

#if LV_USE_OBSERVER

/**
 * @brief Base Subject class wrapping lv_subject_t.
 * Subjects are observable values.
 * Note: Subject owns the lv_subject_t storage. It must not be moved/copied
 * trivially because C pointers in the observer list might reference it.
 */
class Observer;
class Subject;

/**
 * @brief Idiomatic C++ callback for observers.
 * @param observer The observer instance that triggered the callback.
 */
using ObserverCallback = std::function<void(Observer*)>;

class Subject {
 public:
  virtual ~Subject();

  // Non-copyable, Non-moveable to ensure pointer stability
  Subject(const Subject&) = delete;
  Subject& operator=(const Subject&) = delete;
  Subject(Subject&&) = delete;
  Subject& operator=(Subject&&) = delete;

  lv_subject_t* raw() { return &subject_; }
  const lv_subject_t* raw() const { return &subject_; }

  void notify();

  void bind_flag_if_eq(lvgl::Object& obj, lv_obj_flag_t flag,
                       int32_t ref_value);
  void bind_flag_if_not_eq(lvgl::Object& obj, lv_obj_flag_t flag,
                           int32_t ref_value);
  void bind_flag_if_gt(lvgl::Object& obj, lv_obj_flag_t flag,
                       int32_t ref_value);
  void bind_flag_if_ge(lvgl::Object& obj, lv_obj_flag_t flag,
                       int32_t ref_value);
  void bind_flag_if_lt(lvgl::Object& obj, lv_obj_flag_t flag,
                       int32_t ref_value);
  void bind_flag_if_le(lvgl::Object& obj, lv_obj_flag_t flag,
                       int32_t ref_value);

  /**
   * @brief Bind an object flag to an integer value condition (Scoped Enum).
   */
  void bind_flag_if_eq(Object& obj, ObjFlag flag, int32_t ref_value);
  void bind_flag_if_not_eq(Object& obj, ObjFlag flag, int32_t ref_value);
  void bind_flag_if_gt(Object& obj, ObjFlag flag, int32_t ref_value);
  void bind_flag_if_ge(Object& obj, ObjFlag flag, int32_t ref_value);
  void bind_flag_if_lt(Object& obj, ObjFlag flag, int32_t ref_value);
  void bind_flag_if_le(Object& obj, ObjFlag flag, int32_t ref_value);

  void bind_state_if_eq(lvgl::Object& obj, lv_state_t state, int32_t ref_value);
  void bind_state_if_not_eq(lvgl::Object& obj, lv_state_t state,
                            int32_t ref_value);
  void bind_state_if_gt(lvgl::Object& obj, lv_state_t state, int32_t ref_value);
  void bind_state_if_ge(lvgl::Object& obj, lv_state_t state, int32_t ref_value);
  void bind_state_if_lt(lvgl::Object& obj, lv_state_t state, int32_t ref_value);
  void bind_state_if_le(lvgl::Object& obj, lv_state_t state, int32_t ref_value);

  /**
   * @brief Bind an object state to an integer value condition (Scoped Enum).
   */
  void bind_state_if_eq(Object& obj, State state, int32_t ref_value);
  void bind_state_if_not_eq(Object& obj, State state, int32_t ref_value);
  void bind_state_if_gt(Object& obj, State state, int32_t ref_value);
  void bind_state_if_ge(Object& obj, State state, int32_t ref_value);
  void bind_state_if_lt(Object& obj, State state, int32_t ref_value);
  void bind_state_if_le(Object& obj, State state, int32_t ref_value);

  // Checked Binding

  /**
   * @brief Bind the LV_STATE_CHECKED state to the subject's boolean value.
   * The subject acts as a boolean (0 or 1).
   * @param obj The target object (often a Checkbox or Switch).
   */
  void bind_checked(Object& obj);

  /**
   * @brief Add a generic observer callback.
   * @param cb The callback function.
   * @param user_data Optional user data to pass to the callback.
   * @return A pointer to the newly created Observer.
   * @warning The returned Observer* is managed by the caller if not bound to an
   * LVGL object lifecycle.
   */
  [[nodiscard]] class Observer* add_observer(lv_observer_cb_t cb,
                                             void* user_data);

  /**
   * @brief Add a C++ observer callback.
   * @param cb The callback function (lambda, std::function, etc.).
   * @return A pointer to the newly created Observer.
   */
  [[nodiscard]] class Observer* add_observer(ObserverCallback cb);

  /**
   * @brief Add a C++ observer callback tied to an object.
   * @param obj The object to bind to.
   * @param cb The callback function.
   * @return A pointer to the newly created Observer.
   */
  [[nodiscard]] class Observer* add_observer_obj(Object& obj,
                                                 ObserverCallback cb);

 protected:
  Subject();  // abstract
  lv_subject_t subject_;
};

// ... existing code ...

class StringSubject : public Subject {
 public:
  explicit StringSubject(const std::string& value, size_t buf_size = 128);
  ~StringSubject();

  void set(const std::string& value);
  const char* get();
  const char* get_previous();

  /**
   * @brief Set the value using a printf-style format string.
   * @param fmt Format string.
   */
  void set_formatted(const char* fmt, ...);

 private:
  std::vector<char> buf_;
  std::vector<char> prev_buf_;
};

/**
 * @brief Subject holding an integer value.
 */
class IntSubject : public Subject {
 public:
  explicit IntSubject(int32_t value);

  void set(int32_t value);
  int32_t get();
  int32_t get_previous();

  /**
   * @brief Set the valid range for the integer value.
   * @param min Minimum value.
   * @param max Maximum value.
   */
  void set_range(int32_t min, int32_t max);
};

#if LV_USE_FLOAT
class FloatSubject : public Subject {
 public:
  explicit FloatSubject(float value);

  void set(float value);
  float get();
  float get_previous();

  void set_range(float min, float max);
};
#endif  // LV_USE_FLOAT

class PointerSubject : public Subject {
 public:
  explicit PointerSubject(void* ptr);

  void set(void* ptr);
  const void* get();
  const void* get_previous();
};

class ColorSubject : public Subject {
 public:
  explicit ColorSubject(lv_color_t color);

  void set(lv_color_t color);
  lv_color_t get();
  lv_color_t get_previous();
};

class GroupSubject : public Subject {
 public:
  /**
   * @brief Create a group subject from a list of subjects.
   * Warning: The subjects in the list must outlive the GroupSubject.
   * @param subjects List of subjects to group.
   */
  explicit GroupSubject(const std::vector<Subject*>& subjects);

  // No specific set/get for group, as it aggregates others.
  // But we can get elements.
  Subject* get_element(int32_t index);

 private:
  std::vector<lv_subject_t*> raw_subjects_;
  std::vector<Subject*> wrapped_subjects_;
};

/**
 * @brief Observer wrapper.
 * Represents a subscription to a Subject.
 *
 * Life-cycle management:
 * - If `owned` is true (default for bind functions that assume RAII), the
 * observer is removed when this C++ object goes out of scope.
 * - If `owned` is false, it is a lightweight wrapper around the raw
 * `lv_observer_t*`.
 *
 * Usage:
 * Keep the returned Observer object alive as long as you want the subscription
 * to be active.
 */

class Observer {
 public:
  friend class Subject;
  /**
   * @brief Create an observer for a specific subject with a C++ callback.
   * @param subject The subject to observe.
   * @param cb The callback function to invoke.
   */
  Observer(Subject& subject, ObserverCallback cb);

  explicit Observer(lv_observer_t* obs, bool owned = false);
  ~Observer();

  // Move-only to prevent double-free if we own it.
  Observer(const Observer&) = delete;
  Observer& operator=(const Observer&) = delete;
  Observer(Observer&& other) noexcept;
  Observer& operator=(Observer&& other) noexcept;

  void remove();

  lv_observer_t* raw() const { return obs_; }

  /**
   * @brief Get the target object associated with this observer (if any).
   */
  void* get_target() const;

  /**
   * @brief Get the target widget associated with this observer (if any).
   * Returns raw lv_obj_t pointer.
   */
  lv_obj_t* get_target_obj() const;

  // Internal use for static shim
  const ObserverCallback& get_callback() const { return callback_; }

 private:
  lv_observer_t* obs_ = nullptr;
  bool owned_;  // If true, we call lv_observer_remove in destructor.
  ObserverCallback callback_;
};

#endif  // LV_USE_OBSERVER

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_OBSERVER_H_
