#ifndef HAND_H
#define HAND_H
#include <string>
#include <vector>
#include <map>
#include "card.h"

class Hand {
  std::vector<Card> top;
  std::vector<Card> middle;
  std::vector<Card> bottom;

  public:
    Hand(std::vector<Card> top, std::vector<Card> middle, std::vector<Card> bottom);
    void addTop(Card card);
    void addMiddle(Card card);
    void addBottom(Card card);
    friend std::ostream& operator<<(std::ostream& os, const Hand& c);
  private:
    static std::map<char, int> suitToVal;
    static std::map<char, int> rankToVal;
};

#endif
