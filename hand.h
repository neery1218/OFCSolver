#ifndef _HAND_H_
#define _HAND_H_

#include <string>
#include <vector>
#include <map>
#include "card.h"
#include "decision.h"

class Decision;

struct Hand {
  std::vector<Card> top;
  std::vector<Card> middle;
  std::vector<Card> bottom;
  int _size;

  Hand(std::vector<Card> top, std::vector<Card> middle, std::vector<Card> bottom);
  Hand(const Hand &obj);

  void addTop(Card card);
  void addMiddle(Card card);
  void addBottom(Card card);

  int size();
  Hand applyDecision(Decision decision);
  Hand constructOptimalHand(std::vector<Card> cards);
  int calculatePoints(std::vector<Hand> otherHands);
  int calculatePoints(Hand otherHand);

  friend std::ostream& operator<<(std::ostream& os, const Hand& c);
};
#endif
