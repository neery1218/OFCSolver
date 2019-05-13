#include "catch.hpp"
#include <set>
#include "hand.h"
#include "poker_hand_evaluator.h"
#include "gametype.h"
#include "card.h"
#include <boost/algorithm/string.hpp>
#include "decision_finder.h"

extern PokerHandEvaluator eval_ultimate;

using namespace std;

set<Card> parse(string cards) {
  vector<string> tokens;
  set<Card> parsedCards;

  boost::split(tokens, cards, boost::is_any_of(" "));
  for (auto token : tokens) {
    parsedCards.insert(Card(token));
  }

  return parsedCards;
}

TEST_CASE( "calculate all decisisions", "[DecisionFinder]" ) {
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
  Decision d = DecisionFinder(&eval_ultimate).findBestDecision(game_state);
}
