#ifndef _DECISION_H_
#define _DECISION_H_

#include "placement.h"
#include <vector>

struct Decision {
  std::vector<Placement> placements;
  Decision(Placement p1, Placement p2): placements{{p1, p2}} {}
  Decision(Placement p1, Placement p2, Placement p3): placements{{p1, p2, p3}} {}
};

inline std::ostream& operator<<(std::ostream& os, const Decision &d) {
  os << d.placements.at(0) << std::endl << d.placements.at(1);
  if (d.placements.size() == 3) os << d.placements.at(2) << std::endl;
  return os;
}

#endif
