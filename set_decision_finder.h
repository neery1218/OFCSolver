#ifndef _SET_DECISION_FINDER
#define _SET_DECISION_FINDER

#include "poker_hand_evaluator.h"
#include "gametype.h"
#include "pull.h"
#include <vector>
#include <set>
#include "hand.h"
#include "decision.h"
#include "card.h"

class SetDecisionFinder {
  const PokerHandEvaluator *evaluator;
  public:
    SetDecisionFinder(const PokerHandEvaluator &evaluator);
    Decision findBestDecision(const Pull &myPull, const std::vector<Hand> &otherHands);

  private:
    std::vector<Decision> stageOneEvaluation(const std::vector<Decision> &all_decisions, int n, const Pull &my_pull,
        int num_iterations);
    Decision stageTwoEvaluation(const std::vector<Decision> &all_decisions, const Pull &my_pull,
        int num_iterations, const std::vector<Hand> &other_hands);

    std::vector<Decision> findAllDecisions(std::set<Card> cards);
    std::vector<Decision> findAllDecisionsHelper(const std::set<Card> &cards, const std::vector<Placement> &acc);
};

#endif
