#include "solver.h"
#include "deck.h"
#include <cassert>
#include <algorithm>
using namespace std;

Solver::Solver(Method _method): method{_method} {}

Decision Solver::solve(Hand &myHand, Pull &myPull, vector<Hand> otherHands, 
    vector<Decision> decisions, GameType type, vector<Card> deadCards) {
  Deck deck;
  vector<Hand> hands;
  hands.push_back(myHand);
  hands.insert(hands.end(), otherHands.begin(), otherHands.end());

  assert(hands.size() == 1 + otherHands.size());

  // remove known cards from deck
  for (auto &card : myPull.cards) { deck.remove(card); }
  for (auto &hand: hands) {
    for (auto &card : hand.top) { deck.remove(card); }
    for (auto &card : hand.middle) { deck.remove(card); }
    for (auto &card : hand.bottom) { deck.remove(card); }
  }
  for (auto &card : deadCards) { deck.remove(card); }

  cout << "Deck now has " << deck.size() << " cards." << endl;

  // how many cards does each hand need?
  vector<int> cardsNeeded;
  for (auto &hand : hands) {
    int cards = (13 - hand.size()) / 2 * 3;
    cardsNeeded.push_back(cards);
    cout << cards << " needed for hand." << endl;
  }

  int totalCardsNeeded = accumulate(cardsNeeded.begin(), cardsNeeded.end(), 0);
  cout << totalCardsNeeded << " total cards needed." << endl;

  // average hand values over all iterations
  for (int i = 0; i < 10000; i++) {
    // sample cards from deck
    vector<int> drawnCards = deck.select(totalCardsNeeded);
    // add cards to each hand
    // estimate ev
  }

  return decisions[0];
}

ostream& operator<<(ostream& os, const Placement &p) {
  string pos = "top";
  if (p.position == Hand::Position::Middle) { pos = "middle"; }
  if (p.position == Hand::Position::Bottom) { pos = "bottom"; }
  if (p.position == Hand::Position::Dead) { pos = "dead"; }

  os << p.card << " " << pos;
  return os;
}

ostream& operator<<(ostream& os, const Decision &d) {
  os << d.placements.at(0) << endl << d.placements.at(1)  << endl << d.placements.at(2);
  return os;
}
