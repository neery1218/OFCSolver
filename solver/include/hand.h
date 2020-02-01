#ifndef _HAND_H_
#define _HAND_H_

#include "card.h"
#include "decision.h"
#include "fast_poker_hand_evaluator.h"
#include <map>
#include <set>
#include <string>
#include <vector>

struct Decision;
struct CompletedHand;

struct Hand {
  std::vector<Card> top;
  std::vector<Card> middle;
  std::vector<Card> bottom;
  int _size;

  Hand(std::vector<Card> top, std::vector<Card> middle, std::vector<Card> bottom);
  Hand(const Hand& obj);
  Hand();

  void addTop(Card card);
  void addMiddle(Card card);
  void addBottom(Card card);

  int size() const;
  Hand applyDecision(Decision decision) const;
  CompletedHand constructOptimalHand(std::set<Card>& cards, const FastPokerHandEvaluator* evaluator) const;

  friend std::ostream& operator<<(std::ostream& os, const Hand& c);
};

#endif
