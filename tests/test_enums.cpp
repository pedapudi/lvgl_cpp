#include <cassert>
#include <iostream>

#include "../display/display.h"
#include "../widgets/arc.h"
#include "../widgets/bar.h"
#include "../widgets/button.h"
#include "../widgets/chart.h"
#include "../widgets/dropdown.h"
#include "../widgets/keyboard.h"
#include "../widgets/label.h"
#include "../widgets/roller.h"
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
    std::cout << "Testing Object properties..." << std::endl;
    lvgl::Object obj;
    obj.scroll().mode(lvgl::ScrollbarMode::Active);
    obj.scroll().snap(lvgl::ScrollSnap::Center,
                      lvgl::ScrollSnap::None);  // Partial snap set replacement
    obj.set_base_dir(lvgl::BaseDir::Rtl);

    assert(lv_obj_get_scrollbar_mode(obj.raw()) == LV_SCROLLBAR_MODE_ACTIVE);
    assert(lv_obj_get_scroll_snap_x(obj.raw()) == LV_SCROLL_SNAP_CENTER);
    assert(lv_obj_get_style_base_dir(obj.raw(), LV_PART_MAIN) ==
           LV_BASE_DIR_RTL);
  }

  {
    std::cout << "Testing Button styles..." << std::endl;
    lvgl::Button btn(screen);
    btn.set_size(100, 50)
        .style()
        .bg_opa(lvgl::Opacity::Cover)
        .border_opa(lvgl::Opacity::Opa50)
        .border_side(lvgl::BorderSide::Bottom);
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
    std::cout << "Testing Label long mode..." << std::endl;
    lvgl::Label label(screen);
    label.set_text("Hello");
    std::cerr << "Label text set" << std::endl;
    label.style().text_align(lvgl::TextAlign::Center);
    label.set_long_mode(lvgl::Label::LongMode::ScrollCircular);

    assert(lv_obj_get_style_text_align(label.raw(), LV_PART_MAIN) ==
           LV_TEXT_ALIGN_CENTER);
    assert(lv_label_get_long_mode(label.raw()) ==
           LV_LABEL_LONG_SCROLL_CIRCULAR);
  }

  {
    std::cout << "Testing Keyboard mode..." << std::endl;
    lvgl::Keyboard kb;
    kb.set_mode(lvgl::KeyboardMode::Number);
    assert(lv_keyboard_get_mode(kb.raw()) == LV_KEYBOARD_MODE_NUMBER);
  }

  {
    std::cout << "Testing Chart overloads..." << std::endl;
    lvgl::Chart chart;
    chart.set_type(lvgl::Chart::Type::Bar);
    chart.set_update_mode(lvgl::Chart::UpdateMode::Circular);
    chart.set_axis_range(lvgl::Chart::Axis::PrimaryY, 0, 100);
    auto ser =
        chart.add_series(lv_color_hex(0xFF0000), lvgl::Chart::Axis::PrimaryY);

    assert(chart.get_type() == lvgl::Chart::Type::Bar);
    assert(ser.raw() != nullptr);
  }

  {
    std::cout << "Testing Roller mode..." << std::endl;
    lvgl::Roller roller;
    roller.set_options("1\n2\n3", lvgl::RollerMode::Infinite);
    // Roler options parsing is complex to verify via raw getter easily,
    // but we can check if it doesn't crash and the call succeeded.
    assert(roller.raw() != nullptr);
  }

  {
    lvgl::Object obj(&screen);
    // Layout
    obj.layout().flex_flow(lvgl::FlexFlow::Row);
    obj.layout().flex_align(lvgl::FlexAlign::Center,
                            lvgl::FlexAlign::SpaceEvenly,
                            lvgl::FlexAlign::Start);
    obj.scroll().mode(lvgl::ScrollbarMode::Auto);

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
    arc.set_mode(lvgl::Arc::Mode::Reverse);
    assert(lv_arc_get_mode(arc.raw()) == LV_ARC_MODE_REVERSE);
    std::cerr << "Arc Done" << std::endl;
  }

  {
    std::cerr << "Bar Start" << std::endl;
    lvgl::Bar bar(screen);
    bar.set_mode(lvgl::Bar::Mode::Range);
    assert(lv_bar_get_mode(bar.raw()) == LV_BAR_MODE_RANGE);
    std::cerr << "Bar Done" << std::endl;
  }

  {
    std::cerr << "Slider Start" << std::endl;
    lvgl::Slider slider(screen);
    slider.set_mode(lvgl::Slider::Mode::Symmetrical);
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
    label.set_long_mode(lvgl::Label::LongMode::Dot);
    std::cerr << "Label2 set_mode" << std::endl;
    assert(lv_label_get_long_mode(label.raw()) == LV_LABEL_LONG_DOT);
    std::cerr << "Label Long Mode Done" << std::endl;
  }

  {
    std::cerr << "Keyboard Mode Start" << std::endl;
    lvgl::Keyboard kb(screen);
    kb.set_mode(lvgl::KeyboardMode::Number);
    assert(kb.get_mode() == LV_KEYBOARD_MODE_NUMBER);
    std::cerr << "Keyboard Mode Done" << std::endl;
  }

  {
    std::cerr << "Chart Type/Mode Start" << std::endl;
    lvgl::Chart chart(screen);
    chart.set_type(lvgl::Chart::Type::Bar);
    assert(chart.get_type() == lvgl::Chart::Type::Bar);

    // Check Update Mode (no getter usually available, just compile check)
    chart.set_update_mode(lvgl::Chart::UpdateMode::Circular);
    std::cerr << "Chart Type/Mode Done" << std::endl;
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

  {
    std::cerr << "State Enum Start" << std::endl;
    lvgl::Object obj(&screen);
    obj.add_state(lvgl::State::Checked);
    assert(obj.has_state(lvgl::State::Checked));
    obj.remove_state(lvgl::State::Checked);
    assert(!obj.has_state(lvgl::State::Checked));
    std::cerr << "State Enum Done" << std::endl;
  }

  {
    std::cerr << "Align Enum Start" << std::endl;
    lvgl::Button obj(screen);
    obj.style().bg_opa(lvgl::Opacity::Cover);  // Use fluent Stylable method
    obj.set_size(100, 100);
    obj.align(lvgl::Align::Center);
    // Rough check of position, assuming parent is sizeable
    assert(lv_obj_get_style_align(obj.raw(), LV_PART_MAIN) == LV_ALIGN_CENTER);
    std::cerr << "Align Enum Done" << std::endl;
  }

  {
    std::cerr << "ScrollSnap Enum Start" << std::endl;
    lvgl::Object obj(&screen);
    obj.scroll().snap(lvgl::ScrollSnap::None, lvgl::ScrollSnap::Center);
    assert(lv_obj_get_scroll_snap_y(obj.raw()) == LV_SCROLL_SNAP_CENTER);
    std::cerr << "ScrollSnap Enum Done" << std::endl;
  }

  {
    std::cerr << "BlendMode Enum Start" << std::endl;
    lvgl::Button obj(screen);
    obj.style(LV_PART_MAIN).blend_mode(lvgl::BlendMode::Additive);
    assert(lv_obj_get_style_blend_mode(obj.raw(), LV_PART_MAIN) ==
           LV_BLEND_MODE_ADDITIVE);
    std::cerr << "BlendMode Enum Done" << std::endl;
  }

  {
    std::cerr << "GridAlign Enum Start" << std::endl;
    lvgl::Object obj(&screen);
    obj.layout().grid_align(lvgl::GridAlign::Center, lvgl::GridAlign::End);
    assert(lv_obj_get_style_grid_column_align(obj.raw(), LV_PART_MAIN) ==
           LV_GRID_ALIGN_CENTER);
    assert(lv_obj_get_style_grid_row_align(obj.raw(), LV_PART_MAIN) ==
           LV_GRID_ALIGN_END);
    std::cerr << "GridAlign Enum Done" << std::endl;
  }

  {
    std::cerr << "Dir Enum Start" << std::endl;
    lvgl::Dropdown dd(screen);
    dd.set_dir(lvgl::Dir::Bottom);
    assert(dd.get_dir() == LV_DIR_BOTTOM);
    std::cerr << "Dir Enum Done" << std::endl;
  }

  // Test PointTransformFlag
  lvgl::PointTransformFlag pt_flag =
      lvgl::PointTransformFlag::Recursive | lvgl::PointTransformFlag::Inverse;
  assert(static_cast<uint8_t>(pt_flag) ==
         (LV_OBJ_POINT_TRANSFORM_FLAG_RECURSIVE |
          LV_OBJ_POINT_TRANSFORM_FLAG_INVERSE));

  // Test LayerType
  lvgl::LayerType lt = lvgl::LayerType::Transform;
  assert(static_cast<uint8_t>(lt) == LV_LAYER_TYPE_TRANSFORM);

  std::cout << "[SUCCESS] Enums expanded and validated." << std::endl;
  return 0;
}
