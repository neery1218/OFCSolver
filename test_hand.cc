#include "catch.hpp"
#include <set>
#include "hand.h"
#include "poker_hand_evaluator.h"
#include "gametype.h"
#include "card.h"
#include <boost/algorithm/string.hpp>

using namespace std;

PokerHandEvaluator evaluator(GameType::progressive);

set<Card> parseCards(string cards) {
  vector<string> tokens;
  set<Card> parsedCards;

  boost::split(tokens, cards, boost::is_any_of(" "));
  for (auto token : tokens) {
    parsedCards.insert(Card(token));
  }

  return parsedCards;
}

TEST_CASE( "CompletedHand::calculatePoints", "[CompletedHand]" ) {
  PokerHandInfo myTop{1, PokerHandType::HIGH_CARD, 0};
  PokerHandInfo myMid{20, PokerHandType::TRIPS, 2};
  PokerHandInfo myBot{30, PokerHandType::STRAIGHT, 2};


  PokerHandInfo yourTop{2, PokerHandType::HIGH_CARD, 0};
  PokerHandInfo yourMid{5, PokerHandType::PAIR, 0};
  PokerHandInfo yourBot{90, PokerHandType::FLUSH, 4};

  CompletedHand myHand{&myTop, &myMid, &myBot};
  CompletedHand yourHand{&yourTop, &yourMid, &yourBot};

  REQUIRE( myHand.calculatePoints(yourHand) == -1 );
  REQUIRE( yourHand.calculatePoints(myHand) == 1 );
}


TEST_CASE( "CompletedHand::calculatePointsScooped", "[CompletedHand]" ) {
  PokerHandInfo myTop{1, PokerHandType::HIGH_CARD, 0};
  PokerHandInfo myMid{20, PokerHandType::TRIPS, 2};
  PokerHandInfo myBot{30, PokerHandType::STRAIGHT, 2};


  PokerHandInfo yourTop{2, PokerHandType::HIGH_CARD, 0};
  PokerHandInfo yourMid{25, PokerHandType::TRIPS, 2};
  PokerHandInfo yourBot{90, PokerHandType::FLUSH, 4};

  CompletedHand myHand{&myTop, &myMid, &myBot};
  CompletedHand yourHand{&yourTop, &yourMid, &yourBot};

  REQUIRE( myHand.calculatePoints(yourHand) == -8 );
  REQUIRE( yourHand.calculatePoints(myHand) == 8 );
}

TEST_CASE( "CompletedHand::constructOptimalHand", "[CompletedHand]" ) {
  Hand hand(
      parseCards("Ac"),
      parseCards("2c 2d 3d 4s"),
      parseCards("9h 9d 9c 9s"));

  set<Card> cards = parseCards("Ad Kc Ks 2s 3s 4d");
  CompletedHand completedHand = hand.constructOptimalHand(cards, &evaluator);

  REQUIRE( completedHand.topInfo->royalties == 29 );
  REQUIRE( completedHand.middleInfo->royalties == 2 );
  REQUIRE( completedHand.bottomInfo->royalties == 10 );

}
