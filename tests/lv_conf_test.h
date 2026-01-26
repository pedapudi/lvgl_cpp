#ifndef LV_CONF_H
#define LV_CONF_H

#define LV_COLOR_DEPTH 32
#define LV_MEM_SIZE (4096 * 1024)

#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_INFO
#define LV_LOG_PRINTF 1

#define LV_USE_DEMO 0
#define LV_BUILD_EXAMPLES 0

#define LV_USE_BUTTON 1
#define LV_USE_LABEL 1
#define LV_USE_SLIDER 1
#define LV_USE_OBJ_PROPERTY 1
#define LV_USE_FLOAT 1  // Label formatting might need this

// Core
#define LV_USE_GROUP 1
#define LV_USE_INDEV 1
#define LV_USE_DISPLAY 1
#define LV_USE_TIMER 1
#define LV_USE_ANIMATION 1
#define LV_USE_ANIM_TIMELINE 1
#define LV_USE_OBSERVER 1

// Layouts
#define LV_USE_FLEX 1
#define LV_USE_GRID 1

// Widgets
#define LV_USE_BAR 1
#define LV_USE_CHECKBOX 1
#define LV_USE_SWITCH 1
#define LV_USE_LINE 1
#define LV_USE_ARC 1
#define LV_USE_IMAGE 1
#define LV_USE_TEXTAREA 1
#define LV_USE_SPINBOX 1
#define LV_USE_DROPDOWN 1
#define LV_USE_ROLLER 1
#define LV_USE_BUTTONMATRIX 1
#define LV_USE_KEYBOARD 1
#define LV_USE_TABLE 1
#define LV_USE_CANVAS 1
#define LV_USE_CHART 1
#define LV_USE_CALENDAR 1
#define LV_USE_MSGBOX 1
#define LV_USE_MENU 1
#define LV_USE_TILEVIEW 1
#define LV_USE_TABVIEW 1
#define LV_USE_WIN 1
#define LV_USE_SCALE 1
#define LV_USE_ANIMIMG 1  // check name, usually LV_USE_ANIMIMG
#define LV_USE_SPAN 1
#define LV_USE_LIST 1
#define LV_USE_LED 1
#define LV_USE_IMAGEBUTTON 1
#define LV_USE_SPINNER 1
#define LV_USE_LOTTIE 1
#define LV_USE_THORVG_INTERNAL 1
#define LV_USE_VECTOR_GRAPHIC 1
#define LV_USE_THORVG_EXTERNAL 0
#define LV_USE_MATRIX 1

// Filesystem
#define LV_USE_FS_STDIO 1
#if LV_USE_FS_STDIO
#define LV_FS_STDIO_LETTER                                                  \
  'A' /**< Set an upper-case driver-identifier letter for this driver (e.g. \
         'A'). */
#define LV_FS_STDIO_PATH                                                      \
  "" /**< Set the working directory. File/directory paths will be appended to \
        it. */
#define LV_FS_STDIO_CACHE_SIZE \
  0 /**< >0 to cache this number of bytes in lv_fs_read() */
#endif

#endif /*LV_CONF_H*/
