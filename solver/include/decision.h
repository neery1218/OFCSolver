#ifndef _DECISION_H_
#define _DECISION_H_

#include <string>
#include <vector>

#include "placement.h"

struct Decision {
  std::vector<Placement> placements;
  Decision(std::vector<Placement> p) : placements{p} {}
  Decision(Placement p1, Placement p2) : placements{{p1, p2}} {}
  Decision(Placement p1, Placement p2, Placement p3)
      : placements{{p1, p2, p3}} {}
};

inline std::ostream &operator<<(std::ostream &os, const Decision &d) {
  for (auto &p : d.placements) {
    os << p << " ";
  }
  return os;
}
#endif
