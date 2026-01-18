#pragma once

#include "../layout.h"

namespace lvgl {

template <typename Derived>
class Layoutable {
 public:
  Derived& layout(const Flex& flex) {
    flex.apply(static_cast<Derived*>(this)->raw());
    return *static_cast<Derived*>(this);
  }

  Derived& set_grid_cell(lv_grid_align_t x_align, uint8_t col_pos, uint8_t col_span,
                         lv_grid_align_t y_align, uint8_t row_pos, uint8_t row_span) {
      Grid::set_cell(static_cast<Derived*>(this)->raw(), x_align, col_pos, col_span, y_align, row_pos, row_span);
      return *static_cast<Derived*>(this);
  }
};

} // namespace lvgl
