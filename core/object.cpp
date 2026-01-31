#include "object.h"

#include <vector>

#include "../misc/layout.h"
#include "../misc/style.h"
#include "event.h"
#include "lvgl.h"

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

// --- Object Tree Management ---

void Object::clean() {
  if (obj_) lv_obj_clean(obj_);
}

Object Object::get_parent() const {
  if (!obj_)
    return Object(static_cast<lv_obj_t*>(nullptr), Ownership::Unmanaged);
  lv_obj_t* parent = lv_obj_get_parent(obj_);
  return Object(parent, Ownership::Unmanaged);
}

Object Object::get_child(int32_t index) const {
  if (!obj_)
    return Object(static_cast<lv_obj_t*>(nullptr), Ownership::Unmanaged);
  lv_obj_t* child = lv_obj_get_child(obj_, index);
  return Object(child, Ownership::Unmanaged);
}

uint32_t Object::get_child_count() const {
  return obj_ ? lv_obj_get_child_count(obj_) : 0;
}

void Object::set_parent(Object& parent) {
  if (obj_ && parent.raw()) lv_obj_set_parent(obj_, parent.raw());
}

void Object::set_parent(lv_obj_t* parent) {
  if (obj_ && parent) lv_obj_set_parent(obj_, parent);
}

int32_t Object::get_index() const { return obj_ ? lv_obj_get_index(obj_) : -1; }

void Object::move_foreground() {
  if (obj_) lv_obj_move_foreground(obj_);
}

void Object::move_background() {
  if (obj_) lv_obj_move_background(obj_);
}

void Object::delete_async() {
  if (obj_) {
    owned_ = false;  // Prevent double-delete
    lv_obj_delete_async(obj_);
  }
}

// --- Geometric Properties ---

Object& Object::set_x(int32_t x) {
  if (obj_) lv_obj_set_x(obj_, x);
  return *this;
}

Object& Object::set_y(int32_t y) {
  if (obj_) lv_obj_set_y(obj_, y);
  return *this;
}

Object& Object::set_pos(int32_t x, int32_t y) {
  if (obj_) lv_obj_set_pos(obj_, x, y);
  return *this;
}

Object& Object::align(Align align, int32_t x_ofs, int32_t y_ofs) {
  if (obj_) {
    lv_obj_set_align(obj_, static_cast<lv_align_t>(align));
    lv_obj_set_pos(obj_, x_ofs, y_ofs);
  }
  return *this;
}

Object& Object::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  if (obj_) {
    lv_obj_set_align(obj_, align);
    lv_obj_set_pos(obj_, x_ofs, y_ofs);
  }
  return *this;
}

Object& Object::align_to(const Object& base, Align align, int32_t x_ofs,
                         int32_t y_ofs) {
  if (obj_ && base.raw()) {
    lv_obj_align_to(obj_, base.raw(), static_cast<lv_align_t>(align), x_ofs,
                    y_ofs);
  }
  return *this;
}

Object& Object::align_to(const Object& base, lv_align_t align, int32_t x_ofs,
                         int32_t y_ofs) {
  if (obj_ && base.raw()) {
    lv_obj_align_to(obj_, base.raw(), align, x_ofs, y_ofs);
  }
  return *this;
}

Object& Object::center() {
  if (obj_) lv_obj_center(obj_);
  return *this;
}

int32_t Object::get_x() const { return obj_ ? lv_obj_get_x(obj_) : 0; }

int32_t Object::get_y() const { return obj_ ? lv_obj_get_y(obj_) : 0; }

Object& Object::set_width(int32_t w) {
  if (obj_) lv_obj_set_width(obj_, w);
  return *this;
}

Object& Object::set_height(int32_t h) {
  if (obj_) lv_obj_set_height(obj_, h);
  return *this;
}

Object& Object::set_size(int32_t w, int32_t h) {
  if (obj_) lv_obj_set_size(obj_, w, h);
  return *this;
}

int32_t Object::get_width() const { return obj_ ? lv_obj_get_width(obj_) : 0; }

int32_t Object::get_height() const {
  return obj_ ? lv_obj_get_height(obj_) : 0;
}

void Object::install_delete_hook() {
  lv_obj_add_event_cb(obj_, on_delete_event, LV_EVENT_DELETE, this);
}

void Object::on_delete_event(lv_event_t* e) {
  Object* self = static_cast<Object*>(lv_event_get_user_data(e));
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

void Object::add_state(lv_state_t s) {
  if (obj_) lv_obj_add_state(obj_, s);
}

void Object::add_state(State s) { add_state(static_cast<lv_state_t>(s)); }

void Object::remove_state(lv_state_t s) {
  if (obj_) lv_obj_remove_state(obj_, s);
}

void Object::remove_state(State s) { remove_state(static_cast<lv_state_t>(s)); }

bool Object::has_state(lv_state_t s) const {
  return obj_ ? lv_obj_has_state(obj_, s) : false;
}

bool Object::has_state(State s) const {
  return has_state(static_cast<lv_state_t>(s));
}

// --- Layout Shortcuts ---

Object& Object::set_flex_flow(lv_flex_flow_t flow) {
  if (obj_) lv_obj_set_flex_flow(obj_, flow);
  return *this;
}

Object& Object::set_flex_align(lv_flex_align_t main_place,
                               lv_flex_align_t cross_place,
                               lv_flex_align_t track_place) {
  if (obj_) lv_obj_set_flex_align(obj_, main_place, cross_place, track_place);
  return *this;
}

Object& Object::set_flex_grow(uint8_t grow) {
  if (obj_) lv_obj_set_flex_grow(obj_, grow);
  return *this;
}

Object& Object::set_grid_dsc_array(const GridLayout& grid) {
  if (obj_) {
    lv_obj_set_grid_dsc_array(obj_, grid.col_dsc(), grid.row_dsc());
  }
  return *this;
}

Object& Object::set_grid_align(lv_grid_align_t column_align,
                               lv_grid_align_t row_align) {
  if (obj_) lv_obj_set_grid_align(obj_, column_align, row_align);
  return *this;
}

Object& Object::set_grid_cell(lv_grid_align_t column_align, int32_t col_pos,
                              int32_t col_span, lv_grid_align_t row_align,
                              int32_t row_pos, int32_t row_span) {
  if (obj_) {
    lv_obj_set_grid_cell(obj_, column_align, col_pos, col_span, row_align,
                         row_pos, row_span);
  }
  return *this;
}

// --- Scroll ---

Object& Object::scroll_to_view(AnimEnable anim_en) {
  return scroll_to_view(static_cast<lv_anim_enable_t>(anim_en));
}

Object& Object::scroll_to_view(lv_anim_enable_t anim_en) {
  if (obj_) lv_obj_scroll_to_view(obj_, anim_en);
  return *this;
}

Object& Object::scroll_to_view_recursive(AnimEnable anim_en) {
  return scroll_to_view_recursive(static_cast<lv_anim_enable_t>(anim_en));
}

Object& Object::scroll_to_view_recursive(lv_anim_enable_t anim_en) {
  if (obj_) lv_obj_scroll_to_view_recursive(obj_, anim_en);
  return *this;
}

Object& Object::scroll_by(int32_t x, int32_t y, AnimEnable anim_en) {
  return scroll_by(x, y, static_cast<lv_anim_enable_t>(anim_en));
}

Object& Object::scroll_by(int32_t x, int32_t y, lv_anim_enable_t anim_en) {
  if (obj_) lv_obj_scroll_by(obj_, x, y, anim_en);
  return *this;
}

Object& Object::scroll_to(int32_t x, int32_t y, AnimEnable anim_en) {
  return scroll_to(x, y, static_cast<lv_anim_enable_t>(anim_en));
}

Object& Object::scroll_to(int32_t x, int32_t y, lv_anim_enable_t anim_en) {
  if (obj_) lv_obj_scroll_to(obj_, x, y, anim_en);
  return *this;
}

int32_t Object::get_scroll_x() const {
  return obj_ ? lv_obj_get_scroll_x(obj_) : 0;
}

int32_t Object::get_scroll_y() const {
  return obj_ ? lv_obj_get_scroll_y(obj_) : 0;
}

int32_t Object::get_scroll_top() const {
  return obj_ ? lv_obj_get_scroll_top(obj_) : 0;
}

int32_t Object::get_scroll_bottom() const {
  return obj_ ? lv_obj_get_scroll_bottom(obj_) : 0;
}

int32_t Object::get_scroll_left() const {
  return obj_ ? lv_obj_get_scroll_left(obj_) : 0;
}

int32_t Object::get_scroll_right() const {
  return obj_ ? lv_obj_get_scroll_right(obj_) : 0;
}

lv_scrollbar_mode_t Object::get_scrollbar_mode() const {
  return obj_ ? lv_obj_get_scrollbar_mode(obj_) : LV_SCROLLBAR_MODE_OFF;
}

lv_dir_t Object::get_scroll_dir() const {
  return obj_ ? lv_obj_get_scroll_dir(obj_) : LV_DIR_NONE;
}

lv_scroll_snap_t Object::get_scroll_snap_x() const {
  return obj_ ? lv_obj_get_scroll_snap_x(obj_) : LV_SCROLL_SNAP_NONE;
}

lv_scroll_snap_t Object::get_scroll_snap_y() const {
  return obj_ ? lv_obj_get_scroll_snap_y(obj_) : LV_SCROLL_SNAP_NONE;
}

int32_t Object::get_content_width() const {
  return obj_ ? lv_obj_get_content_width(obj_) : 0;
}

int32_t Object::get_content_height() const {
  return obj_ ? lv_obj_get_content_height(obj_) : 0;
}

int32_t Object::get_self_width() const {
  return obj_ ? lv_obj_get_self_width(obj_) : 0;
}

int32_t Object::get_self_height() const {
  return obj_ ? lv_obj_get_self_height(obj_) : 0;
}

Area Object::get_coords() const {
  Area a;
  if (obj_) lv_obj_get_coords(obj_, a.raw());
  return a;
}

Area Object::get_content_coords() const {
  Area a;
  if (obj_) lv_obj_get_content_coords(obj_, a.raw());
  return a;
}

Area Object::get_click_area() const {
  Area a;
  if (obj_) lv_obj_get_click_area(obj_, a.raw());
  return a;
}

Point Object::transform_point(const Point& p, bool recursive,
                              bool inverse) const {
  Point res = p;
  if (obj_) {
    lv_obj_point_transform_flag_t flags = LV_OBJ_POINT_TRANSFORM_FLAG_NONE;
    if (recursive)
      flags = static_cast<lv_obj_point_transform_flag_t>(
          flags | LV_OBJ_POINT_TRANSFORM_FLAG_RECURSIVE);
    if (inverse)
      flags = static_cast<lv_obj_point_transform_flag_t>(
          flags | LV_OBJ_POINT_TRANSFORM_FLAG_INVERSE);
    lv_obj_transform_point(obj_, res.raw(), flags);
  }
  return res;
}

Area Object::get_transformed_area(const Area& area, bool recursive,
                                  bool inverse) const {
  Area res = area;
  if (obj_) {
    lv_obj_point_transform_flag_t flags = LV_OBJ_POINT_TRANSFORM_FLAG_NONE;
    if (recursive)
      flags = static_cast<lv_obj_point_transform_flag_t>(
          flags | LV_OBJ_POINT_TRANSFORM_FLAG_RECURSIVE);
    if (inverse)
      flags = static_cast<lv_obj_point_transform_flag_t>(
          flags | LV_OBJ_POINT_TRANSFORM_FLAG_INVERSE);
    lv_obj_get_transformed_area(obj_, res.raw(), flags);
  }
  return res;
}

void Object::invalidate_area(const Area& area) {
  if (obj_) lv_obj_invalidate_area(obj_, area.raw());
}

bool Object::is_area_visible(const Area& area) const {
  if (!obj_) return false;
  lv_area_t copy = area;
  return lv_obj_area_is_visible(obj_, &copy);
}

void Object::refresh_ext_draw_size() {
  if (obj_) lv_obj_refresh_ext_draw_size(obj_);
}

void Object::redraw(lv_layer_t* layer) {
  if (obj_) lv_obj_redraw(layer, obj_);
}

// --- Animations ---

Object& Object::fade_in(uint32_t time, uint32_t delay) {
  if (obj_) lv_obj_fade_in(obj_, time, delay);
  return *this;
}

Object& Object::fade_in(std::chrono::milliseconds time,
                        std::chrono::milliseconds delay) {
  return fade_in(static_cast<uint32_t>(time.count()),
                 static_cast<uint32_t>(delay.count()));
}

Object& Object::fade_out(uint32_t time, uint32_t delay) {
  if (obj_) lv_obj_fade_out(obj_, time, delay);
  return *this;
}

Object& Object::fade_out(std::chrono::milliseconds time,
                         std::chrono::milliseconds delay) {
  return fade_out(static_cast<uint32_t>(time.count()),
                  static_cast<uint32_t>(delay.count()));
}

Object& Object::fade_to(Opacity opa, uint32_t time, uint32_t delay) {
  if (obj_) {
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj_);
    lv_anim_set_values(&a, lv_obj_get_style_opa(obj_, LV_PART_MAIN),
                       static_cast<lv_opa_t>(opa));
    lv_anim_set_exec_cb(&a, [](void* var, int32_t v) {
      lv_obj_set_style_opa(static_cast<lv_obj_t*>(var), v, LV_PART_MAIN);
    });
    lv_anim_set_duration(&a, time);
    lv_anim_set_delay(&a, delay);
    lv_anim_start(&a);
  }
  return *this;
}

Object& Object::fade_to(Opacity opa, std::chrono::milliseconds time,
                        std::chrono::milliseconds delay) {
  return fade_to(opa, static_cast<uint32_t>(time.count()),
                 static_cast<uint32_t>(delay.count()));
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

Object& Object::add_event_cb(lv_event_code_t event_code,
                             EventCallback callback) {
  if (!obj_) return *this;
  auto node = std::make_unique<CallbackNode>();
  node->event_code = event_code;
  node->callback = std::move(callback);
  lv_obj_add_event_cb(obj_, event_proxy, event_code, node.get());
  callback_nodes_.push_back(std::move(node));
  return *this;
}

void Object::remove_all_event_cbs() {
  if (!obj_) return;
  for (auto& node : callback_nodes_) {
    lv_obj_remove_event_cb_with_user_data(obj_, event_proxy, node.get());
  }
  callback_nodes_.clear();
}

// --- Metadata & ID ---

void Object::set_id(void* id) {
  if (obj_) lv_obj_set_id(obj_, id);
}

void* Object::get_id() const { return obj_ ? lv_obj_get_id(obj_) : nullptr; }

Object Object::find_by_id(const void* id) const {
  if (!obj_)
    return Object(static_cast<lv_obj_t*>(nullptr), Ownership::Unmanaged);
  return Object(lv_obj_find_by_id(obj_, id), Ownership::Unmanaged);
}

SubjectProxy Object::on_subject(Subject& subject) {
  return SubjectProxy(obj_, subject.raw());
}

SubjectProxy Object::on_subject(lv_subject_t* subject) {
  return SubjectProxy(obj_, subject);
}

EventProxy Object::event() { return EventProxy(this); }

StateProxy Object::state() { return StateProxy(obj_); }

GroupProxy Object::group() { return GroupProxy(obj_); }

InteractionProxy Object::interaction() { return InteractionProxy(obj_); }

TreeProxy Object::tree() { return TreeProxy(obj_); }

// --- Styles ---

void Object::add_style(Style& style, lv_style_selector_t selector) {
  if (obj_) lv_obj_add_style(obj_, style.raw(), selector);
}

void Object::remove_style(Style* style, lv_style_selector_t selector) {
  if (obj_) lv_obj_remove_style(obj_, style ? style->raw() : nullptr, selector);
}

// --- Layout & Scroll ---

}  // namespace lvgl
