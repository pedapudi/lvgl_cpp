#ifndef LVGL_CPP_WIDGETS_TILEVIEW_H_
#define LVGL_CPP_WIDGETS_TILEVIEW_H_

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

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
  explicit TileView(Object* parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit TileView(lv_obj_t* obj);

  Tile add_tile(uint8_t col_id, uint8_t row_id, lv_dir_t dir);
  void set_tile(lv_obj_t* tile_obj, lv_anim_enable_t anim_en);
  void set_tile_by_index(uint32_t col_id, uint32_t row_id,
                         lv_anim_enable_t anim_en);
  Tile get_tile_active();
};

}  // namespace lvgl

#endif  // LVGL_CPP_WIDGETS_TILEVIEW_H_
