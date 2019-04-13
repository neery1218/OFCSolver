#include "deck.h"
#include <iterator>
#include <random>
#include <experimental/algorithm>

using namespace std;

Deck::Deck() {
  for (int i = 0; i < 52; i++) {
    deck.insert(i);
  }
}

void Deck::remove(Card card) {
  deck.erase(card.getVal());
}

void Deck::remove(int val) {
  deck.erase(val);
}

int Deck::size() {
  return deck.size();
}

const vector<Card> Deck::select(int k) {
  vector<int> tmp;
  vector<Card> out;
  experimental::sample(
      deck.begin(), deck.end(), 
      back_inserter(tmp), k, 
      std::mt19937{std::random_device{}()});
  transform(
      tmp.begin(), tmp.end(), back_inserter(out), 
      [] (int val) { return Card(val); });
  return out;
}
