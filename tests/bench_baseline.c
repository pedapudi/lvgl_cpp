/*
 * Benchmark: Baseline Object Creation (C)
 * Objective: Measure fixed overhead of creating objects in pure C.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <time.h>

#include "lvgl.h"

#define OBJ_COUNT 50

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

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  for (int i = 0; i < OBJ_COUNT; i++) {
    objects[i] = lv_button_create(screen);
    lv_obj_set_pos(objects[i], i % 100, i / 100);
    lv_obj_set_size(objects[i], 50, 30);
  }

  // Force a layout/update to ensure structures are fully allocated
  lv_timer_handler();

  clock_gettime(CLOCK_MONOTONIC, &end);
  double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                      (end.tv_nsec - start.tv_nsec) / 1000000.0;

  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);

  printf("BENCHMARK_METRIC: TIME=%.2f unit=ms\n", elapsed_ms);
  printf("BENCHMARK_METRIC: RSS=%ld unit=kb\n", usage.ru_maxrss);

  return 0;
}
