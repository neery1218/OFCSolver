#ifndef _POKER_HAND_EVALUATOR_
#define _POKER_HAND_EVALUATOR_

#include <vector>
#include <set>
#include <utility>
#include "card.h"
#include "gametype.h"
#include "position.h"
#include "poker_hand_info.h"

class PokerHandEvaluator {
  GameType gameType;

  public: 
    PokerHandEvaluator(GameType type);
    PokerHandEvaluator(const PokerHandEvaluator&) = delete;
    PokerHandEvaluator& operator=(PokerHandEvaluator&) = delete;
    ~PokerHandEvaluator();
    std::vector<std::pair<std::string, PokerHandInfo>> getTopInfos();
    std::vector<std::pair<std::string, PokerHandInfo>> getMiddleInfos();
    std::vector<std::pair<std::string, PokerHandInfo>> getBottomInfos();

    std::vector<std::pair<std::string, PokerHandInfo>> top_eval_info;
    std::vector<std::pair<std::string, PokerHandInfo>> middle_eval_info;
    std::vector<std::pair<std::string, PokerHandInfo>> bottom_eval_info;

  private:
    unsigned int calculateFantasyBonus(unsigned int overallRank) const;
};

#endif
