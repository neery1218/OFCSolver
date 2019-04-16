#ifndef _PLACEMENT_H_
#define _PLACEMENT_H_

#include "position.h"
#include "card.h"

class Hand;

struct Placement {
  Card card;
  Position position;
  Placement(Card _card, Position _position): card{_card}, position{_position} {}
};

inline std::ostream& operator<<(std::ostream& os, const Placement &p) {
  std::string pos = "top";
  if (p.position == Position::middle) { pos = "middle"; }
  if (p.position == Position::bottom) { pos = "bottom"; }
  if (p.position == Position::dead) { pos = "dead"; }

  os << p.card << " " << pos;
  return os;
}

#endif
