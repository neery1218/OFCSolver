#ifndef _SET_DECISION_FINDER
#define _SET_DECISION_FINDER

#include "poker_hand_evaluator.h"
#include "gametype.h"
#include "game_state.h"
#include "pull.h"
#include <vector>
#include <set>
#include "hand.h"
#include "decision.h"
#include "card.h"

class DecisionFinder {
  const PokerHandEvaluator *evaluator;
  public:
    DecisionFinder(const PokerHandEvaluator *evaluator);
    Decision findBestDecision(const GameState &game_state);
    DecisionFinder(const DecisionFinder&) = delete;
    DecisionFinder& operator=(DecisionFinder&) = delete;

  private:
    std::vector<Decision> stageOneEvaluation(const std::vector<Decision> &all_decisions, int n, const GameState &game_state,
        int num_iterations);
    Decision stageTwoEvaluation(const std::vector<Decision> &all_decisions, const GameState &game_state,
        int num_iterations);

    std::vector<Decision> findAllSetDecisions(const GameState &game_state);
    std::vector<Decision> findAllSetDecisionsHelper(const std::set<Card> &cards, const std::vector<Placement> &acc);

    std::vector<Decision> findAllDrawDecisions(const GameState &game_state);
};

#endif
