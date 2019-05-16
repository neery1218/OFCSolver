#include "catch.hpp"
#include <set>
#include "hand.h"
#include "poker_hand_evaluator.h"
#include "gametype.h"
#include "card.h"
#include <boost/algorithm/string.hpp>

using namespace std;

PokerHandEvaluator evaluator(GameType::Progressive);

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

  CompletedHand myHand{myTop, myMid, myBot};
  CompletedHand yourHand{yourTop, yourMid, yourBot};

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

  CompletedHand myHand{myTop, myMid, myBot};
  CompletedHand yourHand{yourTop, yourMid, yourBot};

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

  REQUIRE( completedHand.topInfo.royalties == 28 );
  REQUIRE( completedHand.middleInfo.royalties == 2 );
  REQUIRE( completedHand.bottomInfo.royalties == 10 );

}

TEST_CASE( "CompletedHand::constructOptimalHand2", "[CompletedHand]" ) {
  Hand hand(
      parseCards("Ad"),
      parseCards("5s 2c"),
      parseCards("Td 9d"));

  set<Card> cards = parseCards("Kd Qd 4d 5c 2s Kh Ks Jh As");
  CompletedHand completedHand = hand.constructOptimalHand(cards, &evaluator);

  REQUIRE( completedHand.topInfo.royalties == 28 );
  REQUIRE( completedHand.middleInfo.royalties == 0 );
  REQUIRE( completedHand.bottomInfo.royalties == 4 );

}

TEST_CASE( "CompletedHand::constructOptimalHand3", "[CompletedHand]" ) {
  Hand hand(
      parseCards("Ad"),
      parseCards("5s 2c"),
      parseCards("Td 9d"));

  set<Card> cards = parseCards("Kd Qd 4s 5c 2s Kh Ks Jh As");
  CompletedHand completedHand = hand.constructOptimalHand(cards, &evaluator);

  REQUIRE( completedHand.topInfo.royalties == 28 );
  REQUIRE( completedHand.middleInfo.royalties == 0 );
  REQUIRE( completedHand.bottomInfo.royalties == 2 );

}

TEST_CASE( "CompletedHand::constructOptimalHandFouled", "[CompletedHand]" ) {
  Hand hand(
      parseCards("Ad Ac Qh"),
      parseCards("5s 2c 2d 3s 4s"),
      parseCards("Td"));

  set<Card> cards = parseCards("Kd Qd 4d 5c 2s Kh Ks Jh As");
  CompletedHand completedHand = hand.constructOptimalHand(cards, &evaluator);

  REQUIRE( completedHand.topInfo.royalties == 0 );
  REQUIRE( completedHand.topInfo.overallRank == -1 );
  REQUIRE( completedHand.middleInfo.royalties == 0 );
  REQUIRE( completedHand.middleInfo.overallRank == -1 );
  REQUIRE( completedHand.bottomInfo.royalties == 0 );
  REQUIRE( completedHand.bottomInfo.overallRank == -1 );

}
