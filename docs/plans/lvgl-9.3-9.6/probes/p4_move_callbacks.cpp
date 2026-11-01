#include "common.h"
#include <utility>
int main() { init();
  int hits = 0;
  Button* b = nullptr;
  { Button a; a.on_clicked([&](Event&) { hits++; }); b = new Button(std::move(a)); }  // 'a' (and its callback_nodes_) destroyed here
  lv_obj_send_event(b->raw(), LV_EVENT_CLICKED, nullptr);
  printf("hits=%d\n", hits);
}
