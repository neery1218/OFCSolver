#include "poker_hand_evaluator.h"
#include "hand.h"
#include "card.h"
#include "deck.h"
#include "game_state.h"
#include "pull.h"
#include "decision_finder.h"
#include <vector>
#include <iostream>
#include <boost/range/irange.hpp>
#include <future>

using namespace std;

CompletedHand complete(const Hand&h, const PokerHandEvaluator *eval) {
  PokerHandInfo *topInfo = eval->eval(h.top, Position::top);
  PokerHandInfo *midInfo = eval->eval(h.middle, Position::middle);
  PokerHandInfo *botInfo = eval->eval(h.bottom, Position::bottom);

  return CompletedHand{*topInfo, *midInfo, *botInfo};
}

int main () {
  int iterations = 10;
  int fantasy_n = 14;
  // each iteration:
  // make a fantasy hand
  // make a regular hand
  // calculate ev
  PokerHandEvaluator *eval_regular = new PokerHandEvaluator(GameType::Regular);

  double total = 0;
  for (int i = 0; i < iterations; ++i) {

    bool refantasy = false;
    do {
      refantasy = false;
      Deck deck;

      // pre generate pulls
      vector< set<Card> > pulls;
      pulls.push_back(deck.select(5));
      deck.remove(pulls[0]);

      for (int i = 1; i <= 4; ++i) {
        pulls.push_back(deck.select(3));
        deck.remove(pulls[i]);
      }

      // make hand
      vector<Hand> hands;
      hands.emplace_back(Hand());
      vector<Card> dead_cards;

      for (int i = 0; i < 5; ++i) {
        GameState state{hands[i], vector<Hand> (), Pull{pulls[i]}, dead_cards};
        for (Card c : pulls[i]) { dead_cards.push_back(c); }
        Decision d = DecisionFinder(eval_regular).findBestDecision(state);
        hands.push_back(hands[i].applyDecision(d));
      }


      set<Card> fantasy_pull = deck.select(14);
      CompletedHand fantasy_hand = Hand().constructOptimalHand(fantasy_pull, eval_regular);

      // create CompletedHand with hands[4]
      CompletedHand my_regular_hand = complete(hands[4], eval_regular);

      double ev = fantasy_hand.calculatePoints(my_regular_hand);
      total += ev;
      cout << "My regular hand: " << hands[4] << "\n";

      if (fantasy_hand.topInfo.handType == PokerHandType::TRIPS ||
          fantasy_hand.bottomInfo.handType == PokerHandType::QUADS ||
          fantasy_hand.bottomInfo.handType == PokerHandType::STRAIGHT_FLUSH ||
          fantasy_hand.bottomInfo.handType == PokerHandType::ROYAL_FLUSH) refantasy = true;
    }
    while (refantasy);
  }
  double avg = total / iterations;
  cout << "Fantasy ev is : " << avg << "\n";
}
