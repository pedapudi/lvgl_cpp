#pragma once

#include "lvgl.h"

namespace lvgl {

class Flex {
 public:
  static Flex Row() { return Flex().flow(FlexFlow::Row); }
  static Flex Column() { return Flex().flow(FlexFlow::Column); }
  static Flex RowWrap() { return Flex().flow(FlexFlow::RowWrap); }
  static Flex ColumnWrap() { return Flex().flow(FlexFlow::ColumnWrap); }
  static Flex RowReverse() { return Flex().flow(FlexFlow::RowReverse); }
  static Flex ColumnReverse() { return Flex().flow(FlexFlow::ColumnReverse); }
  static Flex RowWrapReverse() { return Flex().flow(FlexFlow::RowWrapReverse); }
  static Flex ColumnWrapReverse() {
    return Flex().flow(FlexFlow::ColumnWrapReverse);
  }

  Flex& flow(FlexFlow f) {
    flow_ = f;
    return *this;
  }

  Flex& gap(int32_t row_gap, int32_t col_gap) {
    row_gap_ = row_gap;
    col_gap_ = col_gap;
    return *this;
  }

  Flex& gap(int32_t g) { return gap(g, g); }

  Flex& place(FlexAlign main_place, FlexAlign cross_place,
              FlexAlign track_place) {
    main_place_ = main_place;
    cross_place_ = cross_place;
    track_place_ = track_place;
    return *this;
  }

  Flex& center() {
    return place(FlexAlign::Center, FlexAlign::Center, FlexAlign::Center);
  }

  void apply(lv_obj_t* obj) const {
    lv_obj_set_flex_flow(obj, static_cast<lv_flex_flow_t>(flow_));
    lv_obj_set_flex_align(obj, static_cast<lv_flex_align_t>(main_place_),
                          static_cast<lv_flex_align_t>(cross_place_),
                          static_cast<lv_flex_align_t>(track_place_));
    lv_obj_set_style_pad_row(obj, row_gap_, 0);
    lv_obj_set_style_pad_column(obj, col_gap_, 0);
    lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
  }

 private:
  FlexFlow flow_ = FlexFlow::Row;
  int32_t row_gap_ = 0;
  int32_t col_gap_ = 0;
  FlexAlign main_place_ = FlexAlign::Start;
  FlexAlign cross_place_ = FlexAlign::Start;
  FlexAlign track_place_ = FlexAlign::Start;
};

struct Grid {
  template <size_t C, size_t R>
  static void apply(lv_obj_t* obj, const int32_t (&cols)[C],
                    const int32_t (&rows)[R]) {
    lv_obj_set_grid_dsc_array(obj, cols, rows);
    lv_obj_set_layout(obj, LV_LAYOUT_GRID);
  }

  static void set_cell(lv_obj_t* obj, GridAlign x_align, uint8_t col_pos,
                       uint8_t col_span, GridAlign y_align, uint8_t row_pos,
                       uint8_t row_span) {
    lv_obj_set_grid_cell(obj, static_cast<lv_grid_align_t>(x_align), col_pos,
                         col_span, static_cast<lv_grid_align_t>(y_align),
                         row_pos, row_span);
  }
};

}  // namespace lvgl
