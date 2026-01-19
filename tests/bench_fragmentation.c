/*
 * Benchmark: Fragmentation (C Baseline)
 * Objective: Measure heap fragmentation using actual LVGL widgets and C
 * callbacks.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <time.h>
#include <unistd.h>

#include "lvgl.h"

#define MAX_ALLOCS 1000
#define ITERATIONS 50

// Dummy callback to match C++ lambda behavior
static void event_cb(lv_event_t* e) { (void)e; }

static void flush_cb(lv_display_t* disp, const lv_area_t* area,
                     uint8_t* px_map) {
  lv_display_flush_ready(disp);
}

int main(void) {
  lv_init();

  lv_display_t* disp = lv_display_create(800, 600);
  lv_display_set_flush_cb(disp, flush_cb);
  static uint8_t buf[800 * 10 * 4];
  lv_display_set_buffers(disp, buf, NULL, sizeof(buf),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);

  printf("Starting Fragmentation C benchmark (LVGL Widgets)...\n");

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  // Array to hold object pointers
  lv_obj_t** objects = (lv_obj_t**)malloc(MAX_ALLOCS * sizeof(lv_obj_t*));
  int obj_count = 0;

  lv_obj_t* screen = lv_screen_active();

  // Simple PRNG
  srand(42);

  for (int i = 0; i < ITERATIONS; i++) {
    for (int j = 0; j < 50; j++) {
      int r = rand();
      int op = r % 3;  // 0=alloc, 1=free, 2=noop

      if (op == 0 && obj_count < MAX_ALLOCS) {
        // Alloc: Create Button + Attach Callback
        lv_obj_t* btn = lv_button_create(screen);

        // Attach standard C callback (no capture context overhead, just
        // internal node)
        lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);

        objects[obj_count++] = btn;

      } else if (op == 1 && obj_count > 0) {
        // Free: Delete random object
        int idx = rand() % obj_count;

        lv_obj_delete(objects[idx]);

        // Swap with last
        objects[idx] = objects[obj_count - 1];
        obj_count--;
      }
    }

    lv_timer_handler();
  }

  printf("Fragmentation C workload completed. Objects alive: %d\n", obj_count);

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
