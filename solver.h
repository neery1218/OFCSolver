#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <string>
#include <vector>
#include <array>
#include <ostream>
#include "gametype.h"
#include "pull.h"
#include "hand.h"

enum Position {
  top = 0,
  middle = 1,
  bottom = 2,
  dead = 3
};

struct Placement {
  Card card;
  Position position;
  Placement(Card _card, Position _position): card{_card}, position{_position} {}
};

std::ostream& operator<<(std::ostream& os, const Placement &p);

struct Decision {
  std::array<Placement,3> placements;
};

std::ostream& operator<<(std::ostream& os, const Decision &d);

class Solver { // maybe this should be a standalone function?
  public:
    Solver();
    Decision solve(Hand &myHand, Pull &myPull, std::vector<Hand> otherHands, 
        std::vector<Decision> decisions, GameType type);
};

#endif
