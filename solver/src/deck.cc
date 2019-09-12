#include "deck.h"
#include <iterator>
#include <random>
#include <experimental/algorithm>
#include <cassert>

using namespace std;

Deck::Deck() {
  std::string suits[] {"d", "c", "h", "s"};
  std::string ranks[] {"2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"};
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 13; ++j) {
      std::string card = "";
      card += ranks[j];
      card += suits[i];
      deck.insert(CardUtils::parseCard(card));
    }
  }
}
Deck::Deck(const Deck& d): deck{d.deck} {}
Deck::Deck(const GameState &game_state) {
  std::string suits[4] {"d", "c", "h", "s"};
  std::string ranks[13] {"2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"};
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 13; ++j) {
      std::string card = "";
      card += ranks[j];
      card += suits[i];
      deck.insert(CardUtils::parseCard(card));
    }
  }

  remove(game_state.my_hand.top);
  remove(game_state.my_hand.middle);
  remove(game_state.my_hand.bottom);

  for (auto &h : game_state.other_hands) {
    remove(h.top);
    remove(h.middle);
    remove(h.bottom);
  }

  remove(game_state.my_pull.cards);
  remove(game_state.dead_cards);
}

void Deck::remove(Card card) {
  deck.erase(card);
}

void Deck::remove(const vector<Card> &cards) {
  for (auto &c : cards) { remove(c); }
}

void Deck::remove(const set<Card> &cards) {
  for (auto &c : cards) { remove(c); }
}

int Deck::size() {
  return deck.size();
}

const vector<Card> Deck::select(int k) {
  vector<Card> cards;
  experimental::sample(
      deck.begin(), deck.end(), 
      back_inserter(cards), k, 
      std::mt19937{std::random_device{}()});
  return cards;
}
