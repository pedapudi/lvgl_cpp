#include "common.h"
int main() { init();
  Object scr(lv_screen_active(), Ownership::Unmanaged);
  { Object o(&scr); } printf("Object(&parent) default: children after scope=%u\n", (unsigned)lv_obj_get_child_count(lv_screen_active()));
  { Button b(scr); } printf("+ Button(parent) default: children after scope=%u\n", (unsigned)lv_obj_get_child_count(lv_screen_active()));
  { Label l(scr); } printf("+ Label(parent) default: children after scope=%u\n", (unsigned)lv_obj_get_child_count(lv_screen_active()));
}
