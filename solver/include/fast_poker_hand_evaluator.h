#ifndef _FAST_POKER_HAND_EVALUATOR_
#define _FAST_POKER_HAND_EVALUATOR_

#include <array>
#include <vector>

#include "card.h"
#include "gametype.h"
#include "poker_hand_info.h"
#include "position.h"

class FastPokerHandEvaluator {
  GameType game_type;
  char* filename;

 public:
  FastPokerHandEvaluator(GameType type, char* filename);

  FastPokerHandEvaluator(const FastPokerHandEvaluator&) =
      delete;  // explicit delete
  FastPokerHandEvaluator& operator=(FastPokerHandEvaluator&) =
      delete;  // explicit delete

  ~FastPokerHandEvaluator();

  // evaluation functions. called in Hand::constructOptimalHand
  PokerHandInfo evalTop(const std::vector<Card>& completed_row) const;
  PokerHandInfo evalMiddle(const std::vector<Card>& completed_row) const;
  PokerHandInfo evalBottom(const std::vector<Card>& completed_row) const;

 private:
  unsigned int calculateFantasyBonus(unsigned int overallRank) const;

  std::array<PokerHandInfo, 2197> top_eval_info;
  std::array<PokerHandInfo, 371293> middle_eval_info;
  std::array<PokerHandInfo, 371293> bottom_eval_info;
  std::array<PokerHandInfo, 371293> flush_middle_eval_info;
  std::array<PokerHandInfo, 371293> flush_bottom_eval_info;
};

#endif
