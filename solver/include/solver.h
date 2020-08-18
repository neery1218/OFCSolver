#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <array>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "card.h"
#include "decision.h"
#include "decision_stats.h"
#include "deck.h"
#include "gametype.h"
#include "hand.h"
#include "pull.h"

/*
 * tldr: Pass hands to solver, solver estimates EV of hand. Used by
 * DecisionFinder, AdvancedDecisionFinder
 * */

class Solver {
  const FastPokerHandEvaluator *evaluator;
  std::mt19937 *rng;

 public:
  Solver(const FastPokerHandEvaluator *evaluator, std::mt19937 *rng);
#ifdef RESEARCH
  std::vector<double> solve(int numIterations, const Hand &my_hand,
                            const Pull &my_pull,
                            const std::vector<Hand> &other_hands,
                            const std::vector<Card> &dead_cards);
#else
  DecisionStats solve(int numIterations, const Hand &my_hand,
                      const Pull &my_pull, const std::vector<Hand> &other_hands,
                      const std::vector<Card> &dead_cards);
#endif
  Solver(const Solver &) = delete;
  Solver &operator=(Solver &) = delete;

 private:
  unsigned int findCardsNeeded(const Hand &my_hand) const;
};

#endif
