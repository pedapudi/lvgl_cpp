#include "tileview.h"

#if LV_USE_TILEVIEW

namespace lvgl {

TileView::TileView()
    : TileView(static_cast<Object*>(nullptr), Ownership::Managed) {}

TileView::TileView(Object* parent, Ownership ownership)
    : Widget(lv_tileview_create(parent ? parent->raw() : nullptr), ownership) {}

TileView::TileView(Object& parent) : TileView(&parent) {}

TileView::TileView(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

Tile TileView::add_tile(uint8_t col_id, uint8_t row_id, Dir dir) {
  return add_tile(col_id, row_id, static_cast<lv_dir_t>(dir));
}

Tile TileView::add_tile(uint8_t col_id, uint8_t row_id, lv_dir_t dir) {
  return Tile(obj_ ? lv_tileview_add_tile(obj_, col_id, row_id, dir) : nullptr,
              Ownership::Unmanaged);
}

TileView& TileView::set_tile(lv_obj_t* tile_obj, AnimEnable anim) {
  return set_tile(tile_obj, static_cast<lv_anim_enable_t>(anim));
}

TileView& TileView::set_tile(lv_obj_t* tile_obj, lv_anim_enable_t anim) {
  if (obj_) lv_tileview_set_tile(obj_, tile_obj, anim);
  return *this;
}

TileView& TileView::set_tile(Tile& tile, AnimEnable anim) {
  return set_tile(tile.raw(), anim);
}

TileView& TileView::set_tile(Tile& tile, lv_anim_enable_t anim) {
  return set_tile(tile.raw(), anim);
}

TileView& TileView::set_tile_by_index(uint32_t col_id, uint32_t row_id,
                                      AnimEnable anim) {
  return set_tile_by_index(col_id, row_id, static_cast<lv_anim_enable_t>(anim));
}

TileView& TileView::set_tile_by_index(uint32_t col_id, uint32_t row_id,
                                      lv_anim_enable_t anim) {
  if (obj_) lv_tileview_set_tile_by_index(obj_, col_id, row_id, anim);
  return *this;
}

Tile TileView::get_tile_active() {
  return Tile(obj_ ? lv_tileview_get_tile_active(obj_) : nullptr);
}

}  // namespace lvgl

#endif  // LV_USE_TILEVIEW
