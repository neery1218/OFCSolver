#pragma once

#include <cmath>
#include <ostream>

struct DecisionStats {
  double mean;
  double variance;
  double aces_fantasy_pct;
  double kings_fantasy_pct;
  double queens_fantasy_pct;
};

inline std::ostream &operator<<(std::ostream &os, const DecisionStats &d) {
  os << " EV: " << d.mean;
  os << " stddev: " << sqrt(d.variance);
  os << " afp: " << d.aces_fantasy_pct;
  os << " kfp: " << d.kings_fantasy_pct;
  os << " qfp: " << d.queens_fantasy_pct;
  return os;
}
