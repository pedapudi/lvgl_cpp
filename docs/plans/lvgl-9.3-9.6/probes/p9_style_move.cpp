#include "common.h"
#include <utility>
int main() { init();
  Button b;
  Style s1; s1.bg_color(Color::white());
  b.add_style(s1);
  { Style s2(std::move(s1)); }   // s2 dtor resets the props array s1/b still point to
  s1.radius(4);                   // writes into freed props of moved-from style
  lv_obj_invalidate(b.raw()); lv_refr_now(nullptr);
  printf("survived\n");
}
