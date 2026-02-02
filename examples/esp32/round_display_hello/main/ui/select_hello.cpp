#include "select_hello.h"

#include "esp_log.h"
#include "hello_world.h"

LV_FONT_DECLARE(lv_font_montserrat_14);
LV_FONT_DECLARE(lv_font_montserrat_20);

// Solarized Palette (Full)
static const lvgl::Color BASE03 = lvgl::Color::from_hex(0x002b36);
static const lvgl::Color BASE02 = lvgl::Color::from_hex(0x073642);
static const lvgl::Color BASE01 = lvgl::Color::from_hex(0x586e75);
static const lvgl::Color BASE00 = lvgl::Color::from_hex(0x657b83);
static const lvgl::Color BASE0 = lvgl::Color::from_hex(0x839496);
static const lvgl::Color BASE1 = lvgl::Color::from_hex(0x93a1a1);
static const lvgl::Color BASE2 = lvgl::Color::from_hex(0xeee8d5);
static const lvgl::Color BASE3 = lvgl::Color::from_hex(0xfdf6e3);
static const lvgl::Color YELLOW = lvgl::Color::from_hex(0xb58900);
static const lvgl::Color ORANGE = lvgl::Color::from_hex(0xcb4b16);
static const lvgl::Color RED = lvgl::Color::from_hex(0xdc322f);
static const lvgl::Color MAGENTA = lvgl::Color::from_hex(0xd33682);
static const lvgl::Color VIOLET = lvgl::Color::from_hex(0x6c71c4);
static const lvgl::Color BLUE = lvgl::Color::from_hex(0x268bd2);
static const lvgl::Color CYAN = lvgl::Color::from_hex(0x2aa198);
static const lvgl::Color GREEN = lvgl::Color::from_hex(0x859900);

SelectHello::SelectHello()
    : menu_screen_((lv_obj_t*)nullptr, lvgl::Object::Ownership::Unmanaged),
      hello_screen_((lv_obj_t*)nullptr, lvgl::Object::Ownership::Unmanaged) {}

void SelectHello::show_menu(lvgl::Display& display) {
  display_ = &display;

  // Initialize menu screen if not already done
  if (!menu_screen_.is_valid()) {
    menu_screen_ = lvgl::Object();  // Create a new screen
    menu_screen_.style().bg_color(BASE03).bg_opa(lvgl::Opacity::Cover);

    // Roller
    roller_ = lvgl::Roller(&menu_screen_);
    static const char* options =
        "Hello, world!\nHello, ESP32!\nHello, LVGL C++!";
    roller_.set_options(options, lvgl::Roller::Mode::Infinite);

    roller_.style()
        .text_font(lvgl::Font(&lv_font_montserrat_20))
        .text_color(BASE00)
        .bg_color(BASE2)
        .bg_opa(lvgl::Opacity::Cover)
        .radius(20)
        .border_width(0)
        .outline_width(0);

    roller_.style(lvgl::Part::Selected)
        .text_font(lvgl::Font(&lv_font_montserrat_20))
        .bg_color(BLUE)
        .text_color(BASE3);

    roller_.set_visible_row_count(3);
    roller_.set_width(200);
    roller_.center();

    // Backlight Toggle
    bl_switch_ = lvgl::Switch(&menu_screen_);
    bl_switch_.align(lvgl::Align::TopMid, 0, 10).set_height(50).set_width(125);
    bl_switch_.style().bg_color(BASE2).bg_opa(lvgl::Opacity::Cover);
    bl_switch_
        .style(static_cast<lv_style_selector_t>(lvgl::Part::Indicator) |
               static_cast<lv_style_selector_t>(lvgl::State::Checked))
        .bg_color(BASE2)
        .bg_opa(lvgl::Opacity::Cover);
    bl_switch_.style(lvgl::Part::Knob)
        .bg_color(BASE02)
        .bg_opa(lvgl::Opacity::Cover);
    bl_switch_.add_state(lvgl::State::Checked);  // Default to on
    bl_switch_.add_event_cb(
        lvgl::EventCode::ValueChanged, [this](lvgl::Event& e) {
          if (this->on_backlight_changed_) {
            this->on_backlight_changed_(
                this->bl_switch_.has_state(lvgl::State::Checked));
          }
        });

    // Input Handling
    auto on_select = [this](lvgl::Event& e) {
      if (!this->in_menu_mode_) return;

      bool triggered = false;
      if (e.get_code() == lvgl::EventCode::Clicked) {
        triggered = true;
      } else if (e.get_code() == lvgl::EventCode::Key) {
        if (*e.get_param<lvgl::Key>() == lvgl::Key::Enter) {
          triggered = true;
        }
      }

      if (triggered) {
        this->pending_selection_ = this->roller_.get_selected();
        lvgl::Async::call(
            [this]() { this->load_hello_screen(this->pending_selection_); });
      }
    };

    roller_.add_event_cb(lvgl::EventCode::Clicked, on_select);
    roller_.add_event_cb(lvgl::EventCode::Key, on_select);
  }

  // Ensure navigation works
  lvgl::Group::get_default().remove_all_objs();
  lvgl::Group::get_default().add_obj(roller_);
  lvgl::Group::get_default().add_obj(bl_switch_);

  // Load the menu screen
  in_menu_mode_ = true;
  if (display.get_screen_active() != menu_screen_.raw()) {
    display.load_screen(menu_screen_);
  }
}

void SelectHello::load_hello_screen(int index) {
  const char* text;
  switch (index) {
    case 0:
      text = "Hello, world!";
      break;
    case 1:
      text = "Hello, ESP32!";
      break;
    case 2:
      text = "Hello, LVGL C++!";
      break;
    default:
      text = "Unknown";
      break;
  }

  in_menu_mode_ = false;

  if (display_) {
    // Prep separate hello screen
    if (!hello_screen_.is_valid()) {
      hello_screen_ = lvgl::Object();  // Create new screen
    }

    // Clean whatever was there
    hello_screen_.clean();

    // Load it BEFORE populating so get_screen_active() works in
    // HelloWorld::load? Actually HelloWorld::load takes display and uses
    // get_screen_active(). So we MUST load it first.
    display_->load_screen(hello_screen_);

    // Force immediate update of active screen ptr if needed, but load_screen in
    // C++ wrapper usually calls lv_scr_load.

    // Load HelloWorld widgets onto the now-active hello_screen_
    HelloWorld::load(*display_, text, BASE03, BASE0, index == 0);

    // BACK NAVIGATION: Capture clicks or keys on the screen to return to menu
    // Since hello_screen_ is persistent, we can attach event cb once?
    // No, we clean it every time, so we must re-attach.
    // Wait, cleaning deletes children, but flags and event_cbs on the screen
    // itself persist? Yes on screen object itself. But we want to be safe, so
    // let's check if we already added it? Or just re-add (if not unique).
    // lvgl::Object::add_event_cb adds unique? No. Simple Solution: remove all
    // event cbs first.
    hello_screen_.remove_all_event_cbs();

    hello_screen_.add_flag(lvgl::ObjFlag::Clickable);

    auto on_back = [this](lvgl::Event& e) {
      if (this->in_menu_mode_) return;

      bool trigger = false;
      if (e.get_code() == lvgl::EventCode::Clicked) {
        trigger = true;
      } else if (e.get_code() == lvgl::EventCode::Key) {
        trigger = true;  // Any key goes back
      }

      if (trigger) {
        this->in_menu_mode_ = true;
        lvgl::Async::call([this]() { this->show_menu(*this->display_); });
      }
    };

    hello_screen_.add_event_cb(lvgl::EventCode::Clicked, on_back);
    hello_screen_.add_event_cb(lvgl::EventCode::Key, on_back);

    // Provide a visual hint for back navigation
    lvgl::Label back_hint(&hello_screen_);
    back_hint.set_long_mode(lvgl::Label::LongMode::ScrollCircular)
        .set_width(140)
        .add_flag(lvgl::ObjFlag::Hidden);

    back_hint.set_text("Tap screen to go back")
        .align(lvgl::Align::TopMid, 0, 20);

    back_hint.style()
        .text_font(lvgl::Font(&lv_font_montserrat_14))
        .text_color(BASE01);

    lv_obj_t* hint_obj = back_hint.release();

    // Show hint after some delay
    hint_timer_ = lvgl::Timer::periodic(2000, [hint_obj](lvgl::Timer* t) {
      lvgl::Object(hint_obj).remove_flag(lvgl::ObjFlag::Hidden);
      t->pause();
    });

    // Add screen to group for key navigation
    lvgl::Group::get_default().remove_all_objs();
    lvgl::Group::get_default().add_obj(hello_screen_);
  }
}
