#ifndef LVGL_CPP_CORE_OBSERVER_H_
#define LVGL_CPP_CORE_OBSERVER_H_

#include "lvgl.h"
#include "object.h" // For bindings
#include <string>
#include <vector>

namespace lvgl {

/**
 * @brief Base Subject class wrapping lv_subject_t.
 * Subjects are observable values.
 * Note: Subject owns the lv_subject_t storage. It must not be moved/copied
 * trivially because C pointers in the observer list might reference it.
 */
class Subject {
public:
  virtual ~Subject();

  // Non-copyable, Non-moveable to ensure pointer stability
  Subject(const Subject &) = delete;
  Subject &operator=(const Subject &) = delete;
  Subject(Subject &&) = delete;
  Subject &operator=(Subject &&) = delete;

  lv_subject_t *raw() { return &subject_; }
  const lv_subject_t *raw() const { return &subject_; }

  void notify();

  // Flag Bindings
  void bind_flag_if_eq(Object &obj, lv_obj_flag_t flag, int32_t ref_value);
  void bind_flag_if_not_eq(Object &obj, lv_obj_flag_t flag, int32_t ref_value);
  void bind_flag_if_gt(Object &obj, lv_obj_flag_t flag, int32_t ref_value);
  void bind_flag_if_ge(Object &obj, lv_obj_flag_t flag, int32_t ref_value);
  void bind_flag_if_lt(Object &obj, lv_obj_flag_t flag, int32_t ref_value);
  void bind_flag_if_le(Object &obj, lv_obj_flag_t flag, int32_t ref_value);

  // State Bindings
  void bind_state_if_eq(Object &obj, lv_state_t state, int32_t ref_value);
  void bind_state_if_not_eq(Object &obj, lv_state_t state, int32_t ref_value);
  void bind_state_if_gt(Object &obj, lv_state_t state, int32_t ref_value);
  void bind_state_if_ge(Object &obj, lv_state_t state, int32_t ref_value);
  void bind_state_if_lt(Object &obj, lv_state_t state, int32_t ref_value);
  void bind_state_if_le(Object &obj, lv_state_t state, int32_t ref_value);

  // Checked Binding
  void bind_checked(Object &obj);

  /**
   * @brief Add a generic observer callback.
   * @param cb The callback function.
   * @param user_data Optional user data to pass to the callback.
   * @return A pointer to the newly created Observer.
   * @warning The returned Observer* is managed by the caller if not bound to an
   * LVGL object lifecycle.
   */
  [[nodiscard]] class Observer *add_observer(lv_observer_cb_t cb,
                                             void *user_data);

  /**
   * @brief Add a C++ observer callback.
   * @param cb The callback function.
   * @return A pointer to the newly created Observer.
   */
  // Note: We need a forward declaration or a robust way to handle std::function
  // without heavy includes if possible, but <functional> is standard.
  // doing this in the implementation or a specific method.
  // For now, let's stick to the C-style callback as a base, and maybe a C++
  // one.

protected:
  Subject(); // abstract
  lv_subject_t subject_;
};

class IntSubject : public Subject {
public:
  explicit IntSubject(int32_t value);

  void set(int32_t value);
  int32_t get();
  int32_t get_previous();

  void set_range(int32_t min, int32_t max);
};

class FloatSubject : public Subject {
public:
  explicit FloatSubject(float value);

  void set(float value);
  float get();
  float get_previous();

  void set_range(float min, float max);
};

class StringSubject : public Subject {
public:
  explicit StringSubject(const std::string &value, size_t buf_size = 128);
  ~StringSubject();

  void set(const std::string &value);
  const char *get();
  const char *get_previous();

private:
  std::vector<char> buf_;
  std::vector<char> prev_buf_;
};

class PointerSubject : public Subject {
public:
  explicit PointerSubject(void *ptr);

  void set(void *ptr);
  const void *get();
  const void *get_previous();
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
  explicit GroupSubject(const std::vector<Subject *> &subjects);

  // No specific set/get for group, as it aggregates others.
  // But we can get elements.
  Subject *get_element(int32_t index);

private:
  std::vector<lv_subject_t *> raw_subjects_;
  std::vector<Subject *> wrapped_subjects_;
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
  explicit Observer(lv_observer_t *obs, bool owned = false);
  ~Observer();

  // Move-only to prevent double-free if we own it.
  Observer(const Observer &) = delete;
  Observer &operator=(const Observer &) = delete;
  Observer(Observer &&other) noexcept;
  Observer &operator=(Observer &&other) noexcept;

  void remove();

  lv_observer_t *raw() const { return obs_; }

  /**
   * @brief Get the target object associated with this observer (if any).
   */
  void *get_target() const;

  /**
   * @brief Get the target widget associated with this observer (if any).
   * Returns raw lv_obj_t pointer.
   */
  lv_obj_t *get_target_obj() const;

private:
  lv_observer_t *obs_;
  bool owned_; // If true, we call lv_observer_remove in destructor.
};

} // namespace lvgl

#endif // LVGL_CPP_CORE_OBSERVER_H_
