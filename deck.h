#ifndef _DECK_H_
#define _DECK_H_

#include <set>
#include <vector>
#include "card.h"

class Deck {
  std::set<int> deck;

  public:
    Deck();
    void remove(Card card);
    void remove(int val);
    std::vector<int> select(int k);
    int size();
};

#endif
