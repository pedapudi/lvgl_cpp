/*
 * Benchmark: Various Widgets (C Baseline)
 * Objective: Measure creation overhead of specific widgets (C Baseline).
 * Usage: ./bench_widgets_c <WIDGET_TYPE>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>

#include "lvgl.h"

#define OBJ_COUNT 50

static void flush_cb(lv_display_t* disp, const lv_area_t* area,
                     uint8_t* px_map) {
  lv_display_flush_ready(disp);
}

int main(int argc, char** argv) {
  lv_init();

  lv_display_t* disp = lv_display_create(800, 600);
  lv_display_set_flush_cb(disp, flush_cb);
  static uint8_t buf[800 * 10 * 4];
  lv_display_set_buffers(disp, buf, NULL, sizeof(buf),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);

  const char* widget_type = "slider";
  if (argc > 1) widget_type = argv[1];

  printf("Starting C benchmark for %s (N=%d)...\n", widget_type, OBJ_COUNT);

  lv_obj_t* screen = lv_scr_act();
  lv_obj_t** objects = (lv_obj_t**)malloc(OBJ_COUNT * sizeof(lv_obj_t*));

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  for (int i = 0; i < OBJ_COUNT; i++) {
    lv_obj_t* obj = NULL;

    if (strcmp(widget_type, "arc") == 0) {
      obj = lv_arc_create(screen);
    } else if (strcmp(widget_type, "checkbox") == 0) {
      obj = lv_checkbox_create(screen);
      lv_checkbox_set_text(obj, "Check me");
    } else if (strcmp(widget_type, "slider") == 0) {
      obj = lv_slider_create(screen);
    } else if (strcmp(widget_type, "switch") == 0) {
      obj = lv_switch_create(screen);
    } else if (strcmp(widget_type, "textarea") == 0) {
      obj = lv_textarea_create(screen);
      lv_textarea_set_text(obj, "Hello");
    } else {
      printf("Unknown widget type: %s\n", widget_type);
      return 1;
    }

    lv_obj_set_pos(obj, i % 100, i / 100);
    lv_obj_set_size(obj, 50, 30);
    objects[i] = obj;
  }

  lv_timer_handler();

  clock_gettime(CLOCK_MONOTONIC, &end);
  double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                      (end.tv_nsec - start.tv_nsec) / 1000000.0;

  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);

  printf("BENCHMARK_METRIC: TIME=%.2f unit=ms\n", elapsed_ms);
  printf("BENCHMARK_METRIC: RSS=%ld unit=kb\n", usage.ru_maxrss);

  free(objects);
  return 0;
}
