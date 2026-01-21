#pragma once

#include "../../misc/enums.h"
#include "../layout.h"

namespace lvgl {

template <typename Derived>
class Layoutable {
 public:
  Derived& layout(const Flex& flex) {
    flex.apply(static_cast<Derived*>(this)->raw());
    return *static_cast<Derived*>(this);
  }

  Derived& set_layout(uint32_t layout) {
    lv_obj_set_layout(static_cast<Derived*>(this)->raw(), layout);
    return *static_cast<Derived*>(this);
  }

  bool is_layout_positioned() const {
    return lv_obj_is_layout_positioned(
        static_cast<const Derived*>(this)->raw());
  }

  Derived& mark_layout_as_dirty() {
    lv_obj_mark_layout_as_dirty(static_cast<Derived*>(this)->raw());
    return *static_cast<Derived*>(this);
  }

  Derived& update_layout() {
    lv_obj_update_layout(static_cast<Derived*>(this)->raw());
    return *static_cast<Derived*>(this);
  }

  // Flex Layout
  Derived& set_flex_flow(lv_flex_flow_t flow) {
    lv_obj_set_flex_flow(static_cast<Derived*>(this)->raw(), flow);
    return *static_cast<Derived*>(this);
  }

  Derived& set_flex_flow(FlexFlow flow) {
    return set_flex_flow(static_cast<lv_flex_flow_t>(flow));
  }

  Derived& set_flex_align(lv_flex_align_t main_place,
                          lv_flex_align_t cross_place,
                          lv_flex_align_t track_cross_place) {
    lv_obj_set_flex_align(static_cast<Derived*>(this)->raw(), main_place,
                          cross_place, track_cross_place);
    return *static_cast<Derived*>(this);
  }

  Derived& set_flex_align(FlexAlign main_place, FlexAlign cross_place,
                          FlexAlign track_cross_place) {
    return set_flex_align(static_cast<lv_flex_align_t>(main_place),
                          static_cast<lv_flex_align_t>(cross_place),
                          static_cast<lv_flex_align_t>(track_cross_place));
  }

  Derived& set_flex_grow(uint8_t grow) {
    lv_obj_set_flex_grow(static_cast<Derived*>(this)->raw(), grow);
    return *static_cast<Derived*>(this);
  }

  // Grid Layout
  Derived& set_grid_dsc_array(const int32_t col_dsc[],
                              const int32_t row_dsc[]) {
    lv_obj_set_grid_dsc_array(static_cast<Derived*>(this)->raw(), col_dsc,
                              row_dsc);
    return *static_cast<Derived*>(this);
  }

  Derived& set_grid_align(lv_grid_align_t column_align,
                          lv_grid_align_t row_align) {
    lv_obj_set_grid_align(static_cast<Derived*>(this)->raw(), column_align,
                          row_align);
    return *static_cast<Derived*>(this);
  }

  Derived& set_grid_align(GridAlign column_align, GridAlign row_align) {
    return set_grid_align(static_cast<lv_grid_align_t>(column_align),
                          static_cast<lv_grid_align_t>(row_align));
  }

  Derived& set_grid_cell(lv_grid_align_t column_align, int32_t col_pos,
                         int32_t col_span, lv_grid_align_t row_align,
                         int32_t row_pos, int32_t row_span) {
    lv_obj_set_grid_cell(static_cast<Derived*>(this)->raw(), column_align,
                         col_pos, col_span, row_align, row_pos, row_span);
    return *static_cast<Derived*>(this);
  }

  Derived& set_grid_cell(GridAlign column_align, int32_t col_pos,
                         int32_t col_span, GridAlign row_align, int32_t row_pos,
                         int32_t row_span) {
    return set_grid_cell(static_cast<lv_grid_align_t>(column_align), col_pos,
                         col_span, static_cast<lv_grid_align_t>(row_align),
                         row_pos, row_span);
  }
};

}  // namespace lvgl
