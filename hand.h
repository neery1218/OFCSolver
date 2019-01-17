#ifndef HAND_H
#define HAND_H
#include <string>
#include <vector>
#include <map>
#include "card.h"

struct Hand {
  enum Position { // TODO: add Placement::Position and then remove dead from here.
    Top = 0,
    Middle = 1,
    Bottom = 2,
    Dead = 3
  };

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

std::vector<std::vector<int>> toIntegerRepresentation (Hand &h);
#endif
