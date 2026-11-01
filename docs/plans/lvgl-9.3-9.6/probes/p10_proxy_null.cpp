#include "common.h"
int main() { init();
  Object o(static_cast<lv_obj_t*>(nullptr));
  o.style().bg_color(Color::white());
  o.scroll().to(1,1,AnimEnable::Off);
  printf("survived null proxies\n");
}
