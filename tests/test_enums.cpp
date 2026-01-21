#include <cassert>
#include <iostream>

#include "../display/display.h"
#include "../widgets/arc.h"
#include "../widgets/bar.h"
#include "../widgets/button.h"
#include "../widgets/label.h"
#include "../widgets/slider.h"
#include "lvgl.h"

int main() {
  lv_init();

  // Create a display (required for screens)
  auto disp = lvgl::Display::create(800, 600);

  std::cerr << "Testing Scoped Enums..." << std::endl;

  // Wrap active screen to use as parent
  lvgl::Object screen(lv_screen_active());

  {
    std::cerr << "Block 1 Start" << std::endl;
    lvgl::Button btn(screen);
    std::cerr << "Button created" << std::endl;
    btn.set_size(100, 50)
        .set_bg_opa(lvgl::Opacity::Cover)
        .set_border_opa(lvgl::Opacity::Opa50)
        .set_border_side(lvgl::BorderSide::Bottom);
    std::cerr << "Button styled" << std::endl;

    // Verify
    lv_obj_t* obj = btn.raw();
    assert(lv_obj_get_style_bg_opa(obj, LV_PART_MAIN) == LV_OPA_COVER);
    assert(lv_obj_get_style_border_opa(obj, LV_PART_MAIN) == LV_OPA_50);
    assert(lv_obj_get_style_border_side(obj, LV_PART_MAIN) ==
           LV_BORDER_SIDE_BOTTOM);
    std::cerr << "Block 1 Done" << std::endl;
  }

  {
    std::cerr << "Block 2 Start" << std::endl;
    lvgl::Label label(screen);
    std::cerr << "Label created" << std::endl;
    label.set_text("Hello");
    std::cerr << "Label text set" << std::endl;
    label.set_text_align(lvgl::TextAlign::Center);
    std::cerr << "Label align set" << std::endl;
    label.set_text_opa(lvgl::Opacity::Opa80);
    std::cerr << "Label opa set" << std::endl;

    // Verify
    lv_obj_t* obj = label.raw();
    assert(lv_obj_get_style_text_align(obj, LV_PART_MAIN) ==
           LV_TEXT_ALIGN_CENTER);
    assert(lv_obj_get_style_text_opa(obj, LV_PART_MAIN) == LV_OPA_80);
    std::cerr << "Label Basic Done" << std::endl;
  }

  {
    lvgl::Object obj(&screen);
    // Layout
    obj.set_flex_flow(lvgl::FlexFlow::Row);
    obj.set_flex_align(lvgl::FlexAlign::Center, lvgl::FlexAlign::SpaceEvenly,
                       lvgl::FlexAlign::Start);
    obj.set_scrollbar_mode(lvgl::ScrollbarMode::Auto);

    // Verify Layout
    assert(lv_obj_get_style_flex_flow(obj.raw(), LV_PART_MAIN) ==
           LV_FLEX_FLOW_ROW);
    assert(lv_obj_get_style_flex_main_place(obj.raw(), LV_PART_MAIN) ==
           LV_FLEX_ALIGN_CENTER);
    assert(lv_obj_get_scrollbar_mode(obj.raw()) == LV_SCROLLBAR_MODE_AUTO);
    std::cerr << "Object Layout Done" << std::endl;
  }

  {
    std::cerr << "Arc Start" << std::endl;
    lvgl::Arc arc(screen);
    arc.set_mode(lvgl::ArcMode::Reverse);
    assert(lv_arc_get_mode(arc.raw()) == LV_ARC_MODE_REVERSE);
    std::cerr << "Arc Done" << std::endl;
  }

  {
    std::cerr << "Bar Start" << std::endl;
    lvgl::Bar bar(screen);
    bar.set_mode(lvgl::BarMode::Range);
    assert(lv_bar_get_mode(bar.raw()) == LV_BAR_MODE_RANGE);
    std::cerr << "Bar Done" << std::endl;
  }

  {
    std::cerr << "Slider Start" << std::endl;
    lvgl::Slider slider(screen);
    slider.set_mode(lvgl::SliderMode::Symmetrical);
    // Note: Slider implementation validation depends on internal storage or
    // behavior, checking mode getter if available or just compilation.
    // lv_slider_get_mode doesn't exist in v8/v9 API directly usually, it shares
    // with Bar?
    assert(lv_bar_get_mode(slider.raw()) == LV_BAR_MODE_SYMMETRICAL);
    std::cerr << "Slider Done" << std::endl;
  }

  {
    std::cerr << "Label Long Mode Start" << std::endl;
    lvgl::Label label(screen);
    std::cerr << "Label2 created" << std::endl;
    label.set_long_mode(lvgl::LabelLongMode::Dot);
    std::cerr << "Label2 set_mode" << std::endl;
    assert(lv_label_get_long_mode(label.raw()) == LV_LABEL_LONG_DOT);
    std::cerr << "Label Long Mode Done" << std::endl;
  }

  {
    std::cerr << "Event Start" << std::endl;
    // EventCode compilation check with AddEventCb
    lvgl::Object obj(&screen);
    obj.add_event_cb(lvgl::EventCode::Clicked, [](lvgl::Event& e) { (void)e; });
    // Run an event?
    lv_obj_send_event(obj.raw(), LV_EVENT_CLICKED, nullptr);
    std::cerr << "Event Done" << std::endl;
  }

  std::cerr << "[SUCCESS] Scoped Enums validated." << std::endl;
  return 0;
}
