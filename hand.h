#ifndef _HAND_H_
#define _HAND_H_

#include <string>
#include <vector>
#include <map>
#include "card.h"
#include "decision.h"
#include "poker_hand_evaluator.h"

class Decision;
struct CompletedHand;

struct Hand {
  std::vector<Card> top;
  std::vector<Card> middle;
  std::vector<Card> bottom;
  int _size;

  Hand(std::vector<Card> top, std::vector<Card> middle, std::vector<Card> bottom);
  Hand(const Hand &obj);
  Hand() {}

  void addTop(Card card);
  void addMiddle(Card card);
  void addBottom(Card card);

  int size();
  Hand applyDecision(Decision decision);
  CompletedHand constructOptimalHand(std::vector<Card> &cards, PokerHandEvaluator * pokerHandEvaluator);

  friend std::ostream& operator<<(std::ostream& os, const Hand& c);
};

struct CompletedHand {
  Hand h;
  PokerHandInfo topInfo;
  PokerHandInfo middleInfo;
  PokerHandInfo bottomInfo;
  
  CompletedHand() {};
  CompletedHand(Hand _h, PokerHandInfo top, PokerHandInfo mid, PokerHandInfo bot): h{_h}, topInfo{top}, middleInfo{mid}, bottomInfo{bot} {}
  int calculatePoints(const CompletedHand &other);
};
#endif
