#include "deck.h"
#include <iterator>
#include <random>
#include <experimental/algorithm>

using namespace std;

Deck::Deck(): deck{{"2d", "2c", "2h", "2s", "3d", "3c", "3h", "3s", "4d", "4c", "4h", "4s", "5d", "5c", "5h", "5s", "6d", "6c", "6h", "6s", "7d", "7c", "7h", "7s", "8d", "8c", "8h", "8s", "9d", "9c", "9h", "9s", "Td", "Tc", "Th", "Ts", "Jd", "Jc", "Jh", 
  "Js", "Qd", "Qc", "Qh", "Qs", "Kd", "Kc", "Kh", "Ks", "Ad", "Ac", "Ah", "As"}} {}

void Deck::remove(Card card) {
  deck.erase(card.val);
}

int Deck::size() {
  return deck.size();
}

const vector<Card> Deck::select(int k) {
  vector<string> tmp;
  vector<Card> out;
  experimental::sample(
      deck.begin(), deck.end(), 
      back_inserter(tmp), k, 
      std::mt19937{std::random_device{}()});
  transform(
      tmp.begin(), tmp.end(), back_inserter(out), 
      [] (string val) { return Card(val); });
  return out;
}
