#pragma once

#include "lvgl.h"

namespace lvgl {

class Flex {
 public:
  static Flex Row() { return Flex().flow(LV_FLEX_FLOW_ROW); }
  static Flex Column() { return Flex().flow(LV_FLEX_FLOW_COLUMN); }
  static Flex RowWrap() { return Flex().flow(LV_FLEX_FLOW_ROW_WRAP); }
  static Flex ColumnWrap() { return Flex().flow(LV_FLEX_FLOW_COLUMN_WRAP); }
  static Flex RowReverse() { return Flex().flow(LV_FLEX_FLOW_ROW_REVERSE); }
  static Flex ColumnReverse() { return Flex().flow(LV_FLEX_FLOW_COLUMN_REVERSE); }
  static Flex RowWrapReverse() { return Flex().flow(LV_FLEX_FLOW_ROW_WRAP_REVERSE); }
  static Flex ColumnWrapReverse() { return Flex().flow(LV_FLEX_FLOW_COLUMN_WRAP_REVERSE); }

  Flex& flow(lv_flex_flow_t f) {
    flow_ = f;
    return *this;
  }

  Flex& gap(int32_t row_gap, int32_t col_gap) {
    row_gap_ = row_gap;
    col_gap_ = col_gap;
    return *this;
  }

  Flex& gap(int32_t g) {
      return gap(g, g);
  }

  Flex& place(lv_flex_align_t main_place, lv_flex_align_t cross_place, lv_flex_align_t track_place) {
      main_place_ = main_place;
      cross_place_ = cross_place;
      track_place_ = track_place;
      return *this;
  }
  
  Flex& center() {
      return place(LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  }

  void apply(lv_obj_t* obj) const {
    lv_obj_set_flex_flow(obj, flow_);
    lv_obj_set_flex_align(obj, main_place_, cross_place_, track_place_);
    lv_obj_set_style_pad_row(obj, row_gap_, 0);
    lv_obj_set_style_pad_column(obj, col_gap_, 0);
    lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
  }

 private:
  lv_flex_flow_t flow_ = LV_FLEX_FLOW_ROW;
  int32_t row_gap_ = 0;
  int32_t col_gap_ = 0;
  lv_flex_align_t main_place_ = LV_FLEX_ALIGN_START;
  lv_flex_align_t cross_place_ = LV_FLEX_ALIGN_START;
  lv_flex_align_t track_place_ = LV_FLEX_ALIGN_START;
};

struct Grid {
    template <size_t C, size_t R>
    static void apply(lv_obj_t* obj, const int32_t (&cols)[C], const int32_t (&rows)[R]) {
        lv_obj_set_grid_dsc_array(obj, cols, rows);
        lv_obj_set_layout(obj, LV_LAYOUT_GRID);
    }
    
    static void set_cell(lv_obj_t* obj, lv_grid_align_t x_align, uint8_t col_pos, uint8_t col_span,
                         lv_grid_align_t y_align, uint8_t row_pos, uint8_t row_span) {
        lv_obj_set_grid_cell(obj, x_align, col_pos, col_span, y_align, row_pos, row_span);
    }
};

} // namespace lvgl
