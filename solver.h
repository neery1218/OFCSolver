#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <string>
#include <vector>
#include <array>
#include <ostream>
#include <unordered_map>
#include <utility>
#include "gametype.h"
#include "pull.h"
#include "hand.h"
#include "deck.h"
#include "card.h"
#include "decision.h"

/*
 * tldr: Pass hands to solver, solver estimates EV of hand. Used by DecisionFinder
 * */


class Solver {
  const PokerHandEvaluator *evaluator;

  public:
    Solver(const PokerHandEvaluator *evaluator);
    double solve(int numIterations, const Hand &myHand, const Pull &myPull, const std::vector<Hand> &otherHands, const std::vector<Card> &deadCards) const;
};

#endif
