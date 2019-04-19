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
  Hand myHand(parseCards("Ac"), parseCards("2c 2d"), parseCards("9h 9d"));
  Hand otherHand(parseCards("As"), parseCards("3c 3d"), parseCards("Th Td"));
  Pull myPull = {parseCards("Ah 2s Kd")};

  Decision decision1 = {
    Placement(Card("Ah"), Position::top), 
    Placement(Card("2s"), Position::middle), 
    Placement(Card("Kd"), Position::dead)};

  Decision decision2 = {
    Placement(Card("Ah"), Position::top), 
    Placement(Card("2s"), Position::dead), 
    Placement(Card("Kd"), Position::bottom)};

  vector<Decision> decisions = {decision1, decision2};
  vector<Hand> otherHands = {otherHand};
  vector<Card> deadCards;

  vector<pair<Decision, double>> answer = Solver(Method::CPU, GameType::progressive, 100).solve(
      myHand, myPull, otherHands, 
      decisions, deadCards);

  for (auto &p : answer) {
    cout << p.first << " : " << p.second << endl;
  }
}
