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

  bool is_ptr_size = (sizeof(lvgl::Object) == sizeof(void*));
  printf("Is Object pointer-sized? %s\n", is_ptr_size ? "YES" : "NO");

  if (!is_ptr_size) {
    printf("FAIL: Object is too large.\n");
    return 1;
  }
  return 0;
}
