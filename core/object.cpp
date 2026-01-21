#include "object.h"

#include <vector>

#include "event.h"
#include "lvgl.h"
#include "style.h"

namespace lvgl {

Object::Object() : obj_(lv_obj_create(nullptr)), owned_(true) {
  if (obj_) {
    install_delete_hook();
  }
}

Object::Object(lv_obj_t* obj, Ownership ownership) : obj_(obj) {
  if (ownership == Ownership::Default) {
    if (obj_) owned_ = false;  // Default for wrapping existing obj is Unmanaged
  } else {
    owned_ = (ownership == Ownership::Managed);
  }
  if (obj_) {
    install_delete_hook();
  }
}

Object::Object(Object* parent, Ownership ownership) {
  obj_ = lv_obj_create(parent ? parent->raw() : nullptr);
  if (ownership == Ownership::Default) {
    owned_ = true;  // Default for new child is Owned
  } else {
    owned_ = (ownership == Ownership::Managed);
  }
  if (obj_) {
    install_delete_hook();
  }
}

Object::~Object() {
  if (obj_) {
    // Clean up event callbacks registered by this wrapper instance
    for (auto& node : callback_nodes_) {
      lv_obj_remove_event_cb_with_user_data(obj_, event_proxy, node.get());
    }
    // Remove the internal delete hook to prevent use-after-free of 'this'
    lv_obj_remove_event_cb_with_user_data(obj_, on_delete_event, this);

    if (owned_) {
      lv_obj_delete(obj_);
    }
    obj_ = nullptr;
  }
}

Object::Object(Object&& other) noexcept
    : obj_(other.obj_), owned_(other.owned_) {
  other.obj_ = nullptr;
  other.owned_ = false;
  if (obj_) {
    // Easier way: Remove old callback and add new one with new 'this'
    lv_obj_remove_event_cb_with_user_data(obj_, on_delete_event, &other);
    install_delete_hook();
  }
}

Object& Object::operator=(Object&& other) noexcept {
  if (this != &other) {
    if (obj_) {
      // ALWAYS remove our callback first.
      lv_obj_remove_event_cb_with_user_data(obj_, on_delete_event, this);
      if (owned_) {
        lv_obj_delete(obj_);
      }
    }

    obj_ = other.obj_;
    owned_ = other.owned_;
    other.obj_ = nullptr;
    other.owned_ = false;

    if (obj_) {
      // Remove the old wrapper's callback and install ours
      lv_obj_remove_event_cb_with_user_data(obj_, on_delete_event, &other);
      install_delete_hook();
    }
  }
  return *this;
}

lv_obj_t* Object::release() {
  lv_obj_t* ptr = obj_;
  owned_ = false;
  return ptr;
}

void Object::install_delete_hook() {
  fprintf(stderr, "Install hook: obj=%p, wrapper=%p\n", obj_, this);
  lv_obj_add_event_cb(obj_, on_delete_event, LV_EVENT_DELETE, this);
}

void Object::on_delete_event(lv_event_t* e) {
  Object* self = static_cast<Object*>(lv_event_get_user_data(e));
  fprintf(stderr, "On delete event trigger: obj=%p, wrapper=%p\n",
          lv_event_get_target(e), self);
  if (self) {
    self->obj_ = nullptr;  // Invalidate wrapper
  }
}

// --- Flags & States ---

void Object::add_flag(lv_obj_flag_t f) {
  if (obj_) lv_obj_add_flag(obj_, f);
}

void Object::add_flag(ObjFlag f) { add_flag(static_cast<lv_obj_flag_t>(f)); }

void Object::remove_flag(lv_obj_flag_t f) {
  if (obj_) lv_obj_remove_flag(obj_, f);
}

void Object::remove_flag(ObjFlag f) {
  remove_flag(static_cast<lv_obj_flag_t>(f));
}

bool Object::has_flag(lv_obj_flag_t f) const {
  return obj_ ? lv_obj_has_flag(obj_, f) : false;
}

bool Object::has_flag(ObjFlag f) const {
  return has_flag(static_cast<lv_obj_flag_t>(f));
}

void Object::add_state(State state) {
  if (obj_) lv_obj_add_state(obj_, static_cast<lv_state_t>(state));
}

void Object::remove_state(State state) {
  if (obj_) lv_obj_remove_state(obj_, static_cast<lv_state_t>(state));
}

bool Object::has_state(State state) const {
  return obj_ ? lv_obj_has_state(obj_, static_cast<lv_state_t>(state)) : false;
}

void Object::add_state(lv_state_t state) {
  if (obj_) lv_obj_add_state(obj_, state);
}

void Object::remove_state(lv_state_t state) {
  if (obj_) lv_obj_remove_state(obj_, state);
}

bool Object::has_state(lv_state_t state) const {
  return obj_ ? lv_obj_has_state(obj_, state) : false;
}

// --- Scroll ---

void Object::set_scrollbar_mode(lv_scrollbar_mode_t mode) {
  if (obj_) lv_obj_set_scrollbar_mode(obj_, mode);
}

void Object::set_scrollbar_mode(ScrollbarMode mode) {
  set_scrollbar_mode(static_cast<lv_scrollbar_mode_t>(mode));
}

void Object::set_scroll_snap_x(lv_scroll_snap_t snap) {
  if (obj_) lv_obj_set_scroll_snap_x(obj_, snap);
}

void Object::set_scroll_snap_x(ScrollSnap snap) {
  set_scroll_snap_x(static_cast<lv_scroll_snap_t>(snap));
}

void Object::set_scroll_snap_y(lv_scroll_snap_t snap) {
  if (obj_) lv_obj_set_scroll_snap_y(obj_, snap);
}

void Object::set_scroll_snap_y(ScrollSnap snap) {
  set_scroll_snap_y(static_cast<lv_scroll_snap_t>(snap));
}

// --- Other Properties ---

void Object::set_base_dir(lv_base_dir_t dir) {
  if (obj_) lv_obj_set_style_base_dir(obj_, dir, LV_PART_MAIN);
}

void Object::set_base_dir(BaseDir dir) {
  set_base_dir(static_cast<lv_base_dir_t>(dir));
}

// --- Events ---

void Object::event_proxy(lv_event_t* e) {
  auto* node = static_cast<CallbackNode*>(lv_event_get_user_data(e));
  if (node && node->callback) {
    Event event(e);
    node->callback(event);
  }
}

void Object::add_event_cb(lv_event_code_t event_code, EventCallback callback) {
  if (!obj_) return;
  auto node = std::make_unique<CallbackNode>();
  node->event_code = event_code;
  node->callback = std::move(callback);
  lv_obj_add_event_cb(obj_, event_proxy, event_code, node.get());
  callback_nodes_.push_back(std::move(node));
}

void Object::add_event_cb(EventCode event_code, EventCallback callback) {
  add_event_cb(static_cast<lv_event_code_t>(event_code), std::move(callback));
}

// --- Styles ---

void Object::add_style(Style& style, lv_style_selector_t selector) {
  if (obj_) lv_obj_add_style(obj_, style.raw(), selector);
}

void Object::remove_style(Style* style, lv_style_selector_t selector) {
  if (obj_) lv_obj_remove_style(obj_, style ? style->raw() : nullptr, selector);
}

void Object::set_style_anim_duration(uint32_t value,
                                     lv_style_selector_t selector) {
  if (obj_) lv_obj_set_style_anim_duration(obj_, value, selector);
}

void Object::set_style_text_align(lv_text_align_t value,
                                  lv_style_selector_t selector) {
  if (obj_) lv_obj_set_style_text_align(obj_, value, selector);
}

void Object::set_style_text_align(TextAlign value,
                                  lv_style_selector_t selector) {
  set_style_text_align(static_cast<lv_text_align_t>(value), selector);
}

void Object::set_style_bg_color(lv_color_t value,
                                lv_style_selector_t selector) {
  if (obj_) lv_obj_set_style_bg_color(obj_, value, selector);
}

void Object::set_style_bg_opa(lv_opa_t value, lv_style_selector_t selector) {
  if (obj_) lv_obj_set_style_bg_opa(obj_, value, selector);
}

void Object::set_style_image_recolor_opa(lv_opa_t value,
                                         lv_style_selector_t selector) {
  if (obj_) lv_obj_set_style_image_recolor_opa(obj_, value, selector);
}

void Object::set_style_image_recolor(lv_color_t value,
                                     lv_style_selector_t selector) {
  if (obj_) lv_obj_set_style_image_recolor(obj_, value, selector);
}

void Object::set_style_bg_image_src(const void* value,
                                    lv_style_selector_t selector) {
  if (obj_) lv_obj_set_style_bg_image_src(obj_, value, selector);
}

// --- Layout & Scroll ---

void Object::set_flex_flow(FlexFlow flow) {
  if (obj_) lv_obj_set_flex_flow(obj_, static_cast<lv_flex_flow_t>(flow));
}

void Object::set_flex_align(FlexAlign main_place, FlexAlign cross_place,
                            FlexAlign track_cross_place) {
  if (obj_) {
    lv_obj_set_flex_align(obj_, static_cast<lv_flex_align_t>(main_place),
                          static_cast<lv_flex_align_t>(cross_place),
                          static_cast<lv_flex_align_t>(track_cross_place));
  }
}

void Object::set_grid_align(GridAlign column_align, GridAlign row_align) {
  if (obj_) {
    lv_obj_set_grid_align(obj_, static_cast<lv_grid_align_t>(column_align),
                          static_cast<lv_grid_align_t>(row_align));
  }
}

}  // namespace lvgl
