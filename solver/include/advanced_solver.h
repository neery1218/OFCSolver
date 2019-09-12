#ifndef _ADVANCED_SOLVER_H_
#define _ADVANCED_SOLVER_H_

#include "fast_poker_hand_evaluator.h"
#include "game_state.h"
#include "deck.h"

struct SolverParams {
  unsigned int stage_one_iterations;
  unsigned int stage_two_iterations;
  unsigned int search_level;
};

class AdvancedSolver {
  const FastPokerHandEvaluator *evaluator;

  public:
    AdvancedSolver(const FastPokerHandEvaluator *evaluator);
    double solve(int iterations, const GameState &game_state, const Deck &initial_deck, int search_level) const;
    AdvancedSolver(const AdvancedSolver&) = delete;
    AdvancedSolver& operator=(AdvancedSolver&) = delete;
};
#endif
