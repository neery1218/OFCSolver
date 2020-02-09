#ifndef _DECISION_FINDER_
#define _DECISION_FINDER_

#include "card.h"
#include "decision.h"
#include "fast_poker_hand_evaluator.h"
#include "game_state.h"
#include "gametype.h"
#include "hand.h"
#include "pull.h"
#include <random>
#include <set>
#include <vector>

class DecisionFinder {
  const FastPokerHandEvaluator* evaluator;
  std::mt19937* rng;

  public:
  DecisionFinder(const FastPokerHandEvaluator* evaluator, std::mt19937* rng);
  Decision findBestDecision(const GameState& game_state, int iterations);
  DecisionFinder(const DecisionFinder&) = delete;
  DecisionFinder& operator=(DecisionFinder&) = delete;
};

#endif
