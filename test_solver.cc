#include "catch.hpp"
#include <set>
#include "hand.h"
#include "poker_hand_evaluator.h"
#include "gametype.h"
#include "solver.h"
#include "card.h"
#include "pull.h"
#include <boost/algorithm/string.hpp>

using namespace std;

PokerHandEvaluator eval(GameType::Ultimate);

set<Card> parse_cards(string cards) {
  vector<string> tokens;
  set<Card> parsedCards;

  boost::split(tokens, cards, boost::is_any_of(" "));
  for (auto token : tokens) {
    parsedCards.insert(Card(token));
  }

  return parsedCards;
}

TEST_CASE( "Solver::solve", "[Solver]" ) {
  Hand hand(
      parse_cards("Ac"),
      parse_cards("2c 2d 3d 4s"),
      parse_cards("9h 9d 9c 9s"));

  Pull pull{parse_cards("Ac 4s 4d")};

  double ev = Solver(&eval).solve(10, hand, pull, vector<Hand> (), vector<Card> ());
  REQUIRE(ev >= 10);

}

TEST_CASE( "hand is dead, ev must be 0", "[Solver]" ) {
  // all jacks are dead
  Hand hand(
      parse_cards("Kh Ad"),
      parse_cards("2c 2d 3d 3s 5d"),
      parse_cards("Ks Qh Ts 9s"));

  Hand other_hand(
      parse_cards("Ac Kd Qd"),
      parse_cards("6d 6s 4c 4d Jh"),
      parse_cards("7d 7s 2h 2s Jc"));

  Pull pull{parse_cards("Ts 4s Jd")};
  set<Card> dead_card_set = parse_cards("Qd Js As Ah");

  vector<Card> dead_cards(dead_card_set.begin(), dead_card_set.end());
  vector<Hand> other_hands{other_hand};

  double ev = Solver(&eval).solve(10, hand, pull, other_hands, dead_cards);
  REQUIRE(ev == -6.0);

}
