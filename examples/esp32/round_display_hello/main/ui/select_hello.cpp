#include "select_hello.h"

#include "esp_log.h"
#include "hello_world.h"
#include "widgets/screen.h"

// Declare the font used for the roller and labels
LV_FONT_DECLARE(lv_font_montserrat_20);

/*
 * SOLARIZED PALETTE (Full 16-color set)
 * -------------------------------------
 * Solarized is a professionally designed palette that provides consistent
 * contrast and color relationships across different themes (Light/Dark).
 *
 * For "Solarized Light":
 * - Backgrounds: BASE3, BASE2
 * - Content: BASE00, BASE01
 * - Emphasized: BASE01, BASE02
 *
 * Hex values translated to LVGL RGB colors:
 */
// Darker Base Tones
static const lvgl::Color BASE03 = lvgl::Color::make(0x00, 0x2b, 0x36);
static const lvgl::Color BASE02 = lvgl::Color::make(0x07, 0x36, 0x42);
static const lvgl::Color BASE01 = lvgl::Color::make(0x58, 0x6e, 0x75);
static const lvgl::Color BASE00 = lvgl::Color::make(0x65, 0x7b, 0x83);
// Lighter Base Tones
static const lvgl::Color BASE0  = lvgl::Color::make(0x83, 0x94, 0x96);
static const lvgl::Color BASE1  = lvgl::Color::make(0x93, 0xa1, 0xa1);
static const lvgl::Color BASE2  = lvgl::Color::make(0xee, 0xe8, 0xd5);
static const lvgl::Color BASE3  = lvgl::Color::make(0xfd, 0xf6, 0xe3);
// Accent Colors
static const lvgl::Color YELLOW  = lvgl::Color::make(0xb5, 0x89, 0x00);
static const lvgl::Color ORANGE  = lvgl::Color::make(0xcb, 0x4b, 0x16);
static const lvgl::Color RED     = lvgl::Color::make(0xdc, 0x32, 0x2f);
static const lvgl::Color MAGENTA = lvgl::Color::make(0xd3, 0x36, 0x82);
static const lvgl::Color VIOLET  = lvgl::Color::make(0x6c, 0x71, 0xc4);
static const lvgl::Color BLUE    = lvgl::Color::make(0x26, 0x8b, 0xd2);
static const lvgl::Color CYAN    = lvgl::Color::make(0x2a, 0xa1, 0x98);
static const lvgl::Color GREEN   = lvgl::Color::make(0x85, 0x99, 0x00);

SelectHello::SelectHello() {}

void SelectHello::show_menu(lvgl::Display& display) {
  display_ = &display;
  auto screen = display_->get_screen_active();
  lvgl::Screen scr(screen);

  // Clean the current screen by deleting all its child objects.
  scr.clean();
  scr.set_bg_color(BASE3);  // Base3 background for Solarized Light

  in_menu_mode_ = true;

  // Create the Roller widget
  roller_ = lvgl::Roller(&scr);
  static const char* options = "Hello, world!\nHello, ESP32!\nHello, LVGL C++!";
  roller_.set_options(options, lvgl::RollerMode::Infinite);

  // --- Fix 3: Fonts render poorly ---
  // Apply font to MAIN part so non-selected items use it too
  auto part_main = static_cast<lv_style_selector_t>(lvgl::Part::Main);
  auto part_selected = static_cast<lv_style_selector_t>(lvgl::Part::Selected);
  auto state_focus_key =
      static_cast<lv_style_selector_t>(lvgl::State::FocusKey);
  auto state_focused = static_cast<lv_style_selector_t>(lvgl::State::Focused);
  auto state_pressed = static_cast<lv_style_selector_t>(lvgl::State::Pressed);

  /*
   * Styling the Roller
   * ------------------
   * In LVGL, widgets consist of PARTS (like the Background or the Selected
   * row). We use the 'fluent' C++ API to set multiple properties in a row.
   */
  roller_.style(part_main)
      .text_font(lvgl::Font(&lv_font_montserrat_20))
      .text_color(BASE00)  // Solarized text color
      .bg_color(BASE2)     // Subtle background contrast
      .bg_opa(lvgl::Opacity::Cover);

  // Style for the currently SELECTED item
  roller_.style(part_selected)
      .text_color(BASE3)  // Bright text on dark highlight
      .bg_color(BLUE)     // Solarized Blue for selection
      .text_font(lvgl::Font(&lv_font_montserrat_20));

  // Remove the blue focus outline/border
  roller_.style(state_focus_key).outline_width(0).border_width(0);
  roller_.style(state_focused).outline_width(0).border_width(0);

  // --- Fix 1: Roller hard to scroll ---
  // Increase visible rows to make it easier to grab
  roller_.set_visible_row_count(3);
  roller_.set_width(200);
  roller_.center();

  // Create a Select Button with distinctive style
  select_btn_ = lvgl::Button(&scr);
  select_btn_.align(lvgl::Align::BottomMid, 0, -15);

  // Use C++ API for size and flags
  select_btn_.set_size(110, 40);
  select_btn_.add_flag(lvgl::ObjFlag::Clickable);
  select_btn_.remove_flag(lvgl::ObjFlag::Scrollable);

  /*
   * Styling the Button
   * ------------------
   * We apply different colors for the Default state and the Pressed state
   * to provide visual feedback to the user.
   */
  select_btn_.style(part_main).bg_color(ORANGE);     // Neutral Solarized green
  select_btn_.style(state_pressed).bg_color(YELLOW);  // Feedback when pressed
  select_btn_.style(part_main).text_color(BASE3);

  select_btn_label_ = lvgl::Label(&select_btn_);
  select_btn_label_.set_text("SELECT");
  select_btn_label_.style()
      .text_font(lvgl::Font(&lv_font_montserrat_20))
      .text_color(BASE3);
  select_btn_label_.center();
  // Print coordinates to debug layout
  select_btn_.layout().update();

  // Ensure label doesn't block clicks
  select_btn_label_.remove_flag(lvgl::ObjFlag::Clickable);

  // --- Fix 2: Select button does nothing ---
  // Note: add_event_cb in Widget takes (Callback, Code)
  select_btn_.add_event_cb(
      [this](lvgl::Event& e) {
        ESP_LOGI("SelectHello", "Select button clicked");
        pending_selection_ = roller_.get_selected();
        lvgl::Async::call(
            [this]() { this->load_hello_screen(this->pending_selection_); });
      },
      LV_EVENT_CLICKED);

  // Create a Label at the top to hint at functionality
  hint_label_ = lvgl::Label(&scr);
  hint_label_.set_text("Choose option").align(lvgl::Align::TopMid, 0, 20);
  hint_label_.style()
      .text_font(lvgl::Font(&lv_font_montserrat_20))
      .text_color(BASE01)
      .bg_opa(lvgl::Opacity::Transparent);

  // Input Handling: Add the roller to the group so it can receive key events.
  group_.remove_all_objs();
  group_.add_obj(roller_);

  // Register an event callback for key presses.
  roller_.add_event_cb(
      [this](lvgl::Event& e) {
        uint32_t key = *e.get_param<uint32_t>();
        // Only trigger on Enter key. Touch selection is handled by the Button.
        if (key == static_cast<uint32_t>(lvgl::Key::Enter)) {
          ESP_LOGI("SelectHello", "ENTER pressed via Key");
          pending_selection_ = roller_.get_selected();
          lvgl::Async::call(
              [this]() { this->load_hello_screen(this->pending_selection_); });
        }
      },
      LV_EVENT_CLICKED);
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
  ESP_LOGI("SelectHello", "Loading hello screen: %s", text);

  if (display_) {
    active_screen_ = lvgl::Object(display_->get_screen_active(),
                                  lvgl::Object::Ownership::Unmanaged);
    active_screen_.clean();

    HelloWorld::load(*display_, text, BASE03, BASE0);

    lvgl::Label back_hint(&active_screen_);
    back_hint.set_text("Tap to go back")
        .align(lvgl::Align::TopMid, 0, 40)
        .add_flag(lvgl::ObjFlag::Hidden);
    back_hint.style()
        .text_font(lvgl::Font(&lv_font_montserrat_20))
        .text_color(BASE01);

    lv_obj_t* hint_obj = back_hint.release();

    lvgl::Timer::oneshot(2100, [hint_obj]() {
      lvgl::Object(hint_obj).remove_flag(lvgl::ObjFlag::Hidden);
    });

    active_screen_.add_flag(lvgl::ObjFlag::Clickable);
    group_.remove_all_objs();
    group_.add_obj(active_screen_);

    active_screen_.add_event_cb(
        lvgl::EventCode::Clicked, [this](lvgl::Event& e) {
          if (in_menu_mode_) return;
          ESP_LOGI("SelectHello", "Back click received");
          in_menu_mode_ = true;
          lvgl::Async::call([this]() {
            ESP_LOGI("SelectHello", "Returning to menu");
            this->show_menu(*this->display_);
          });
        });
  }
}
