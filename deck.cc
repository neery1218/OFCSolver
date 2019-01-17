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
  remove(card.getVal());
}

void Deck::remove(int val) {
  deck.erase(val);
}

int Deck::size() {
  return deck.size();
}

vector<int> Deck::select(int k) {
  vector<int> out;
  experimental::sample(
      deck.begin(), deck.end(), 
      back_inserter(out), k, 
      std::mt19937{std::random_device{}()});
  return out;
}
