#ifndef LVGL_CPP_LVGLCPP_H_
#define LVGL_CPP_LVGLCPP_H_

#include "core/group.h"          // IWYU pragma: export
#include "core/object.h"         // IWYU pragma: export
#include "core/observer.h"       // IWYU pragma: export
#include "display/display.h"     // IWYU pragma: export
#include "draw/draw.h"           // IWYU pragma: export
#include "draw/image_decoder.h"  // IWYU pragma: export
#include "font/font.h"           // IWYU pragma: export
#include "indev/input_device.h"  // IWYU pragma: export
#include "misc/animation.h"      // IWYU pragma: export
#include "misc/async.h"          // IWYU pragma: export
#include "misc/color.h"          // IWYU pragma: export
#include "misc/file_system.h"    // IWYU pragma: export
#include "misc/log.h"            // IWYU pragma: export
#include "misc/timer.h"          // IWYU pragma: export
#if LV_USE_ANIMIMG
#include "widgets/anim_image.h"  // IWYU pragma: export
#endif
#if LV_USE_ARC
#include "widgets/arc.h"  // IWYU pragma: export
#endif
#if LV_USE_BAR
#include "widgets/bar.h"  // IWYU pragma: export
#endif
#if LV_USE_BUTTON
#include "widgets/button.h"  // IWYU pragma: export
#endif
#if LV_USE_BUTTONMATRIX
#include "widgets/button_matrix.h"  // IWYU pragma: export
#endif
#if LV_USE_CALENDAR
#include "widgets/calendar.h"  // IWYU pragma: export
#endif
#if LV_USE_CANVAS
#include "widgets/canvas.h"  // IWYU pragma: export
#endif
#if LV_USE_CHART
#include "widgets/chart.h"  // IWYU pragma: export
#endif
#if LV_USE_CHECKBOX
#include "widgets/checkbox.h"  // IWYU pragma: export
#endif
#if LV_USE_DROPDOWN
#include "widgets/dropdown.h"  // IWYU pragma: export
#endif
#if LV_USE_IMAGE
#include "widgets/image.h"  // IWYU pragma: export
#endif
#if LV_USE_IMAGEBUTTON
#include "widgets/image_button.h"  // IWYU pragma: export
#endif
#if LV_USE_KEYBOARD
#include "widgets/keyboard.h"  // IWYU pragma: export
#endif
#if LV_USE_LABEL
#include "widgets/label.h"  // IWYU pragma: export
#endif
#if LV_USE_LED
#include "widgets/led.h"  // IWYU pragma: export
#endif
#if LV_USE_LINE
#include "widgets/line.h"  // IWYU pragma: export
#endif
#if LV_USE_LIST
#include "widgets/list.h"  // IWYU pragma: export
#endif
#if LV_USE_MENU
#include "widgets/menu.h"  // IWYU pragma: export
#endif
#if LV_USE_MSGBOX
#include "widgets/msgbox.h"  // IWYU pragma: export
#endif
#if LV_USE_ROLLER
#include "widgets/roller.h"  // IWYU pragma: export
#endif
#if LV_USE_SCALE
#include "widgets/scale.h"  // IWYU pragma: export
#endif
#if LV_USE_SLIDER
#include "widgets/slider.h"  // IWYU pragma: export
#endif
#if LV_USE_SPAN
#include "widgets/span.h"  // IWYU pragma: export
#endif
#if LV_USE_SPINBOX
#include "widgets/spinbox.h"  // IWYU pragma: export
#endif
#if LV_USE_SPINNER
#include "widgets/spinner.h"  // IWYU pragma: export
#endif
#if LV_USE_SWITCH
#include "widgets/switch.h"  // IWYU pragma: export
#endif
#if LV_USE_TABLE
#include "widgets/table.h"  // IWYU pragma: export
#endif
#if LV_USE_TABVIEW
#include "widgets/tabview.h"  // IWYU pragma: export
#endif
#if LV_USE_TEXTAREA
#include "widgets/textarea.h"  // IWYU pragma: export
#endif
#if LV_USE_TILEVIEW
#include "widgets/tileview.h"  // IWYU pragma: export
#endif
#if LV_USE_WIN
#include "widgets/win.h"  // IWYU pragma: export
#endif

// Expose underlying LVGL headers if needed (optional)
#include "lvgl.h"  // IWYU pragma: export

#endif  // LVGL_CPP_LVGLCPP_H_
