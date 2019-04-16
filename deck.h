#ifndef _DECK_H_
#define _DECK_H_

#include <unordered_set>
#include <vector>
#include "card.h"
#include <string>

class Deck {
  std::unordered_set<std::string> deck;

  public:
    Deck();
    void remove(Card card);
    const std::vector<Card> select(int k);
    int size();
};

#endif
