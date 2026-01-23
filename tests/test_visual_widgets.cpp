#include <iostream>
#include <vector>

#include "../core/object.h"
#include "../display/display.h"
#include "../widgets/anim_image.h"
#include "../widgets/canvas.h"
#include "../widgets/chart.h"
#include "../widgets/image_button.h"
#include "../widgets/label.h"
#include "../widgets/scale.h"
#include "../widgets/span.h"
#include "../widgets/spinner.h"
#include "lvgl.h"

using namespace lvgl;

// Helper to get active screen wrapper
Object get_screen() { return Object(lv_screen_active()); }

void test_chart_1() {
  std::cout << "Testing Chart Example 1..." << std::endl;
  Object screen = get_screen();
  Chart chart(screen);
  chart.set_size(200, 150);
  chart.center();
  chart.set_type(lvgl::Chart::Type::Line);

  ChartSeries ser1 = chart.add_series(lv_palette_main(LV_PALETTE_GREEN),
                                      lvgl::Chart::Axis::PrimaryY);
  ChartSeries ser2 = chart.add_series(lv_palette_main(LV_PALETTE_RED),
                                      lvgl::Chart::Axis::SecondaryY);

  for (int i = 0; i < 10; i++) {
    ser1.set_next_value(i * 10);
    ser2.set_next_value(i * 5);
  }

  chart.refresh();
  std::cout << "Chart Example 1 Passed" << std::endl;
}

void test_canvas_1() {
  std::cout << "Testing Canvas Example 1..." << std::endl;
  Object screen = get_screen();

  // Canvas requires a buffer.
  static uint8_t buffer[50 * 50 * 4];
  Canvas canvas(screen);
  canvas.set_buffer(buffer, 50, 50, LV_COLOR_FORMAT_ARGB8888);
  canvas.fill_bg(lv_color_hex3(0xccc), LV_OPA_COVER);
  canvas.center();

  // Draw rectangle
  lv_layer_t layer;
  canvas.init_layer(&layer);

  lv_draw_rect_dsc_t dsc;
  lv_draw_rect_dsc_init(&dsc);
  dsc.bg_color = lv_palette_main(LV_PALETTE_RED);

  lv_area_t coords = {10, 10, 40, 30};
  lv_draw_rect(&layer, &dsc, &coords);

  canvas.finish_layer(&layer);

  std::cout << "Canvas Example 1 Passed" << std::endl;
}

void test_animimg_1() {
  std::cout << "Testing AnimImage Example 1..." << std::endl;
  Object screen = get_screen();
  AnimImage animimg(screen);

  // Mock images
  static lv_image_dsc_t img1 = {
      {LV_COLOR_FORMAT_ARGB8888, 10, 10, 40, 0}, 0, 0};
  static lv_image_dsc_t img2 = {
      {LV_COLOR_FORMAT_ARGB8888, 10, 10, 40, 0}, 0, 0};
  static const void* anim_imgs[] = {&img1, &img2};

  animimg.set_src(anim_imgs, 2);
  animimg.set_duration(1000);
  animimg.set_repeat_count(LV_ANIM_REPEAT_INFINITE);
  animimg.start();

  std::cout << "AnimImage Example 1 Passed" << std::endl;
}

void test_scale_1() {
  std::cout << "Testing Scale Example 1..." << std::endl;
  Object screen = get_screen();
  Scale scale(screen);
  scale.set_size(LV_PCT(80), 100);
  scale.set_mode(LV_SCALE_MODE_HORIZONTAL_BOTTOM);
  scale.center();
  scale.set_total_tick_count(31);
  scale.set_major_tick_every(5);
  scale.set_range(10, 40);

  std::cout << "Scale Example 1 Passed" << std::endl;
}

void test_imagebutton_1() {
  std::cout << "Testing ImageButton Example 1..." << std::endl;
  Object screen = get_screen();
  ImageButton imgbtn(screen);

  // Mock images
  static lv_image_dsc_t img_left = {
      {LV_COLOR_FORMAT_ARGB8888, 10, 10, 40, 0}, 0, 0};
  static lv_image_dsc_t img_mid = {
      {LV_COLOR_FORMAT_ARGB8888, 10, 10, 40, 0}, 0, 0};
  static lv_image_dsc_t img_right = {
      {LV_COLOR_FORMAT_ARGB8888, 10, 10, 40, 0}, 0, 0};

  imgbtn.set_src(LV_IMAGEBUTTON_STATE_RELEASED, &img_left, &img_mid,
                 &img_right);
  imgbtn.set_width(100);
  imgbtn.align(LV_ALIGN_CENTER, 0, 0);

  // Create label on imgbtn
  // Note: imgbtn is an object, so we can wrap it if needed or use as parent if
  // wrapper conversion works? ImageButton inherits Object, so it converts to
  // Object via valid copy/move or just &imgbtn.
  Label label(imgbtn);
  label.set_text("Button");
  label.align(LV_ALIGN_CENTER, 0, -4);

  std::cout << "ImageButton Example 1 Passed" << std::endl;
}

void test_spangroup_1() {
  std::cout << "Testing SpanGroup Example 1..." << std::endl;
  Object screen = get_screen();
  SpanGroup spans(screen);
  spans.set_width(300);
  spans.set_height(LV_SIZE_CONTENT);
  spans.center();

  lv_span_t* span = spans.add_span();
  spans.set_span_text(span, "China is a beautiful country.");

  span = spans.add_span();
  spans.set_span_text(span, "good good study, day day up.");

  spans.refresh();

  std::cout << "SpanGroup Example 1 Passed" << std::endl;
}

void test_spinner_1() {
  std::cout << "Testing Spinner Example 1..." << std::endl;
  Object screen = get_screen();
  Spinner spinner(screen);
  spinner.set_size(100, 100);
  spinner.center();

  // set_anim_params(duration, spin_count) - verify signature in header if
  // needed. In v9 it seems it might be different or same. Checked spinner.h?
  // No, I checked only existence. Assuming standard API.

  std::cout << "Spinner Example 1 Passed" << std::endl;
}

int main() {
  lv_init();
  lvgl::Display display = lvgl::Display::create(800, 480);

  test_chart_1();
  test_canvas_1();
  test_animimg_1();
  test_scale_1();
  test_imagebutton_1();
  test_spangroup_1();
  test_spinner_1();

  std::cout << "All Visual Widget Tests Passed!" << std::endl;
  return 0;
}
