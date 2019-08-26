#ifndef _FANTASY_SOLVER_
#define _FANTASY_SOLVER_

#include "hand.h"
#include "card.h"
#include "gametype.h"
#include "poker_hand_evaluator.h"
#include <set>


class FantasySolver {
  PokerHandEvaluator evaluator;

  public:
    FantasySolver();
    CompletedHand solve(GameType type, std::set<Card> &cards);
};

#endif
