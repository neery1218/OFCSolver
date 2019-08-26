#include "deck.h"
#include <iterator>
#include <random>
#include <experimental/algorithm>
#include <cassert>

using namespace std;

Deck::Deck(): deck{{"2d", "2c", "2h", "2s", "3d", "3c", "3h", "3s", "4d", "4c", "4h", "4s", "5d", "5c", "5h", "5s", "6d", "6c", "6h", "6s", "7d", "7c", "7h", "7s", "8d", "8c", "8h", "8s", "9d", "9c", "9h", "9s", "Td", "Tc", "Th", "Ts", "Jd", "Jc", "Jh", 
  "Js", "Qd", "Qc", "Qh", "Qs", "Kd", "Kc", "Kh", "Ks", "Ad", "Ac", "Ah", "As"}} { }

Deck::Deck(const Deck& d): deck{d.deck} {}

Deck::Deck(const GameState &game_state): deck{{"2d", "2c", "2h", "2s", "3d", "3c", "3h", "3s", "4d", "4c", "4h", "4s", "5d", "5c", "5h", "5s", "6d", "6c", "6h", "6s", "7d", "7c", "7h", "7s", "8d", "8c", "8h", "8s", "9d", "9c", "9h", "9s", "Td", "Tc", "Th", "Ts", "Jd", "Jc", "Jh", 
  "Js", "Qd", "Qc", "Qh", "Qs", "Kd", "Kc", "Kh", "Ks", "Ad", "Ac", "Ah", "As"}} {

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
  deck.erase(card.val);
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

const set<Card> Deck::select(int k) {
  vector<string> tmp;
  set<Card> out;
  experimental::sample(
      deck.begin(), deck.end(), 
      back_inserter(tmp), k, 
      std::mt19937{std::random_device{}()});
  transform(
      tmp.begin(), tmp.end(), inserter(out, out.begin()), 
      [] (string val) { return Card(val); });
  return out;
}
