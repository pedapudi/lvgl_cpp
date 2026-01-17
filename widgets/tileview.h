#ifndef LVGL_CPP_WIDGETS_TILEVIEW_H_
#define LVGL_CPP_WIDGETS_TILEVIEW_H_

#include <cstdint>

#include "../core/object.h"  // IWYU pragma: export
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

class Tile : public Object {
 public:
  using Object::Object;
};

class TileView : public Object {
 public:
  /**
   * @brief Create a TileView on the active screen.
   */
  TileView();
  /**
   * @brief Create a TileView with a parent.
   * @param parent The parent object.
   */
  explicit TileView(Object& parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit TileView(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Tile add_tile(uint8_t col_id, uint8_t row_id, lv_dir_t dir);
  TileView& set_tile(lv_obj_t* tile_obj, lv_anim_enable_t anim_en);
  TileView& set_tile_by_index(uint32_t col_id, uint32_t row_id,
                              lv_anim_enable_t anim_en);

  // Fluent API shadows
  TileView& set_width(int32_t width);
  TileView& set_height(int32_t height);
  TileView& set_size(int32_t width, int32_t height);
  TileView& align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  TileView& add_state(lv_state_t state);
  TileView& remove_state(lv_state_t state);
  TileView& add_flag(lv_obj_flag_t flag);
  TileView& remove_flag(lv_obj_flag_t flag);
  Tile get_tile_active();
};

}  // namespace lvgl

#endif  // LV_USE_TILEVIEW
#endif  // LVGL_CPP_WIDGETS_TILEVIEW_H_
