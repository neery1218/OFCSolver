#ifndef _HAND_H_
#define _HAND_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include "card.h"
#include "decision.h"
#include "poker_hand_evaluator.h"

class Decision;
struct CompletedHand;

struct Hand {
  std::set<Card> top;
  std::set<Card> middle;
  std::set<Card> bottom;
  int _size;

  Hand(std::set<Card> top, std::set<Card> middle, std::set<Card> bottom);
  Hand(const Hand &obj);
  Hand();

  void addTop(Card card);
  void addMiddle(Card card);
  void addBottom(Card card);

  int size() const;
  Hand applyDecision(Decision decision) const;
  CompletedHand constructOptimalHand(std::set<Card> &cards, const PokerHandEvaluator *evaluator) const;

  friend std::ostream& operator<<(std::ostream& os, const Hand& c);
};

struct CompletedHand {
  PokerHandInfo topInfo;
  PokerHandInfo middleInfo;
  PokerHandInfo bottomInfo;
  
  CompletedHand() {};
  CompletedHand(PokerHandInfo top, PokerHandInfo mid, PokerHandInfo bot): topInfo{top}, middleInfo{mid}, bottomInfo{bot} {}
  int calculatePoints(const CompletedHand &other) const;
  int calculatePoints() const;
};
#endif
