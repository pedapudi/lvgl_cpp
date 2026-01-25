#include "select_hello.h"

#include "esp_log.h"
#include "hello_world.h"

// Declare the font used for the roller and labels
LV_FONT_DECLARE(lv_font_unscii_8);

SelectHello::SelectHello() {}

void SelectHello::show_menu(lvgl::Display& display) {
  display_ = &display;
  auto screen = display_->get_screen_active();
  lvgl::Object scr(screen);

  // Clean the current screen by deleting all its child objects.
  // This is a standard way to "clear" a screen before building a new UI.
  // Note: This invalidates any previous C++ widgets (like roller_) that were
  // children of this screen.

  // Cancel any pending hint timer to prevent accessing deleted objects.
  hint_timer_ = lvgl::Timer();

  scr.clean();

  in_menu_mode_ = true;

  // Create the Roller widget
  roller_ = lvgl::Roller(&scr);
  static const char* options = "Hello World\nHello ESP32\nHello LVGL C++";
  roller_.set_options(options, lvgl::RollerMode::Infinite);

  // Use the small 8px font for the 128x64 SSD1306 display.
  // We set it for both the normal part and the selected part.
  roller_.style().text_font(lvgl::Font(&lv_font_unscii_8));
  roller_.style(lvgl::Part::Selected).text_font(lvgl::Font(&lv_font_unscii_8));

  // Set the background opacity to cover (100%) to ensure it's not transparent.
  roller_.style().bg_opa(lvgl::Opacity::Cover);

  // Remove the border and outline that LVGL automatically adds when a widget
  // is focused via keypad/encoder. This results in a cleaner, border-less UI.
  roller_.style().border_width(0).outline_width(0);
  roller_.style(lvgl::State::FocusKey).border_width(0).outline_width(0);

  roller_.set_visible_row_count(2);
  roller_.set_width(120);
  roller_.align(lvgl::Align::TopMid, 0, 0);

  // Create a Label at the bottom to hint at navigation buttons
  hint_label_ = lvgl::Label(&scr);
  hint_label_.set_text("Next     Select");
  hint_label_.style().text_font(lvgl::Font(&lv_font_unscii_8));
  hint_label_.align(lvgl::Align::BottomMid, 0, -2);

  // Input Handling: Add the roller to the group so it can receive key events.
  group_.remove_all_objs();
  group_.add_obj(roller_);

  // Register an event callback for key presses.
  // We only care about ENTER here because the Roller widget
  // natively handles LV_KEY_UP/DOWN to scroll through options.
  roller_.add_event_cb(
      [this](lvgl::Event& e) {
        lvgl::Key key = *e.get_param<lvgl::Key>();
        if (key == lvgl::Key::Enter) {
          ESP_LOGI("SelectHello", "ENTER pressed, selection: %lu",
                   (unsigned long)roller_.get_selected());

          // Step 1: Store the selection immediately.
          pending_selection_ = roller_.get_selected();

          // Step 2: Use lvgl::Async::call to defer the screen transition.
          lvgl::Async::call(
              [this]() { this->load_hello_screen(this->pending_selection_); });
        }
      },
      static_cast<lv_event_code_t>(lvgl::EventCode::Key));
}

void SelectHello::load_hello_screen(int index) {
  const char* text;
  switch (index) {
    case 0:
      text = "Hello World";
      break;
    case 1:
      text = "Hello ESP32";
      break;
    case 2:
      text = "Hello LVGL C++";
      break;
    default:
      text = "Unknown";
      break;
  }

  in_menu_mode_ = false;
  ESP_LOGI("SelectHello", "Loading hello screen: %s", text);

  if (display_) {
    // CRITICAL: We wrap the active screen in a member variable
    // (active_screen_).
    active_screen_ = lvgl::Object(display_->get_screen_active());

    // Clean the screen for the new UI.
    active_screen_.clean();

    // Load the HelloWorld UI (it creates its own widgets and animations).
    HelloWorld::load(*display_, text);

    // Create a "Back" hint at the top of the screen.
    lvgl::Label back_hint(&active_screen_);
    back_hint.set_text("Press any key to go back");
    back_hint.style().text_font(lvgl::Font(&lv_font_unscii_8));
    back_hint.align(lvgl::Align::TopMid, 0, 2)
        .add_flag(lvgl::ObjFlag::Hidden)
        .set_width(display_->get_horizontal_resolution());

    back_hint.style().text_font(lvgl::Font(&lv_font_unscii_8));
    back_hint.set_long_mode(lvgl::Label::LongMode::ScrollCircular);

    // Release the C++ wrapper's ownership of the label.
    lv_obj_t* hint_obj = back_hint.release();

    // Show the hint text ONLY after the 2-second animation finishes.
    // Use a member timer so it can be cancelled if the user navigates back.
    hint_timer_ = lvgl::Timer::periodic(2100, [this, hint_obj](lvgl::Timer*) {
      lvgl::Object(hint_obj).remove_flag(lvgl::ObjFlag::Hidden);
      hint_timer_.pause();  // Stop after one execution
    });

    // To capture "Back" key presses, we make the screen itself focusable.
    active_screen_.add_flag(lvgl::ObjFlag::Clickable);
    group_.remove_all_objs();
    group_.add_obj(active_screen_);

    // Register the "Back" handler on the screen object.
    active_screen_.add_event_cb(lvgl::EventCode::Key, [this](lvgl::Event& e) {
      if (in_menu_mode_) return;  // Guard against double-clicks

      ESP_LOGI("SelectHello", "Back key received");
      in_menu_mode_ = true;

      // Again, use async call for a safe transition back to the menu.
      lvgl::Async::call([this]() {
        ESP_LOGI("SelectHello", "Returning to menu");
        this->show_menu(*this->display_);
      });
    });
  }
}
