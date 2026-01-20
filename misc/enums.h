#ifndef LVGL_CPP_MISC_ENUMS_H_
#define LVGL_CPP_MISC_ENUMS_H_

#include "lvgl.h"

namespace lvgl {

/**
 * @brief Wrapper for lv_align_t.
 */
enum class Align : uint8_t {
  Default = LV_ALIGN_DEFAULT,
  TopLeft = LV_ALIGN_TOP_LEFT,
  TopMid = LV_ALIGN_TOP_MID,
  TopRight = LV_ALIGN_TOP_RIGHT,
  BottomLeft = LV_ALIGN_BOTTOM_LEFT,
  BottomMid = LV_ALIGN_BOTTOM_MID,
  BottomRight = LV_ALIGN_BOTTOM_RIGHT,
  LeftMid = LV_ALIGN_LEFT_MID,
  RightMid = LV_ALIGN_RIGHT_MID,
  Center = LV_ALIGN_CENTER,
  OutTopLeft = LV_ALIGN_OUT_TOP_LEFT,
  OutTopMid = LV_ALIGN_OUT_TOP_MID,
  OutTopRight = LV_ALIGN_OUT_TOP_RIGHT,
  OutBottomLeft = LV_ALIGN_OUT_BOTTOM_LEFT,
  OutBottomMid = LV_ALIGN_OUT_BOTTOM_MID,
  OutBottomRight = LV_ALIGN_OUT_BOTTOM_RIGHT,
  OutLeftTop = LV_ALIGN_OUT_LEFT_TOP,
  OutLeftMid = LV_ALIGN_OUT_LEFT_MID,
  OutLeftBottom = LV_ALIGN_OUT_LEFT_BOTTOM,
  OutRightTop = LV_ALIGN_OUT_RIGHT_TOP,
  OutRightMid = LV_ALIGN_OUT_RIGHT_MID,
  OutRightBottom = LV_ALIGN_OUT_RIGHT_BOTTOM,
};

/**
 * @brief Wrapper for lv_dir_t.
 */
enum class Dir : uint8_t {
  None = LV_DIR_NONE,
  Left = LV_DIR_LEFT,
  Right = LV_DIR_RIGHT,
  Top = LV_DIR_TOP,
  Bottom = LV_DIR_BOTTOM,
  Hor = LV_DIR_HOR,
  Ver = LV_DIR_VER,
  All = LV_DIR_ALL,
};

/**
 * @brief Wrapper for lv_part_t.
 */
enum class Part : uint32_t {
  Main = LV_PART_MAIN,
  Scrollbar = LV_PART_SCROLLBAR,
  Indicator = LV_PART_INDICATOR,
  Knob = LV_PART_KNOB,
  Selected = LV_PART_SELECTED,
  Items = LV_PART_ITEMS,
  Cursor = LV_PART_CURSOR,
  CustomFirst = LV_PART_CUSTOM_FIRST,
  Any = LV_PART_ANY,
};

/**
 * @brief Wrapper for lv_state_t.
 */
enum class State : uint16_t {
  Default = LV_STATE_DEFAULT,
  Checked = LV_STATE_CHECKED,
  Focused = LV_STATE_FOCUSED,
  FocusKey = LV_STATE_FOCUS_KEY,
  Edited = LV_STATE_EDITED,
  Hovered = LV_STATE_HOVERED,
  Pressed = LV_STATE_PRESSED,
  Scrolled = LV_STATE_SCROLLED,
  Disabled = LV_STATE_DISABLED,
  User1 = LV_STATE_USER_1,
  User2 = LV_STATE_USER_2,
  User3 = LV_STATE_USER_3,
  User4 = LV_STATE_USER_4,
  Any = LV_STATE_ANY,
};

inline State operator|(State lhs, State rhs) {
  return static_cast<State>(static_cast<lv_state_t>(lhs) |
                            static_cast<lv_state_t>(rhs));
}

inline State operator&(State lhs, State rhs) {
  return static_cast<State>(static_cast<lv_state_t>(lhs) &
                            static_cast<lv_state_t>(rhs));
}

inline State operator~(State rhs) {
  return static_cast<State>(~static_cast<lv_state_t>(rhs));
}

inline State& operator|=(State& lhs, State rhs) {
  lhs = lhs | rhs;
  return lhs;
}

inline State& operator&=(State& lhs, State rhs) {
  lhs = lhs & rhs;
  return lhs;
}

/**
 * @brief Wrapper for lv_opa_t.
 */
enum class Opacity : lv_opa_t {
  Transparent = LV_OPA_TRANSP,
  Opa0 = LV_OPA_0,
  Opa10 = LV_OPA_10,
  Opa20 = LV_OPA_20,
  Opa30 = LV_OPA_30,
  Opa40 = LV_OPA_40,
  Opa50 = LV_OPA_50,
  Opa60 = LV_OPA_60,
  Opa70 = LV_OPA_70,
  Opa80 = LV_OPA_80,
  Opa90 = LV_OPA_90,
  Opa100 = LV_OPA_100,
  Cover = LV_OPA_COVER,
};

/**
 * @brief Wrapper for lv_text_align_t.
 */
enum class TextAlign : uint8_t {
  Auto = LV_TEXT_ALIGN_AUTO,
  Left = LV_TEXT_ALIGN_LEFT,
  Center = LV_TEXT_ALIGN_CENTER,
  Right = LV_TEXT_ALIGN_RIGHT,
};

/**
 * @brief Wrapper for lv_border_side_t.
 */
enum class BorderSide : uint8_t {
  None = LV_BORDER_SIDE_NONE,
  Bottom = LV_BORDER_SIDE_BOTTOM,
  Top = LV_BORDER_SIDE_TOP,
  Left = LV_BORDER_SIDE_LEFT,
  Right = LV_BORDER_SIDE_RIGHT,
  Full = LV_BORDER_SIDE_FULL,
  Internal = LV_BORDER_SIDE_INTERNAL,
};

/**
 * @brief Wrapper for lv_blend_mode_t.
 */
enum class BlendMode : uint8_t {
  Normal = LV_BLEND_MODE_NORMAL,
  Additive = LV_BLEND_MODE_ADDITIVE,
  Subtractive = LV_BLEND_MODE_SUBTRACTIVE,
  Multiply = LV_BLEND_MODE_MULTIPLY,
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_ENUMS_H_
