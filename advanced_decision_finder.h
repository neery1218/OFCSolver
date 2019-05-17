#ifndef _ADVANCED_DECISION_FINDER
#define _ADVANCED_DECISION_FINDER

#include "poker_hand_evaluator.h"
#include "gametype.h"
#include "game_state.h"
#include "pull.h"
#include "solver.h"
#include <vector>
#include <set>
#include "hand.h"
#include "decision.h"
#include "card.h"
#include "solver.h"
#include "advanced_solver.h"

class AdvancedDecisionFinder {
  const PokerHandEvaluator *evaluator;
  public:
    AdvancedDecisionFinder(const PokerHandEvaluator *evaluator);
    Decision findBestDecision(const GameState &game_state);
    AdvancedDecisionFinder(const AdvancedDecisionFinder&) = delete;
    AdvancedDecisionFinder& operator=(AdvancedDecisionFinder&) = delete;

  private:
    SolverParams getSolverParams(const GameState &game_state) const;
    std::vector<Decision> stageOneEvaluation(const std::vector<Decision> &all_decisions, unsigned int n, const GameState &game_state, int num_iterations);
    Decision stageTwoEvaluation(const std::vector<Decision> &all_decisions, const GameState &game_state, int num_iterations);

    std::vector<Decision> findAllSetDecisions(const GameState &game_state);
    std::vector<Decision> findAllSetDecisionsHelper(const std::set<Card> &cards, const std::vector<Placement> &acc);

    std::vector<Decision> findAllDrawDecisions(const GameState &game_state);
};

#endif
