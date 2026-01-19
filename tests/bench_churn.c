/*
 * Benchmark: Churn (C Baseline)
 * Objective: Measure performance of creating/destroying objects in pure C.
 */

#include <stdio.h>
#include <sys/resource.h>
#include <time.h>

#include "lvgl.h"

#define ITERATIONS 100
#define WIDGETS_PER_SCREEN 20

static void dummy_event_cb(lv_event_t* e) { (void)e; }

static void flush_cb(lv_display_t* disp, const lv_area_t* area,
                     uint8_t* px_map) {
  lv_display_flush_ready(disp);
}

void run_cycle() {
  // 1. Create a Screen
  lv_obj_t* screen = lv_obj_create(NULL);

  // 2. Add widgets
  for (int i = 0; i < WIDGETS_PER_SCREEN; i++) {
    lv_obj_t* btn = lv_button_create(screen);
    lv_obj_set_pos(btn, i * 10, i * 10);

    // Add callback
    lv_obj_add_event_cb(btn, dummy_event_cb, LV_EVENT_CLICKED, NULL);
  }

  lv_timer_handler();

  // 3. Destroy
  // In C, deleting the parent automatically deletes children.
  lv_obj_delete(screen);
}

int main(void) {
  lv_init();

  lv_display_t* disp = lv_display_create(800, 600);
  lv_display_set_flush_cb(disp, flush_cb);
  static uint8_t buf[800 * 10 * 4];
  lv_display_set_buffers(disp, buf, NULL, sizeof(buf),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);

  printf("Starting Churn C benchmark (%d cycles)...\n", ITERATIONS);

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  for (int i = 0; i < ITERATIONS; i++) {
    if (i % 10 == 0) printf("Cycle %d\n", i);
    run_cycle();
  }

  clock_gettime(CLOCK_MONOTONIC, &end);
  double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                      (end.tv_nsec - start.tv_nsec) / 1000000.0;

  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);

  printf("BENCHMARK_METRIC: TIME=%.2f unit=ms\n", elapsed_ms);
  printf("BENCHMARK_METRIC: RSS=%ld unit=kb\n", usage.ru_maxrss);

  printf("Churn C benchmark completed.\n");
  return 0;
}
