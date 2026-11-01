#include "common.h"
int main() { init();
  int hits = 0; lv_obj_t* raw;
  { Button b; b.on_clicked([&](Event&) { hits++; }); raw = b.release(); }
  lv_obj_send_event(raw, LV_EVENT_CLICKED, nullptr);
  printf("hits after release()+wrapper destroyed=%d (callback silently dropped if 0)\n", hits);
}
