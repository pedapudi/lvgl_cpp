#include <cassert>
#include <iostream>

#include "../draw/draw_task.h"

using namespace lvgl::draw;

void test_enum_mapping() {
  assert(static_cast<int>(DrawTaskType::Fill) == LV_DRAW_TASK_TYPE_FILL);
  assert(static_cast<int>(DrawTaskType::Image) == LV_DRAW_TASK_TYPE_IMAGE);
  assert(static_cast<int>(DrawTaskType::Label) == LV_DRAW_TASK_TYPE_LABEL);

#if LV_USE_VECTOR_GRAPHIC
  assert(static_cast<int>(DrawTaskType::Vector) == LV_DRAW_TASK_TYPE_VECTOR);
#endif

  std::cout << "DrawTaskType enum mapping verified." << std::endl;
}

int main() {
  test_enum_mapping();
  std::cout << "All tests passed for DrawTaskType." << std::endl;
  return 0;
}
