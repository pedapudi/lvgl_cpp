#include "common.h"
int main() { init();
  Button b;
  uint32_t before = lv_obj_get_event_count(b.raw());
  { Object w1(b.raw()); Object w2 = b.get_parent(); Object w3(b.raw()); printf("event dsc count with 2 extra wrappers alive: %u -> %u\n", (unsigned)before, (unsigned)lv_obj_get_event_count(b.raw())); }
  lv_mem_monitor_t m0, m1; lv_mem_monitor(&m0);
  for (int i = 0; i < 1000; i++) { Object w(b.raw()); }
  lv_mem_monitor(&m1);
  printf("free_size before/after 1000 temp wrappers: %u / %u\n", (unsigned)m0.free_size, (unsigned)m1.free_size);
}
