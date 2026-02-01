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
  return Tile(raw() ? lv_tileview_add_tile(raw(), col_id, row_id,
                                           static_cast<lv_dir_t>(dir))
                    : nullptr,
              Ownership::Unmanaged);
}

TileView& TileView::set_tile(lv_obj_t* tile_obj, AnimEnable anim) {
  if (raw())
    lv_tileview_set_tile(raw(), tile_obj, static_cast<lv_anim_enable_t>(anim));
  return *this;
}

TileView& TileView::set_tile(Tile& tile, AnimEnable anim) {
  return set_tile(tile.raw(), anim);
}

TileView& TileView::set_tile_by_index(uint32_t col_id, uint32_t row_id,
                                      AnimEnable anim) {
  if (raw())
    lv_tileview_set_tile_by_index(raw(), col_id, row_id,
                                  static_cast<lv_anim_enable_t>(anim));
  return *this;
}

Tile TileView::get_tile_active() {
  return Tile(raw() ? lv_tileview_get_tile_active(raw()) : nullptr);
}

}  // namespace lvgl

#endif  // LV_USE_TILEVIEW
