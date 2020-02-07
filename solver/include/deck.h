#ifndef _DECK_H_
#define _DECK_H_
#include "card.h"
#include "game_state.h"
#include <random>
#include <set>
#include <vector>

class Deck {
  std::set<Card> deck;
  std::mt19937 e;

  public:
  Deck();
  Deck(const GameState& state);
  Deck(const Deck& d);
  void remove(Card card);
  void remove(const std::vector<Card>& cards);
  void remove(const std::set<Card>& cards);
  const std::vector<Card> select(int k);
  int size();
};

#endif
