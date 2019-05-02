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
    double solve(int numIterations, const Hand &my_hand, const Pull &my_pull, const std::vector<Hand> &other_hands, const std::vector<Card> &dead_cards) const;
  private:
    unsigned int findCardsNeeded(const Hand &my_hand) const;
};

#endif
