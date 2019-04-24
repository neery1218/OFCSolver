#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <utility>
#include "hand.h"
#include "pull.h"
#include "gametype.h"
#include "solver.h"
#include "card.h"
#include "position.h"
#include "decision.h"
#include "placement.h"
#include "decision_finder.h"

using namespace std;

set<Card> parseCards(string cards) {
  vector<string> tokens;
  set<Card> parsedCards;

  boost::split(tokens, cards, boost::is_any_of(" "));
  for (auto token : tokens) {
    parsedCards.insert(Card(token));
  }

  return parsedCards;
}

int main(int argc, char *argv[]) {
  // create ofc hands
  Hand myHand(parseCards("Ac"), parseCards("2c 2d"), parseCards("9h 9d 7d 7s"));
  Hand otherHand(parseCards("As Qc Qd"), parseCards("3c 3d 3s"), parseCards("Th Td Ts"));
  Pull myPull = {parseCards("Ah 2s 9s")};

  vector<Hand> otherHands = {otherHand};
  vector<Card> deadCards;

  DecisionFinder(GameType::Progressive).findBestDecision(myHand, myPull, otherHands, deadCards);
}
