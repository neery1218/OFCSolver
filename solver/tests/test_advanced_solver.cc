#include "catch.hpp"
#include <set>
#include "hand.h"
#include "poker_hand_evaluator.h"
#include <iostream>
#include "gametype.h"
#include "solver.h"
#include "card.h"
#include "pull.h"
#include <boost/algorithm/string.hpp>
#include "advanced_solver.h"
#include "game_state.h"

using namespace std;

extern PokerHandEvaluator eval_ultimate;
//extern set<Card> _parse(string cards);
set<Card> _parse(string cards) {
  vector<string> tokens;
  set<Card> parsedCards;

  boost::split(tokens, cards, boost::is_any_of(" "));
  for (auto token : tokens) {
    parsedCards.insert(Card(token));
  }

  return parsedCards;
}

TEST_CASE( "AdvancedSolver::solve", "[AdvancedSolver]" ) {
  Hand hand(
      _parse("Ac"),
      _parse("2c 2d"),
      _parse("9h 9d"));

  set<Card> cards = _parse("");
  Pull pull{cards};
  GameState state{hand, vector<Hand>(), pull, vector<Card>()};

  cout << "Start";
  double ev = AdvancedSolver(&eval_ultimate).solve(100, state);
  cout << ev << "\n";
  cout << "End";
  REQUIRE(ev >= 5);

}

TEST_CASE( "AdvancedSolver::solve dead hand", "[Solver]" ) {
  // all jacks are dead
  Hand hand(
      _parse("Kh Ad"),
      _parse("2c 2d 3d 3s 5d"),
      _parse("Ks Qh Ts 9s"));

  Hand other_hand(
      _parse("Ac Kd"),
      _parse("6d 6s 4c 4d"),
      _parse("7d 7s 2h 2s Jc"));

  Pull pull{_parse("Js 4s Jd")};
  set<Card> dead_card_set = _parse("Qd Js As");

  vector<Card> dead_cards(dead_card_set.begin(), dead_card_set.end());
  vector<Hand> other_hands{other_hand};
  GameState state{hand, other_hands, pull, dead_cards};

  double ev = AdvancedSolver(&eval_ultimate).solve(100, state);
  cout << "Bricked ev: " << ev << "\n";
  REQUIRE(ev <= -6.0);

}
