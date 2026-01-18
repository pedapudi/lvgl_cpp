#include "tileview.h"

#if LV_USE_TILEVIEW

namespace lvgl {

TileView::TileView()
    : Widget(lv_tileview_create(nullptr), Ownership::Managed) {}

TileView::TileView(Object& parent, Ownership ownership)
    : Widget(lv_tileview_create(parent.raw()), ownership) {}

TileView::TileView(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

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

Tile TileView::get_tile_active() {
  return Tile(obj_ ? lv_tileview_get_tile_active(obj_) : nullptr);
}

}  // namespace lvgl

#endif  // LV_USE_TILEVIEW
