#include "fantasy_simulator.h"
#include "poker_hand_evaluator.h"
#include "hand.h"
#include "card.h"
#include "deck.h"
#include <vector>
#include <iostream>
#include <boost/range/irange.hpp>
#include <future>

using namespace std;

FantasySimulator::FantasySimulator(): evaluator{PokerHandEvaluator(GameType::nobonus)} {}

double FantasySimulator::run(int numCards, int numIterations) {
  Deck deck;
  double total = 0;
  int numRefantasy = 0;

  for (int i : boost::irange(1, numIterations)){
    if (i % 100 == 0) cout << i << "\n";
    vector<Card> cards = deck.select(numCards);
    set<Card> cardSet(cards.begin(), cards.end());

    CompletedHand myOptimalHand = Hand().constructOptimalHand(cardSet, &evaluator);
    double sum = myOptimalHand.topInfo->royalties + myOptimalHand.middleInfo->royalties + myOptimalHand.bottomInfo->royalties;
    if (myOptimalHand.topInfo->handType == PokerHandType::TRIPS ||
        myOptimalHand.bottomInfo->handType == PokerHandType::QUADS ||
        myOptimalHand.bottomInfo->handType == PokerHandType::STRAIGHT_FLUSH ||
        myOptimalHand.bottomInfo->handType == PokerHandType::ROYAL_FLUSH) ++numRefantasy;

    total += sum;

  }

  cout << "Total: " << total << "\n"; 
  cout << "Num Refantasy: " << numRefantasy << "\n";
  return (total / numIterations) / (1 - (numRefantasy * 1.0 / numIterations));

}

int main () {
  FantasySimulator simulator;


  vector<future<double>> l;
  for (int i : boost::irange(1, 2)) {
    l.push_back(async(simulator.run, 14, 500));
  }

  double out = 0;
  for (int i : boost::irange(1, 2)) {
    int tmp = l[i].get();
    out += tmp;
  }

  int avg = out / l.size();
  cout << avg << endl;
}
