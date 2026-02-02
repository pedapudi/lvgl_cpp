#include <cstdio>
#include <type_traits>

#include "../core/object.h"
#include "../widgets/button.h"
#include "../widgets/label.h"

int main() {
  printf("sizeof(void*): %zu\n", sizeof(void*));
  printf("sizeof(lvgl::Object): %zu\n", sizeof(lvgl::Object));
  printf("sizeof(lvgl::Button): %zu\n", sizeof(lvgl::Button));
  printf("sizeof(lvgl::Label): %zu\n", sizeof(lvgl::Label));

  // Modern lvgl_cpp Object includes RAII (bool) and callback vector (24 bytes)
  // plus virtual table (8 bytes). 48 bytes is expected on 64-bit.
  bool is_reasonable_size = (sizeof(lvgl::Object) <= 64);
  printf("Is Object size reasonable (<= 64 bytes)? %s\n",
         is_reasonable_size ? "YES" : "NO");

  if (!is_reasonable_size) {
    printf("FAIL: Object is unexpectedly large (%zu bytes).\n",
           sizeof(lvgl::Object));
    return 1;
  }
  return 0;
}
