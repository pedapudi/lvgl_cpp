#include "common.h"
#include <vector>
int main() { init();
  IntSubject s(0);
  int hits = 0;
  std::vector<Observer> v;
  v.push_back(Observer(s, [&](Observer*) { hits++; }));
  printf("hits after add=%d\n", hits);
  s.set(5);
  printf("hits after set (expect +1)=%d\n", hits);
}
