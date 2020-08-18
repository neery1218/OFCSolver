#ifndef _ADVANCED_SOLVER_H_
#define _ADVANCED_SOLVER_H_

#include "decision_stats.h"
#include "deck.h"
#include "fast_poker_hand_evaluator.h"
#include "game_state.h"

struct SolverParams {
  unsigned int stage_one_iterations;
  unsigned int stage_two_iterations;
  unsigned int search_level;
};

/* If you parallelize this later, make sure to create new seeds for the child
 * threads. */
class AdvancedSolver {
  const FastPokerHandEvaluator* evaluator;
  std::mt19937 rng;

 public:
  AdvancedSolver(const FastPokerHandEvaluator* evaluator, uint32_t seed);
  DecisionStats solve(int iterations, const GameState& game_state,
                      const Deck& initial_deck, int search_level);
  AdvancedSolver(const AdvancedSolver&) = delete;
  AdvancedSolver& operator=(AdvancedSolver&) = delete;
};
#endif
