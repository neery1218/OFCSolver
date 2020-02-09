#ifndef _ADVANCED_DECISION_FINDER
#define _ADVANCED_DECISION_FINDER

#include "advanced_solver.h"
#include "card.h"
#include "decision.h"
#include "fast_poker_hand_evaluator.h"
#include "game_state.h"
#include "gametype.h"
#include "hand.h"
#include "pull.h"
#include "solver.h"
#include <vector>

class AdvancedDecisionFinder {
  const FastPokerHandEvaluator* evaluator;
  std::random_device rd;

  public:
  AdvancedDecisionFinder(const FastPokerHandEvaluator* evaluator);
  AdvancedDecisionFinder(const AdvancedDecisionFinder&) = delete;
  AdvancedDecisionFinder& operator=(AdvancedDecisionFinder&) = delete;

  Decision findBestDecision(const GameState& game_state);

  private:
  SolverParams getSolverParams(const GameState& game_state) const;
  std::vector<Decision> stageOneEvaluation(const std::vector<Decision>& all_decisions, unsigned int n, const GameState& game_state, const SolverParams& solver_params);
  Decision stageTwoEvaluation(const std::vector<Decision>& all_decisions, const GameState& game_state, const SolverParams& solver_params);
};

#endif
