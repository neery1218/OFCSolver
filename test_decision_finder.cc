#include "catch.hpp"
#include <set>
#include "hand.h"
#include "poker_hand_evaluator.h"
#include "gametype.h"
#include "card.h"
#include <boost/algorithm/string.hpp>
#include "decision_finder.h"

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
  Hand myHand(parse("Ac"), parse("2c 2d"), parse("9h 9d"));
  Pull myPull = {parse("Ah As Kd")};
  /*
  DecisionFinder().findBestDecision(myHand, myPull);
  */

}
