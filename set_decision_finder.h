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
  PokerHandEvaluator evaluator;
  public:
    SetDecisionFinder(GameType _type);
    Decision findBestDecision(const Pull &myPull, const std::vector<Hand> &otherHands);

  private:
    std::vector<Decision> findAllDecisions(std::set<Card> cards);
    std::vector<Decision> findAllDecisionsHelper(const std::set<Card> &cards, const std::vector<Placement> &acc);
};

#endif
