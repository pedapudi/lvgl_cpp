#include "common.h"
int main() { init(); Object o; o.set_id(nullptr); (void)o.get_id(); (void)o.find_by_id(nullptr); }
