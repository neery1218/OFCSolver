#include "catch.hpp"
#include <set>
#include "hand.h"
#include "poker_hand_evaluator.h"
#include "gametype.h"
#include "card.h"
#include <boost/algorithm/string.hpp>
#include "advanced_decision_finder.h"

extern PokerHandEvaluator eval_ultimate;

using namespace std;

extern set<Card> parse(string cards);

TEST_CASE( "AdvancedDecision::last draw", "[AdvancedDecisionFinder]" ) {
  Hand my_hand(
      parse("Ac Ks"), 
      parse("2c 2d 2s 3s 3d"), 
      parse("9h 9d 9c Js"));

  Hand other_hand(
      parse("As Kc Qd"),
      parse("4s 4d 5d 5s Jc"),
      parse("6s 6d 7d 7s Jd"));
  vector<Hand> other_hands{other_hand};

  Pull my_pull = {parse("Ah 9s Kd")};
  GameState game_state{my_hand, other_hands, my_pull, vector<Card>()};
  Decision d = AdvancedDecisionFinder(&eval_ultimate).findBestDecision(game_state);
  cout << "best decision is: " << d;
}

TEST_CASE( "AdvancedDecision::set", "[AdvancedDecisionFinder]" ) {
  Hand my_hand;

  Pull my_pull = {parse("Ah 9s 9d 2s 2c")};

  GameState game_state{my_hand, vector<Hand>(), my_pull, vector<Card>()};
  Decision d = AdvancedDecisionFinder(&eval_ultimate).findBestDecision(game_state);
  cout << "best decision is: " << d;
}

TEST_CASE( "AdvancedDecision::set2", "[AdvancedDecisionFinder]" ) {
  Hand my_hand;

  Pull my_pull = {parse("Jh 7s 6c 4h 4d")};

  GameState game_state{my_hand, vector<Hand>(), my_pull, vector<Card>()};
  Decision d = AdvancedDecisionFinder(&eval_ultimate).findBestDecision(game_state);
  cout << "best decision is: " << d;
}

TEST_CASE( "AdvancedDecision::set3", "[AdvancedDecisionFinder]" ) {
  Hand other_hand(
      parse("Ac As"), 
      parse("2c"), 
      parse("9h 9d"));
  Hand my_hand;

  vector<Hand> other_hands{other_hand};

  Pull my_pull = {parse("Jh 7s 6c 4h 4d")};

  GameState game_state{my_hand, other_hands, my_pull, vector<Card>()};
  Decision d = AdvancedDecisionFinder(&eval_ultimate).findBestDecision(game_state);
  cout << "best decision is: " << d;
}
