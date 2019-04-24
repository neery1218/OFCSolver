#ifndef _DECISION_FINDER_
#define _DECISION_FINDER_

#include "decision.h"
#include "pull.h"
#include "hand.h"
#include "gametype.h"
#include "solver.h"
#include "poker_hand_evaluator.h"
#include <vector>

/* Makes all draw decisions */
class DecisionFinder {
  PokerHandEvaluator evaluator;
  public:
    DecisionFinder(GameType _type);

    Decision findBestDecision(const Hand &h, const Pull &myPull, std::vector<Hand> &otherHands, std::vector<Card> &deadCards);
  private:
    std::vector<Decision> findAllDecisions(const Hand &h, const Pull &myPull);
    int findIterationsRequired(const Hand &h);
};

#endif
