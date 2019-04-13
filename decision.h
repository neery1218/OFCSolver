#ifndef _DECISION_H_
#define _DECISION_H_

#include "placement.h"

struct Decision {
  std::array<Placement, 3> placements;
};

inline std::ostream& operator<<(std::ostream& os, const Decision &d) {
  os << d.placements.at(0) << std::endl << d.placements.at(1)  << std::endl << d.placements.at(2);
  return os;
}

#endif
