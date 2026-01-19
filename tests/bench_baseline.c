/*
 * Benchmark: Baseline Object Creation (C)
 * Objective: Measure fixed overhead of creating objects in pure C.
 */

#include <stdio.h>
#include <stdlib.h>

#include "lvgl.h"

#define OBJ_COUNT 1000

// Mock driver to satisfy LVGL
static void flush_cb(lv_display_t* disp, const lv_area_t* area,
                     uint8_t* px_map) {
  lv_display_flush_ready(disp);
}

int main(void) {
  lv_init();

  // Minimal display setup
  lv_display_t* disp = lv_display_create(800, 600);
  lv_display_set_flush_cb(disp, flush_cb);
  static uint8_t buf[800 * 10 * 4];
  lv_display_set_buffers(disp, buf, NULL, sizeof(buf),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);

  printf("Starting C baseline benchmark (N=%d)...\n", OBJ_COUNT);

  lv_obj_t* screen = lv_scr_act();
  lv_obj_t* objects[OBJ_COUNT];

  for (int i = 0; i < OBJ_COUNT; i++) {
    objects[i] = lv_button_create(screen);
    lv_obj_set_pos(objects[i], i % 100, i / 100);
    lv_obj_set_size(objects[i], 50, 30);
  }

  // Force a layout/update to ensure structures are fully allocated
  lv_timer_handler();

  printf("Objects created. Sleeping for profile capture...\n");
  // In a real profile run, this sleep allows ensuring background threads (if
  // any) settle, though for heap profile we just care about the snapshot here.

  return 0;
}
