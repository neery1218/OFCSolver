#pragma once

#include "fast_poker_hand_evaluator.h"
#include "hand.h"
#include "poker_hand_info.h"

struct CompletedHand {
  PokerHandInfo topInfo;
  PokerHandInfo middleInfo;
  PokerHandInfo bottomInfo;

  CompletedHand() {};
  CompletedHand(const Hand& h, const FastPokerHandEvaluator* evaluator);
  CompletedHand(const PokerHandInfo top, const PokerHandInfo mid, const PokerHandInfo bot)
      : topInfo { top }
      , middleInfo { mid }
      , bottomInfo { bot }
  {
  }
  int calculatePoints(const CompletedHand& other) const;
  int calculatePoints() const;
};
