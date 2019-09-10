#ifndef _HAND_H_
#define _HAND_H_

#include <string>
#include <vector>
#include <map>
#include "card.h"
#include "decision.h"
#include "poker_hand_evaluator.h"

struct Decision;
struct CompletedHand;

struct Hand {
  std::vector<Card> top;
  std::vector<Card> middle;
  std::vector<Card> bottom;
  int _size;

  Hand(std::vector<Card> top, std::vector<Card> middle, std::vector<Card> bottom);
  Hand(const Hand &obj);
  Hand();

  void addTop(Card card);
  void addMiddle(Card card);
  void addBottom(Card card);

  int size() const;
  Hand applyDecision(Decision decision) const;
  CompletedHand constructOptimalHand(std::vector<Card> &cards, const PokerHandEvaluator *evaluator) const;

  friend std::ostream& operator<<(std::ostream& os, const Hand& c);
};

struct CompletedHand {
  PokerHandInfo topInfo;
  PokerHandInfo middleInfo;
  PokerHandInfo bottomInfo;
  
  CompletedHand() {};
  CompletedHand(const Hand &h, const PokerHandEvaluator *evaluator);
  CompletedHand(const PokerHandInfo &top, const PokerHandInfo &mid, const PokerHandInfo &bot): topInfo{top}, middleInfo{mid}, bottomInfo{bot} {}
  int calculatePoints(const CompletedHand &other) const;
  int calculatePoints() const;
};
#endif
