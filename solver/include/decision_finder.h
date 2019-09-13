#ifndef _DECISION_FINDER_
#define _DECISION_FINDER_

#include "fast_poker_hand_evaluator.h"
#include "gametype.h"
#include "game_state.h"
#include "pull.h"
#include <vector>
#include <set>
#include "hand.h"
#include "decision.h"
#include "card.h"

class DecisionFinder {
  const FastPokerHandEvaluator *evaluator;
  public:
    DecisionFinder(const FastPokerHandEvaluator *evaluator);
    Decision findBestDecision(const GameState &game_state);
    Decision findBestDecision(const GameState &game_state, int iterations);
    DecisionFinder(const DecisionFinder&) = delete;
    DecisionFinder& operator=(DecisionFinder&) = delete;

  private:
    std::vector<Decision> stageOneEvaluation(const std::vector<Decision> &all_decisions, unsigned int n, const GameState &game_state, int num_iterations);
    Decision stageTwoEvaluation(const std::vector<Decision> &all_decisions, const GameState &game_state, int num_iterations);
};

#endif
