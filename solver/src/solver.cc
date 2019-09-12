#include "solver.h"
#include "deck.h"
#include <cassert>
#include <algorithm>
#include <numeric>
#include <boost/range/irange.hpp>
#include <stdexcept>
using namespace std;

Solver::Solver(const FastPokerHandEvaluator *_evaluator): evaluator{_evaluator} {}

unsigned int Solver::findCardsNeeded(const Hand &h) const {
  if (h.size() == 5) return 9; // not a typo
  if (h.size() == 7) return 7;
  if (h.size() == 9) return 5;
  if (h.size() == 11) return 3; 
  if (h.size() == 13) return 1;
  throw runtime_error("Hand size is not valid!");
}

double Solver::solve(int numIterations, const Hand &myHand, const Pull &myPull, const vector<Hand> &otherHands, const vector<Card> &deadCards) const {

  // Create deck, and remove known cards.
  Deck deck;
  for (auto &card : myPull.cards) { deck.remove(card); }
  for (auto &hand: otherHands) {
    for (auto &card : hand.top) { deck.remove(card); }
    for (auto &card : hand.middle) { deck.remove(card); }
    for (auto &card : hand.bottom) { deck.remove(card); }
  }
  for (auto &card : myHand.top) { deck.remove(card); }
  for (auto &card : myHand.middle) { deck.remove(card); }
  for (auto &card : myHand.bottom) { deck.remove(card); }
  for (auto &card : deadCards) { deck.remove(card); }

  // Create allHands structure for simplicity. 
  // myHand is inserted first and that is an invariant.
  vector<Hand> allHands;
  allHands.push_back(myHand);
  allHands.insert(allHands.end(), otherHands.begin(), otherHands.end());
  assert(allHands.size() == 1 + otherHands.size());

  // how many cards does each hand need?
  vector<unsigned int> cardsNeeded;
  for (const auto &h : allHands) { cardsNeeded.push_back(findCardsNeeded(h)); }

  // average hand values over all iterations
  int total = 0;
  for (int it = 0; it < numIterations; ++it) {
    // sample cards from deck
    vector<Card> my_draw = deck.select(cardsNeeded[0]);
    set<Card> my_draw_set(my_draw.begin(), my_draw.end());
    CompletedHand myOptimalHand = myHand.constructOptimalHand(my_draw_set, evaluator);

    if (allHands.size() == 1) total += myOptimalHand.calculatePoints();
    else for (unsigned int i = 1; i < allHands.size(); ++i) {
      vector<Card> other_draw = deck.select(cardsNeeded[i]);
      set<Card> other_draw_set(other_draw.begin(), other_draw.end());
      CompletedHand otherHand = allHands[i].constructOptimalHand(other_draw_set, evaluator);
      total += myOptimalHand.calculatePoints(otherHand);
    }
  }

  return total * 1.0 / numIterations;
}
