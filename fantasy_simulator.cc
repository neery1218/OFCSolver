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
  if (topInfo->overallRank > midInfo->overallRank || midInfo->overallRank > botInfo->overallRank) {
    cout << "brick\n";
    return CompletedHand{
      PokerHandInfo{-1, PokerHandType::HIGH_CARD, 0},
      PokerHandInfo{-1, PokerHandType::HIGH_CARD, 0},
      PokerHandInfo{-1, PokerHandType::HIGH_CARD, 0}};
  }

  return CompletedHand{*topInfo, *midInfo, *botInfo};
}

int main () {
  int iterations = 300;
  int fantasy_n = 16;
  // each iteration:
  // make a fantasy hand
  // make a regular hand
  // calculate ev
  PokerHandEvaluator *eval_ultimate = new PokerHandEvaluator(GameType::Progressive);
  PokerHandEvaluator *eval_nobonus = new PokerHandEvaluator(GameType::NoBonus);

  double total = 0;
  for (int i = 0; i < iterations; ++i) {
    cout << "i : " << i << "\n";

    bool refantasy = false;
    int num_cards = 16;
    do {
      cout << "Using " << num_cards << " cards\n";
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
        Decision d = DecisionFinder(eval_ultimate).findBestDecision(state);
        hands.push_back(hands[i].applyDecision(d));
      }


      set<Card> fantasy_pull = deck.select(num_cards);
      CompletedHand fantasy_hand = Hand().constructOptimalHand(fantasy_pull, eval_nobonus);

      // create CompletedHand with hands[4]
      cout << "My regular hand: " << hands[5] << "\n";
      CompletedHand my_regular_hand = complete(hands[5], eval_ultimate);

      double ev = fantasy_hand.calculatePoints(my_regular_hand);
      total += ev;

      if (fantasy_hand.topInfo.handType == PokerHandType::TRIPS ||
          fantasy_hand.bottomInfo.handType == PokerHandType::QUADS ||
          fantasy_hand.bottomInfo.handType == PokerHandType::STRAIGHT_FLUSH ||
          fantasy_hand.bottomInfo.handType == PokerHandType::ROYAL_FLUSH) refantasy = true;
      if (refantasy) {
        cout << "REFANTASY\n";
        num_cards = 14;
      }
    }
    while (refantasy);
  }
  double avg = total / iterations;
  cout << "Fantasy ev is : " << avg << "\n";
}
