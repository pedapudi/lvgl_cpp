/*
 * Benchmark: Event Overhead (C)
 * Objective: Measure callbacks overhead in pure C.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <time.h>

#include "lvgl.h"

#define OBJ_COUNT 500

static void flush_cb(lv_display_t* disp, const lv_area_t* area,
                     uint8_t* px_map) {
  lv_display_flush_ready(disp);
}

static void my_event_cb(lv_event_t* e) {
  // No-op
}

int main(void) {
  lv_init();

  lv_display_t* disp = lv_display_create(800, 600);
  lv_display_set_flush_cb(disp, flush_cb);
  static uint8_t buf[800 * 10 * 4];
  lv_display_set_buffers(disp, buf, NULL, sizeof(buf),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);

  printf("Starting C events benchmark (N=%d)...\n", OBJ_COUNT);

  lv_obj_t* screen = lv_scr_act();
  lv_obj_t* objects[OBJ_COUNT];

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  for (int i = 0; i < OBJ_COUNT; i++) {
    objects[i] = lv_button_create(screen);
    // Add one lightweight C event
    lv_obj_add_event_cb(objects[i], my_event_cb, LV_EVENT_CLICKED, NULL);
  }

  lv_timer_handler();
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
