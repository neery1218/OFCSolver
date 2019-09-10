#ifndef _DECK_H_
#define _DECK_H_
#include <set>
#include <vector>
#include "card.h"
#include "game_state.h"

class Deck {
  std::set<Card> deck;

  public:
    Deck();
    Deck(const GameState &state);
    Deck(const Deck &d);
    void remove(Card card);
    void remove(const std::vector<Card> &cards);
    void remove(const std::set<Card> &cards);
    const std::vector<Card> select(int k);
    int size();
};

#endif
