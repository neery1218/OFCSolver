#ifndef _FANTASY_SOLVER_
#define _FANTASY_SOLVER_

#include <set>

#include "card.h"
#include "gametype.h"
#include "hand.h"
#include "optimal_hand.h"
#include "poker_hand_evaluator.h"

class FantasySolver {
 public:
  FantasySolver() {}
  Decision solve(FastPokerHandEvaluator* eval, std::vector<Card>& cards) {
    Hand emptyHand;

    std::sort(cards.begin(), cards.end());
    CompletedHand completedHand =
        OptimalHand::constructOptimalHand(emptyHand, cards, eval, true);

    std::vector<Placement> placements;
    for (auto c : completedHand.h->top) {
      placements.push_back(Placement{c, Position::top});
    }
    for (auto c : completedHand.h->middle) {
      placements.push_back(Placement{c, Position::middle});
    }
    for (auto c : completedHand.h->bottom) {
      placements.push_back(Placement{c, Position::bottom});
    }

    return Decision(placements);
  }
};

#endif
