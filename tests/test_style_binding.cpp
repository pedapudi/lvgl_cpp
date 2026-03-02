#include <lvgl.h>

#include <iostream>

#include "lvgl_cpp/core/observer.h"
#include "lvgl_cpp/display/display.h"
#include "lvgl_cpp/widgets/label.h"

using namespace lvgl;

void test_bind_style_prop() {
#if LV_USE_OBSERVER
  lvgl::Display display = lvgl::Display::create(800, 480);
  Label lbl;
  IntSubject subject(255);
  lbl.bind_style_prop(LV_STYLE_TEXT_OPA, subject.raw());

  subject.set(128);
#endif
}

void test_style_proxy_bind() {
#if LV_USE_OBSERVER
  Label lbl;
  IntSubject subject(0xff0000);
  lbl.style().bind(LV_STYLE_TEXT_COLOR, subject.raw());
#endif
}

int main() {
  lv_init();
  test_bind_style_prop();
  test_style_proxy_bind();
  std::cout << "Style binding tests passed!\n";
  return 0;
}
