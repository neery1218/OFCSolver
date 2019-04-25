#include "solver.h"
#include "deck.h"
#include <cassert>
#include <algorithm>
#include <numeric>
#include <boost/range/irange.hpp>
using namespace std;

Solver::Solver(PokerHandEvaluator &_evaluator): evaluator{&_evaluator} {}

double Solver::solve(int numIterations, Hand &myHand, const Pull &myPull, vector<Hand> &otherHands, 
    vector<Card> &deadCards) {

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
  cout << "Deck now has " << deck.size() << " cards." << endl;

  // Create allHands structure for simplicity. 
  // myHand is inserted first and that is an invariant.
  vector<Hand> allHands;
  allHands.push_back(myHand);
  allHands.insert(allHands.end(), otherHands.begin(), otherHands.end());
  assert(allHands.size() == 1 + otherHands.size());

  // how many cards does each hand need?
  vector<int> cardsNeeded;
  transform(allHands.begin(), allHands.end(), back_inserter(cardsNeeded), 
      [] (Hand h) { return (13 - h.size()) / 2 * 3; });

  cout << "Size of allHands: " << allHands.size() << endl;
  for (auto &numCards : cardsNeeded) { cout << numCards << " needed for hand." << endl; }
  int totalCardsNeeded = accumulate(cardsNeeded.begin(), cardsNeeded.end(), 0);

  cout << "Total cards needed: " << totalCardsNeeded << endl;

  // average hand values over all iterations
  int total = 0;
  for (int i : boost::irange(1, numIterations)) {
    // sample cards from deck
    vector<Card> drawnCards = deck.select(totalCardsNeeded);

    // add cards to each hand
    unsigned int counter = cardsNeeded[0];

    set<Card> cards(drawnCards.begin(), drawnCards.begin() + cardsNeeded[0]);
    CompletedHand myOptimalHand = myHand.constructOptimalHand(cards, evaluator);

    for (int i = 1; i < allHands.size(); ++i) {
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
