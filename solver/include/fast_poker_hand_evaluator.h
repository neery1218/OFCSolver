#ifndef _FAST_POKER_HAND_EVALUATOR_
#define _FAST_POKER_HAND_EVALUATOR_

#include "gametype.h"
#include <vector>
#include "card.h"
#include "position.h"
#include "poker_hand_info.h"


class FastPokerHandEvaluator {
  GameType game_type;

  public:
    FastPokerHandEvaluator(GameType type);

    FastPokerHandEvaluator(const FastPokerHandEvaluator&) = delete; // explicit delete
    FastPokerHandEvaluator& operator=(FastPokerHandEvaluator&) = delete; // explicit delete

    ~FastPokerHandEvaluator();

    // evaluation functions. called in Hand::constructOptimalHand
    PokerHandInfo evalTop(const std::vector<Card> &completed_row) const;
    PokerHandInfo evalMiddle(const std::vector<Card> &completed_row) const;
    PokerHandInfo evalBottom(const std::vector<Card> &completed_row) const;

  private:
    unsigned int calculateFantasyBonus(unsigned int overallRank) const;

    std::vector<PokerHandInfo> top_eval_info;
    std::vector<PokerHandInfo> middle_eval_info;
    std::vector<PokerHandInfo> bottom_eval_info;
    std::vector<PokerHandInfo> flush_middle_eval_info;
    std::vector<PokerHandInfo> flush_bottom_eval_info;
};

#endif
