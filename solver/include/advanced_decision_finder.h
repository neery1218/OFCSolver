#ifndef _ADVANCED_DECISION_FINDER
#define _ADVANCED_DECISION_FINDER

#include "fast_poker_hand_evaluator.h"
#include "gametype.h"
#include "game_state.h"
#include "pull.h"
#include "solver.h"
#include <vector>
#include "hand.h"
#include "decision.h"
#include "card.h"
#include "solver.h"
#include "advanced_solver.h"

class AdvancedDecisionFinder {
  const FastPokerHandEvaluator *evaluator;
  public:
    AdvancedDecisionFinder(const FastPokerHandEvaluator *evaluator);
    AdvancedDecisionFinder(const AdvancedDecisionFinder&) = delete;
    AdvancedDecisionFinder& operator=(AdvancedDecisionFinder&) = delete;

    Decision findBestDecision(const GameState &game_state);
  private:
    SolverParams getSolverParams(const GameState &game_state) const;
    std::vector<Decision> stageOneEvaluation(const std::vector<Decision> &all_decisions, unsigned int n, const GameState &game_state, const SolverParams &solver_params);
    Decision stageTwoEvaluation(const std::vector<Decision> &all_decisions, const GameState &game_state, const SolverParams &solver_params);
};

#endif
