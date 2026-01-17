#include "tileview.h"

#if LV_USE_TILEVIEW

namespace lvgl {

TileView::TileView()
    : Object(lv_tileview_create(nullptr), Ownership::Managed) {}

TileView::TileView(Object& parent, Ownership ownership)
    : Object(lv_tileview_create(parent.raw()), ownership) {}

TileView::TileView(lv_obj_t* obj, Ownership ownership)
    : Object(obj, ownership) {}

Tile TileView::add_tile(uint8_t col_id, uint8_t row_id, lv_dir_t dir) {
  return Tile(obj_ ? lv_tileview_add_tile(obj_, col_id, row_id, dir) : nullptr);
}

TileView& TileView::set_tile(lv_obj_t* tile_obj, lv_anim_enable_t anim_en) {
  if (obj_) lv_tileview_set_tile(obj_, tile_obj, anim_en);
  return *this;
}

TileView& TileView::set_tile_by_index(uint32_t col_id, uint32_t row_id,
                                      lv_anim_enable_t anim_en) {
  if (obj_) lv_tileview_set_tile_by_index(obj_, col_id, row_id, anim_en);
  return *this;
}

TileView& TileView::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
TileView& TileView::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
TileView& TileView::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
TileView& TileView::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
TileView& TileView::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
TileView& TileView::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
TileView& TileView::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
TileView& TileView::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

Tile TileView::get_tile_active() {
  return Tile(obj_ ? lv_tileview_get_tile_active(obj_) : nullptr);
}

}  // namespace lvgl

#endif  // LV_USE_TILEVIEW
