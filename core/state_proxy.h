#ifndef LVGL_CPP_CORE_STATE_PROXY_H_
#define LVGL_CPP_CORE_STATE_PROXY_H_

#include "../misc/enums.h"
#include "lvgl.h"

namespace lvgl {

class Object;

/**
 * @brief Transient proxy object for managing states on an object.
 *
 * This class is returned by `Object::state()` and provides a fluent API
 * for adding, removing, and checking widget states (e.g. Pressed, Checked).
 */
class StateProxy {
 public:
  explicit StateProxy(lv_obj_t* obj) : obj_(obj) {}

  /**
   * @brief Add a state to the object.
   * @param state The state to add.
   * @return Reference to this proxy for chaining.
   */
  StateProxy& add(State state) {
    if (obj_) lv_obj_add_state(obj_, static_cast<lv_state_t>(state));
    return *this;
  }

  /**
   * @brief Remove a state from the object.
   * @param state The state to remove.
   * @return Reference to this proxy for chaining.
   */
  StateProxy& remove(State state) {
    if (obj_) lv_obj_remove_state(obj_, static_cast<lv_state_t>(state));
    return *this;
  }

  /**
   * @brief Check if a state is active on the object.
   * @param state The state to check.
   * @return true if the state is active.
   */
  bool is(State state) const {
    return obj_ ? lv_obj_has_state(obj_, static_cast<lv_state_t>(state))
                : false;
  }

  /**
   * @brief Set or clear a state based on a boolean value.
   * @param state The state to set/clear.
   * @param value if true, add state; if false, remove state.
   * @return Reference to this proxy for chaining.
   */
  StateProxy& set(State state, bool value) {
    if (value)
      add(state);
    else
      remove(state);
    return *this;
  }

  // --- Convenience Helpers ---

  bool is_checked() const { return is(State::Checked); }
  bool is_pressed() const { return is(State::Pressed); }
  bool is_focused() const { return is(State::Focused); }
  bool is_disabled() const { return is(State::Disabled); }

  StateProxy& set_checked(bool v) { return set(State::Checked, v); }
  StateProxy& set_disabled(bool v) { return set(State::Disabled, v); }

 private:
  lv_obj_t* obj_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_STATE_PROXY_H_
