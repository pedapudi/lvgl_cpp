#ifndef LVGL_CPP_WIDGETS_TILEVIEW_H_
#define LVGL_CPP_WIDGETS_TILEVIEW_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_TILEVIEW
/**
 * @file tileview.h
 * @brief C++ Wrapper for LVGL TileView Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::TileView widget(lv_screen_active());
 * widget.center();
 * lvgl::Tile tile1 = widget.add_tile(0, 0, LV_DIR_BOTTOM);
 * ```
 */
namespace lvgl {

class Tile : public Widget<Tile> {
 public:
  using Widget::Widget;
};

class TileView : public Widget<TileView> {
 public:
  TileView();
  explicit TileView(Object* parent, Ownership ownership = Ownership::Default);
  explicit TileView(Object& parent);
  explicit TileView(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Add a tile to the TileView.
   * @return A Tile wrapper with Ownership::Unmanaged.
   */
  Tile add_tile(uint8_t col_id, uint8_t row_id, Dir dir);
  [[deprecated("Use add_tile(uint8_t, uint8_t, Dir) instead")]]
  Tile add_tile(uint8_t col_id, uint8_t row_id, lv_dir_t dir);

  TileView& set_tile(lv_obj_t* tile_obj, AnimEnable anim = AnimEnable::On);
  [[deprecated("Use set_tile(lv_obj_t*, AnimEnable) instead")]]
  TileView& set_tile(lv_obj_t* tile_obj, lv_anim_enable_t anim);
  TileView& set_tile(Tile& tile, AnimEnable anim = AnimEnable::On);
  [[deprecated("Use set_tile(Tile&, AnimEnable) instead")]]
  TileView& set_tile(Tile& tile, lv_anim_enable_t anim);

  TileView& set_tile_by_index(uint32_t col_id, uint32_t row_id,
                              AnimEnable anim = AnimEnable::On);
  [[deprecated(
      "Use set_tile_by_index(uint32_t, uint32_t, AnimEnable) instead")]]
  TileView& set_tile_by_index(uint32_t col_id, uint32_t row_id,
                              lv_anim_enable_t anim);

  Tile get_tile_active();
};

}  // namespace lvgl

#endif  // LV_USE_TILEVIEW
#endif  // LVGL_CPP_WIDGETS_TILEVIEW_H_
