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
  int iterations = 10;
  int fantasy_n = 14;
  // each iteration:
  // make a fantasy hand
  // make a regular hand
  // calculate ev
  PokerHandEvaluator *eval_regular = new PokerHandEvaluator(GameType::Regular);

  double total = 0;
  for (int i : boost::irange(iterations)) {

    Deck deck;

    // pre generate pulls
    vector< vector<Card> > pulls;
    pulls[0] = deck.select(5);
    deck.remove(pulls[0]);

    for (int i = 1; i <= 4; ++i) {
      pulls[i] = deck.select(3);
      deck.remove(pulls[i]);
    }

    // make hand
    vector<Hand> hands;
    hands.emplace_back(Hand());
    vector<Card> dead_cards;

    for (int i = 0; i < 5; ++i) {
      GameState state{hands[i], vector<Hand> (), pull[i], dead_cards};
      for (Card c : pull[i]) { dead_cards.push_back(c); }
      Decision d = DecisionFinder(eval_regular).findBestDecision(game_state);
      hands.push_back(hands[i].applyDecision(d);
    }


    vector<Card> fantasy_pull = deck.select(14);
    set<Card> card_set(fantasy_pull.begin(), fantasy_pull.end());
    CompletedHand myOptimalHand = Hand().constructOptimalHand(cardSet, &evaluator);

    myOptimalHand.calculatePoints
    double sum = myOptimalHand.topInfo->royalties + myOptimalHand.middleInfo->royalties + myOptimalHand.bottomInfo->royalties;
    if (myOptimalHand.topInfo->handType == PokerHandType::TRIPS ||
        myOptimalHand.bottomInfo->handType == PokerHandType::QUADS ||
        myOptimalHand.bottomInfo->handType == PokerHandType::STRAIGHT_FLUSH ||
        myOptimalHand.bottomInfo->handType == PokerHandType::ROYAL_FLUSH) ++numRefantasy;


    
  }
}
