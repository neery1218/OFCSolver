#ifndef _ADVANCED_SOLVER_H_
#define _ADVANCED_SOLVER_H_

#include "poker_hand_evaluator.h"
#include "game_state.h"

class AdvancedSolver {
  const PokerHandEvaluator *evaluator;

  public:
    AdvancedSolver(const PokerHandEvaluator *evaluator);
    double solve(int iterations, const GameState &game_state) const;
    AdvancedSolver(const AdvancedSolver&) = delete;
    AdvancedSolver& operator=(AdvancedSolver&) = delete;
};
#endif
