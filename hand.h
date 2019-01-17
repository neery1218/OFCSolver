#ifndef HAND_H
#define HAND_H
#include <string>
#include <vector>
#include <map>
#include "card.h"

struct Hand {
  std::vector<Card> top;
  std::vector<Card> middle;
  std::vector<Card> bottom;
  int _size;

  Hand(std::vector<Card> top, std::vector<Card> middle, std::vector<Card> bottom);
  void addTop(Card card);
  void addMiddle(Card card);
  void addBottom(Card card);
  int size();
  friend std::ostream& operator<<(std::ostream& os, const Hand& c);
};

#endif
