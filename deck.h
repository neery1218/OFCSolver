#ifndef _DECK_H_
#define _DECK_H_

#include <unordered_set>
#include <set>
#include <vector>
#include "card.h"
#include <string>

class Deck {
  std::unordered_set<std::string> deck;

  public:
    Deck();
    void remove(Card card);
    void remove(const std::vector<Card> &cards);
    void remove(const std::set<Card> &cards);
    const std::set<Card> select(int k);
    int size();
};

#endif
