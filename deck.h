#ifndef _DECK_H_
#define _DECK_H_

#include <unordered_set>
#include <vector>
#include "card.h"

class Deck {
  std::unordered_set<int> deck;

  public:
    Deck();
    void remove(Card card);
    void remove(int val);
    const std::vector<Card> select(int k);
    int size();
};

#endif
