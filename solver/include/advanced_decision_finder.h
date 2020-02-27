#ifndef _ADVANCED_DECISION_FINDER
#define _ADVANCED_DECISION_FINDER

#include <boost/asio.hpp>
#include <vector>

#include "advanced_solver.h"
#include "card.h"
#include "decision.h"
#include "fast_poker_hand_evaluator.h"
#include "game_state.h"
#include "gametype.h"
#include "hand.h"
#include "pull.h"
#include "solver.h"

class AdvancedDecisionFinder {
  const FastPokerHandEvaluator *evaluator;
  std::random_device rd;
  boost::asio::thread_pool pool;

 public:
  AdvancedDecisionFinder(const FastPokerHandEvaluator *evaluator);
  AdvancedDecisionFinder(const AdvancedDecisionFinder &) = delete;
  AdvancedDecisionFinder &operator=(AdvancedDecisionFinder &) = delete;

  std::vector<std::pair<double, Decision>> findBestDecision(
      const GameState &game_state);

 private:
  SolverParams getSolverParams(const GameState &game_state) const;
  std::vector<Decision> stageOneEvaluation(
      const std::vector<Decision> &all_decisions, unsigned int n,
      const GameState &game_state, const SolverParams &solver_params);
  std::vector<std::pair<double, Decision>> stageTwoEvaluation(
      const std::vector<Decision> &all_decisions, const GameState &game_state,
      const SolverParams &solver_params);
};

#endif
