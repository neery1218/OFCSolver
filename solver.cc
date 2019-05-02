#include "solver.h"
#include "deck.h"
#include <cassert>
#include <algorithm>
#include <numeric>
#include <boost/range/irange.hpp>
using namespace std;

Solver::Solver(const PokerHandEvaluator *_evaluator): evaluator{_evaluator} {}

unsigned int Solver::findCardsNeeded(const Hand &h) const {
  if (h.size() == 5) return 9; // not a typo
  if (h.size() == 7) return 9;
  if (h.size() == 9) return 6;
  if (h.size() == 11) return 3;
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

  for (auto &numCards : cardsNeeded) { cout << numCards << " needed for hand." << endl; }
  int totalCardsNeeded = accumulate(cardsNeeded.begin(), cardsNeeded.end(), 0);

  // average hand values over all iterations
  int total = 0;
  for (int i : boost::irange(1, numIterations)) {
    // sample cards from deck
    vector<Card> drawnCards = deck.select(totalCardsNeeded);

    // add cards to each hand
    unsigned int counter = cardsNeeded[0];

    set<Card> cards(drawnCards.begin(), drawnCards.begin() + cardsNeeded[0]);
    CompletedHand myOptimalHand = myHand.constructOptimalHand(cards, evaluator);

    if (allHands.size() == 1) total += myOptimalHand.calculatePoints();
    else for (int i = 1; i < allHands.size(); ++i) {
      set<Card> cards(
          drawnCards.begin() + counter,
          drawnCards.begin() + counter + cardsNeeded[i]);
      counter += cardsNeeded[i];
      CompletedHand otherHand = allHands[i].constructOptimalHand(cards, evaluator);
      total += myOptimalHand.calculatePoints(otherHand);
    }
  }

  return total * 1.0 / numIterations;
}
