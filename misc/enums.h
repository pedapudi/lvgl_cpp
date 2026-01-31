#ifndef LVGL_CPP_MISC_ENUMS_H_
#define LVGL_CPP_MISC_ENUMS_H_

#include <cstdint>

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

// Masking operators for Part (Part | Part, etc.)
inline Part operator|(Part lhs, Part rhs) {
  return static_cast<Part>(static_cast<uint32_t>(lhs) |
                           static_cast<uint32_t>(rhs));
}

inline Part operator&(Part lhs, Part rhs) {
  return static_cast<Part>(static_cast<uint32_t>(lhs) &
                           static_cast<uint32_t>(rhs));
}

inline Part operator~(Part rhs) {
  return static_cast<Part>(~static_cast<uint32_t>(rhs));
}

inline Part& operator|=(Part& lhs, Part rhs) {
  lhs = lhs | rhs;
  return lhs;
}

inline Part& operator&=(Part& lhs, Part rhs) {
  lhs = lhs & rhs;
  return lhs;
}

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

// Masking operators for State (State | State)
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

// ============================================================================
// Cross-Type Masking Operators (Part | State) -> lv_style_selector_t
// See: https://github.com/pedapudi/lvgl_cpp/issues/168
// ============================================================================

/**
 * @brief Combine Part and State into a style selector.
 */
inline lv_style_selector_t operator|(Part p, State s) {
  return static_cast<lv_style_selector_t>(p) |
         static_cast<lv_style_selector_t>(s);
}

/**
 * @brief Combine State and Part into a style selector.
 */
inline lv_style_selector_t operator|(State s, Part p) {
  return static_cast<lv_style_selector_t>(p) |
         static_cast<lv_style_selector_t>(s);
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
 * @brief Wrapper for lv_text_decor_t.
 */
enum class TextDecor : uint8_t {
  None = LV_TEXT_DECOR_NONE,
  Underline = LV_TEXT_DECOR_UNDERLINE,
  Strikethrough = LV_TEXT_DECOR_STRIKETHROUGH,
};

inline TextDecor operator|(TextDecor lhs, TextDecor rhs) {
  return static_cast<TextDecor>(static_cast<uint8_t>(lhs) |
                                static_cast<uint8_t>(rhs));
}

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

/**
 * @brief Wrapper for lv_grad_dir_t.
 */
enum class GradDir : uint8_t {
  None = LV_GRAD_DIR_NONE,
  Ver = LV_GRAD_DIR_VER,
  Hor = LV_GRAD_DIR_HOR,
};

/**
 * @brief Wrapper for lv_image_align_t.
 */
enum class ImageAlign : uint8_t {
  Default = LV_IMAGE_ALIGN_DEFAULT,
  TopLeft = LV_IMAGE_ALIGN_TOP_LEFT,
  TopMid = LV_IMAGE_ALIGN_TOP_MID,
  TopRight = LV_IMAGE_ALIGN_TOP_RIGHT,
  BottomLeft = LV_IMAGE_ALIGN_BOTTOM_LEFT,
  BottomMid = LV_IMAGE_ALIGN_BOTTOM_MID,
  BottomRight = LV_IMAGE_ALIGN_BOTTOM_RIGHT,
  LeftMid = LV_IMAGE_ALIGN_LEFT_MID,
  RightMid = LV_IMAGE_ALIGN_RIGHT_MID,
  Center = LV_IMAGE_ALIGN_CENTER,
  Stretch = LV_IMAGE_ALIGN_STRETCH,
  Tile = LV_IMAGE_ALIGN_TILE,
  Contain = LV_IMAGE_ALIGN_CONTAIN,
  Cover = LV_IMAGE_ALIGN_COVER,
};

// ============================================================================
// Event Codes
// ============================================================================

/**
 * @brief Wrapper for lv_event_code_t.
 */
enum class EventCode : uint32_t {
  All = LV_EVENT_ALL,
  Pressed = LV_EVENT_PRESSED,
  Pressing = LV_EVENT_PRESSING,
  PressLost = LV_EVENT_PRESS_LOST,
  ShortClicked = LV_EVENT_SHORT_CLICKED,
  SingleClicked = LV_EVENT_SINGLE_CLICKED,
  DoubleClicked = LV_EVENT_DOUBLE_CLICKED,
  TripleClicked = LV_EVENT_TRIPLE_CLICKED,
  LongPressed = LV_EVENT_LONG_PRESSED,
  LongPressedRepeat = LV_EVENT_LONG_PRESSED_REPEAT,
  Clicked = LV_EVENT_CLICKED,
  Released = LV_EVENT_RELEASED,
  ScrollBegin = LV_EVENT_SCROLL_BEGIN,
  ScrollThrowBegin = LV_EVENT_SCROLL_THROW_BEGIN,
  ScrollEnd = LV_EVENT_SCROLL_END,
  Scroll = LV_EVENT_SCROLL,
  Gesture = LV_EVENT_GESTURE,
  Key = LV_EVENT_KEY,
  Rotary = LV_EVENT_ROTARY,
  Focused = LV_EVENT_FOCUSED,
  Defocused = LV_EVENT_DEFOCUSED,
  Leave = LV_EVENT_LEAVE,
  HitTest = LV_EVENT_HIT_TEST,
  IndevReset = LV_EVENT_INDEV_RESET,
  HoverOver = LV_EVENT_HOVER_OVER,
  HoverLeave = LV_EVENT_HOVER_LEAVE,
  CoverCheck = LV_EVENT_COVER_CHECK,
  RefrExtDrawSize = LV_EVENT_REFR_EXT_DRAW_SIZE,
  DrawMainBegin = LV_EVENT_DRAW_MAIN_BEGIN,
  DrawMain = LV_EVENT_DRAW_MAIN,
  DrawMainEnd = LV_EVENT_DRAW_MAIN_END,
  DrawPostBegin = LV_EVENT_DRAW_POST_BEGIN,
  DrawPost = LV_EVENT_DRAW_POST,
  DrawPostEnd = LV_EVENT_DRAW_POST_END,
  DrawTaskAdded = LV_EVENT_DRAW_TASK_ADDED,
  ValueChanged = LV_EVENT_VALUE_CHANGED,
  Insert = LV_EVENT_INSERT,
  Refresh = LV_EVENT_REFRESH,
  Ready = LV_EVENT_READY,
  Cancel = LV_EVENT_CANCEL,
  Create = LV_EVENT_CREATE,
  Delete = LV_EVENT_DELETE,
  ChildChanged = LV_EVENT_CHILD_CHANGED,
  ChildCreated = LV_EVENT_CHILD_CREATED,
  ChildDeleted = LV_EVENT_CHILD_DELETED,
  ScreenUnloadStart = LV_EVENT_SCREEN_UNLOAD_START,
  ScreenLoadStart = LV_EVENT_SCREEN_LOAD_START,
  ScreenLoaded = LV_EVENT_SCREEN_LOADED,
  ScreenUnloaded = LV_EVENT_SCREEN_UNLOADED,
  SizeChanged = LV_EVENT_SIZE_CHANGED,
  StyleChanged = LV_EVENT_STYLE_CHANGED,
  LayoutChanged = LV_EVENT_LAYOUT_CHANGED,
  GetSelfSize = LV_EVENT_GET_SELF_SIZE,
  InvalidateArea = LV_EVENT_INVALIDATE_AREA,
  ResolutionChanged = LV_EVENT_RESOLUTION_CHANGED,
  ColorFormatChanged = LV_EVENT_COLOR_FORMAT_CHANGED,
  RefrRequest = LV_EVENT_REFR_REQUEST,
  RefrStart = LV_EVENT_REFR_START,
  RefrReady = LV_EVENT_REFR_READY,
  RenderStart = LV_EVENT_RENDER_START,
  RenderReady = LV_EVENT_RENDER_READY,
  FlushStart = LV_EVENT_FLUSH_START,
  FlushFinish = LV_EVENT_FLUSH_FINISH,
  FlushWaitStart = LV_EVENT_FLUSH_WAIT_START,
  FlushWaitFinish = LV_EVENT_FLUSH_WAIT_FINISH,
  Vsync = LV_EVENT_VSYNC,
  VsyncRequest = LV_EVENT_VSYNC_REQUEST,
  Last = LV_EVENT_LAST,
};

// ============================================================================
// Layout Enums
// ============================================================================

/**
 * @brief Wrapper for lv_flex_flow_t.
 */
enum class FlexFlow : uint8_t {
  Row = LV_FLEX_FLOW_ROW,
  Column = LV_FLEX_FLOW_COLUMN,
  RowWrap = LV_FLEX_FLOW_ROW_WRAP,
  RowReverse = LV_FLEX_FLOW_ROW_REVERSE,
  RowWrapReverse = LV_FLEX_FLOW_ROW_WRAP_REVERSE,
  ColumnWrap = LV_FLEX_FLOW_COLUMN_WRAP,
  ColumnReverse = LV_FLEX_FLOW_COLUMN_REVERSE,
  ColumnWrapReverse = LV_FLEX_FLOW_COLUMN_WRAP_REVERSE,
};

/**
 * @brief Wrapper for lv_flex_align_t.
 */
enum class FlexAlign : uint8_t {
  Start = LV_FLEX_ALIGN_START,
  End = LV_FLEX_ALIGN_END,
  Center = LV_FLEX_ALIGN_CENTER,
  SpaceEvenly = LV_FLEX_ALIGN_SPACE_EVENLY,
  SpaceAround = LV_FLEX_ALIGN_SPACE_AROUND,
  SpaceBetween = LV_FLEX_ALIGN_SPACE_BETWEEN,
};

/**
 * @brief Wrapper for lv_grid_align_t.
 */
enum class GridAlign : uint8_t {
  Start = LV_GRID_ALIGN_START,
  Center = LV_GRID_ALIGN_CENTER,
  End = LV_GRID_ALIGN_END,
  Stretch = LV_GRID_ALIGN_STRETCH,
  SpaceEvenly = LV_GRID_ALIGN_SPACE_EVENLY,
  SpaceAround = LV_GRID_ALIGN_SPACE_AROUND,
  SpaceBetween = LV_GRID_ALIGN_SPACE_BETWEEN,
};

// ============================================================================
// Widget Mode Enums (remaining - to be migrated to class-scoped in future)
// ============================================================================

/**
 * @brief Wrapper for lv_keyboard_mode_t.
 * @deprecated Use Keyboard::Mode instead.
 */
using KeyboardMode [[deprecated("Use Keyboard::Mode instead")]] = uint8_t;
// Note: We use uint8_t here to avoid collision with the new enum if it's
// included, but the static_cast logic in the classes will still work. Actually,
// it's safer to keep it as enum class for now but mark it deprecated.

// ============================================================================
// Scale Enums
// ============================================================================

/**
 * @brief Wrapper for lv_scale_mode_t.
 * @deprecated Use Scale::Mode instead.
 */
enum class [[deprecated("Use Scale::Mode instead")]] ScaleMode : uint8_t {
  HorizontalTop = LV_SCALE_MODE_HORIZONTAL_TOP,
  HorizontalBottom = LV_SCALE_MODE_HORIZONTAL_BOTTOM,
  VerticalLeft = LV_SCALE_MODE_VERTICAL_LEFT,
  VerticalRight = LV_SCALE_MODE_VERTICAL_RIGHT,
  RoundInner = LV_SCALE_MODE_ROUND_INNER,
  RoundOuter = LV_SCALE_MODE_ROUND_OUTER,
};

// ============================================================================
// Scroll Enums
// ============================================================================

/**
 * @brief Wrapper for lv_scrollbar_mode_t.
 */
enum class ScrollbarMode : uint8_t {
  Off = LV_SCROLLBAR_MODE_OFF,
  On = LV_SCROLLBAR_MODE_ON,
  Active = LV_SCROLLBAR_MODE_ACTIVE,
  Auto = LV_SCROLLBAR_MODE_AUTO,
};

/**
 * @brief Wrapper for lv_scroll_snap_t.
 */
enum class ScrollSnap : uint8_t {
  None = LV_SCROLL_SNAP_NONE,
  Start = LV_SCROLL_SNAP_START,
  End = LV_SCROLL_SNAP_END,
  Center = LV_SCROLL_SNAP_CENTER,
};

/**
 * @brief Wrapper for lv_obj_flag_t.
 */
enum class ObjFlag : uint32_t {
  None = 0,
  Hidden = LV_OBJ_FLAG_HIDDEN,
  Clickable = LV_OBJ_FLAG_CLICKABLE,
  ClickFocusable = LV_OBJ_FLAG_CLICK_FOCUSABLE,
  Checkable = LV_OBJ_FLAG_CHECKABLE,
  Scrollable = LV_OBJ_FLAG_SCROLLABLE,
  ScrollElastic = LV_OBJ_FLAG_SCROLL_ELASTIC,
  ScrollMomentum = LV_OBJ_FLAG_SCROLL_MOMENTUM,
  ScrollChainHor = LV_OBJ_FLAG_SCROLL_CHAIN_HOR,
  ScrollChainVer = LV_OBJ_FLAG_SCROLL_CHAIN_VER,
  ScrollChain = LV_OBJ_FLAG_SCROLL_CHAIN,
  ScrollOnFocus = LV_OBJ_FLAG_SCROLL_ON_FOCUS,
  ScrollWithArrow = LV_OBJ_FLAG_SCROLL_WITH_ARROW,
  Snappable = LV_OBJ_FLAG_SNAPPABLE,
  PressLock = LV_OBJ_FLAG_PRESS_LOCK,
  EventBubble = LV_OBJ_FLAG_EVENT_BUBBLE,
  GestureBubble = LV_OBJ_FLAG_GESTURE_BUBBLE,
  AdvHitTest = LV_OBJ_FLAG_ADV_HITTEST,
  IgnoreLayout = LV_OBJ_FLAG_IGNORE_LAYOUT,
  Floating = LV_OBJ_FLAG_FLOATING,
  OverflowVisible = LV_OBJ_FLAG_OVERFLOW_VISIBLE,
  Layout1 = LV_OBJ_FLAG_LAYOUT_1,
  Layout2 = LV_OBJ_FLAG_LAYOUT_2,
  Widget1 = LV_OBJ_FLAG_WIDGET_1,
  Widget2 = LV_OBJ_FLAG_WIDGET_2,
  User1 = LV_OBJ_FLAG_USER_1,
  User2 = LV_OBJ_FLAG_USER_2,
  User3 = LV_OBJ_FLAG_USER_3,
  User4 = LV_OBJ_FLAG_USER_4,
};

inline ObjFlag operator|(ObjFlag lhs, ObjFlag rhs) {
  return static_cast<ObjFlag>(static_cast<uint32_t>(lhs) |
                              static_cast<uint32_t>(rhs));
}

/**
 * @brief Wrapper for lv_key_t.
 */
enum class Key : uint8_t {
  Up = LV_KEY_UP,
  Down = LV_KEY_DOWN,
  Right = LV_KEY_RIGHT,
  Left = LV_KEY_LEFT,
  Esc = LV_KEY_ESC,
  Del = LV_KEY_DEL,
  Backspace = LV_KEY_BACKSPACE,
  Enter = LV_KEY_ENTER,
  Next = LV_KEY_NEXT,
  Prev = LV_KEY_PREV,
  Home = LV_KEY_HOME,
  End = LV_KEY_END,
};

/**
 * @brief Wrapper for lv_obj_point_transform_flag_t.
 */
enum class PointTransformFlag : uint8_t {
  None = LV_OBJ_POINT_TRANSFORM_FLAG_NONE,
  Recursive = LV_OBJ_POINT_TRANSFORM_FLAG_RECURSIVE,
  Inverse = LV_OBJ_POINT_TRANSFORM_FLAG_INVERSE,
  InverseRecursive = LV_OBJ_POINT_TRANSFORM_FLAG_INVERSE_RECURSIVE,
};

inline PointTransformFlag operator|(PointTransformFlag lhs,
                                    PointTransformFlag rhs) {
  return static_cast<PointTransformFlag>(static_cast<uint8_t>(lhs) |
                                         static_cast<uint8_t>(rhs));
}

// ============================================================================
// Input Device Enums
// ============================================================================

/**
 * @brief Wrapper for lv_indev_state_t.
 * @deprecated Use InputDevice::State instead.
 */
enum class [[deprecated(
    "Use InputDevice::State instead")]] IndevState : uint8_t {
  Released = LV_INDEV_STATE_RELEASED,
  Pressed = LV_INDEV_STATE_PRESSED,
};

/**
 * @brief Wrapper for lv_indev_type_t.
 * @deprecated Use InputDevice::Type instead.
 */
enum class IndevType : uint8_t {
  None = LV_INDEV_TYPE_NONE,
  Pointer = LV_INDEV_TYPE_POINTER,
  Keypad = LV_INDEV_TYPE_KEYPAD,
  Button = LV_INDEV_TYPE_BUTTON,
  Encoder = LV_INDEV_TYPE_ENCODER,
};

/**
 * @brief Wrapper for lv_indev_mode_t.
 */
enum class IndevMode : uint8_t {
  None = LV_INDEV_MODE_NONE,
  Timer = LV_INDEV_MODE_TIMER,
  Event = LV_INDEV_MODE_EVENT,
};

/**
 * @brief Wrapper for lv_indev_gesture_type_t.
 */
enum class GestureType : uint8_t {
  None = LV_INDEV_GESTURE_NONE,
  Pinch = LV_INDEV_GESTURE_PINCH,
  Swipe = LV_INDEV_GESTURE_SWIPE,
  Rotate = LV_INDEV_GESTURE_ROTATE,
  TwoFingersSwipe = LV_INDEV_GESTURE_TWO_FINGERS_SWIPE,
  Scroll = LV_INDEV_GESTURE_SCROLL,
};

/**
 * @brief Wrapper for lv_roller_mode_t.
 * @deprecated Use Roller::Mode instead.
 */
enum class [[deprecated("Use Roller::Mode instead")]] RollerMode : uint8_t {
  Normal = LV_ROLLER_MODE_NORMAL,
  Infinite = LV_ROLLER_MODE_INFINITE,
};

/**
 * @brief Wrapper for lv_base_dir_t.
 */
enum class BaseDir : uint8_t {
  Ltr = LV_BASE_DIR_LTR,
  Rtl = LV_BASE_DIR_RTL,
  Auto = LV_BASE_DIR_AUTO,
};

/**
 * @brief Wrapper for lv_screen_load_anim_t.
 */
enum class ScreenLoadAnim : uint8_t {
  None = LV_SCR_LOAD_ANIM_NONE,
  OverLeft = LV_SCR_LOAD_ANIM_OVER_LEFT,
  OverRight = LV_SCR_LOAD_ANIM_OVER_RIGHT,
  OverTop = LV_SCR_LOAD_ANIM_OVER_TOP,
  OverBottom = LV_SCR_LOAD_ANIM_OVER_BOTTOM,
  MoveLeft = LV_SCR_LOAD_ANIM_MOVE_LEFT,
  MoveRight = LV_SCR_LOAD_ANIM_MOVE_RIGHT,
  MoveTop = LV_SCR_LOAD_ANIM_MOVE_TOP,
  MoveBottom = LV_SCR_LOAD_ANIM_MOVE_BOTTOM,
  FadeOn = LV_SCR_LOAD_ANIM_FADE_ON,
  FadeOut = LV_SCR_LOAD_ANIM_FADE_OUT,
  OutLeft = LV_SCR_LOAD_ANIM_OUT_LEFT,
  OutRight = LV_SCR_LOAD_ANIM_OUT_RIGHT,
  OutTop = LV_SCR_LOAD_ANIM_OUT_TOP,
  OutBottom = LV_SCR_LOAD_ANIM_OUT_BOTTOM,
};

// ============================================================================
// Color and Display Enums
// ============================================================================

/**
 * @brief Wrapper for lv_palette_t.
 */
enum class Palette : uint8_t {
  Red = LV_PALETTE_RED,
  Pink = LV_PALETTE_PINK,
  Purple = LV_PALETTE_PURPLE,
  DeepPurple = LV_PALETTE_DEEP_PURPLE,
  Indigo = LV_PALETTE_INDIGO,
  Blue = LV_PALETTE_BLUE,
  LightBlue = LV_PALETTE_LIGHT_BLUE,
  Cyan = LV_PALETTE_CYAN,
  Teal = LV_PALETTE_TEAL,
  Green = LV_PALETTE_GREEN,
  LightGreen = LV_PALETTE_LIGHT_GREEN,
  Lime = LV_PALETTE_LIME,
  Yellow = LV_PALETTE_YELLOW,
  Amber = LV_PALETTE_AMBER,
  Orange = LV_PALETTE_ORANGE,
  DeepOrange = LV_PALETTE_DEEP_ORANGE,
  Brown = LV_PALETTE_BROWN,
  BlueGrey = LV_PALETTE_BLUE_GREY,
  Grey = LV_PALETTE_GREY,
  None = LV_PALETTE_NONE,
};

/**
 * @brief Wrapper for lv_color_format_t.
 */
enum class ColorFormat : uint8_t {
  Unknown = LV_COLOR_FORMAT_UNKNOWN,
  L8 = LV_COLOR_FORMAT_L8,
  A8 = LV_COLOR_FORMAT_A8,
  I1 = LV_COLOR_FORMAT_I1,
  I2 = LV_COLOR_FORMAT_I2,
  I4 = LV_COLOR_FORMAT_I4,
  I8 = LV_COLOR_FORMAT_I8,
  RGB565 = LV_COLOR_FORMAT_RGB565,
  RGB888 = LV_COLOR_FORMAT_RGB888,
  ARGB8888 = LV_COLOR_FORMAT_ARGB8888,
  XRGB8888 = LV_COLOR_FORMAT_XRGB8888,
};

/**
 * @brief Wrapper for lv_anim_enable_t.
 */
enum class AnimEnable : uint8_t {
  Off = LV_ANIM_OFF,
  On = LV_ANIM_ON,
};

/**
 * @brief Wrapper for lv_result_t.
 */
enum class Result : uint8_t {
  Invalid = LV_RESULT_INVALID,
  Ok = LV_RESULT_OK,
};

/**
 * @brief Wrapper for lv_layer_type_t.
 */
enum class LayerType : uint8_t {
  None = LV_LAYER_TYPE_NONE,
  Simple = LV_LAYER_TYPE_SIMPLE,
  Transform = LV_LAYER_TYPE_TRANSFORM,
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_ENUMS_H_
