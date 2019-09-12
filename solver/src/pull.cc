#include "pull.h"
#include "card.h"
#include <iostream>

using namespace std;

ostream& operator<<(ostream& os, const Pull &p) {
  for (const Card &c : p.cards) {
    os << CardUtils::cardToString(c) << " ";
  }
  return os;
}
