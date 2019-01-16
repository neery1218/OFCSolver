#include "pull.h"
#include <iostream>

using namespace std;

ostream& operator<<(ostream& os, const Pull &p) {
  for (const Card &c : p.cards) {
    os << c;
  }
  return os;
}
