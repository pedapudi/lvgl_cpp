#ifndef LVGL_CPP_MISC_LAYOUT_H_
#define LVGL_CPP_MISC_LAYOUT_H_

#include <cstdint>
#include <vector>

#include "constants.h"
#include "lvgl.h"

namespace lvgl {

/**
 * @brief Helper for building Grid layouts.
 *
 * This class provides a fluent API to build column and row descriptor arrays
 * for Grid layouts. The resulting descriptors are terminated with
 * LV_GRID_TEMPLATE_LAST automatically.
 *
 * @note The user must ensure the GridLayout object stays alive as long as
 * the LVGL object uses its descriptor arrays, as LVGL stores raw pointers.
 */
class GridLayout {
 public:
  GridLayout() = default;

  /**
   * @brief Add a column to the grid.
   * @param width Width of the column (pixel value, Size::Percent(), or
   * Grid::Template::Fr/Content).
   * @return Reference to this builder.
   */
  GridLayout& add_column(int32_t width) {
    cols_.push_back(width);
    dirty_ = true;
    return *this;
  }

  /**
   * @brief Add a row to the grid.
   * @param height Height of the row (pixel value, Size::Percent(), or
   * Grid::Template::Fr/Content).
   * @return Reference to this builder.
   */
  GridLayout& add_row(int32_t height) {
    rows_.push_back(height);
    dirty_ = true;
    return *this;
  }

  /**
   * @brief Get the column descriptor array.
   * @return Pointer to the terminated array of column sizes.
   */
  const int32_t* col_dsc() const {
    prepare_descriptors();
    return finalized_cols_.data();
  }

  /**
   * @brief Get the row descriptor array.
   * @return Pointer to the terminated array of row sizes.
   */
  const int32_t* row_dsc() const {
    prepare_descriptors();
    return finalized_rows_.data();
  }

 private:
  void prepare_descriptors() const {
    if (dirty_) {
      finalized_cols_ = cols_;
      finalized_cols_.push_back(Grid::Template::Last);

      finalized_rows_ = rows_;
      finalized_rows_.push_back(Grid::Template::Last);

      dirty_ = false;
    }
  }

  std::vector<int32_t> cols_;
  std::vector<int32_t> rows_;

  mutable std::vector<int32_t> finalized_cols_;
  mutable std::vector<int32_t> finalized_rows_;
  mutable bool dirty_ = true;
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_LAYOUT_H_
