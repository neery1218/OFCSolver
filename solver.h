#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <string>
#include <vector>
#include <array>
#include <ostream>
#include "gametype.h"
#include "pull.h"
#include "hand.h"

enum Method {
  CPU = 0,
  GPU = 1
};

struct Placement {
  Card card;
  Hand::Position position;
  Placement(Card _card, Hand::Position _position): card{_card}, position{_position} {}
};

std::ostream& operator<<(std::ostream& os, const Placement &p);

struct Decision {
  std::array<Placement,3> placements;
};

std::ostream& operator<<(std::ostream& os, const Decision &d);

class Solver { // maybe this should be a standalone function?
  Method method;
  public:
    Solver(Method method);
    Decision solve(Hand &myHand, Pull &myPull, std::vector<Hand> otherHands, 
        std::vector<Decision> decisions, GameType type, std::vector<Card> deadCards);
};

#endif
