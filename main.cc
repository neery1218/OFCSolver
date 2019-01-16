#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include "hand.h"
#include "pull.h"
#include "gametype.h"
#include "solver.h"
#include "card.h"

using namespace std;

vector<Card> parseCards(string cards) {
  vector<string> tokens;
  vector<Card> parsedCards;

  boost::split(tokens, cards, boost::is_any_of(" "));
  for (auto token : tokens) {
    parsedCards.push_back(Card(token));
  }

  return parsedCards;
}

int main(int argc, char *argv[]) {
  // create ofc hands
  Hand myHand(parseCards("Ac Ks"), parseCards("2c 2d 3c"), parseCards("9h 9d 9c 9s"));
  Hand otherHand(parseCards("Qc Qd"), parseCards("As Ad 4h"), parseCards("5h 5d 5s 8d"));
  Pull myPull = {parseCards("Ah 2s 3d")};

  Decision decision1 = {
    Placement(Card("Ah"), Position::top), 
    Placement(Card("2s"), Position::middle), 
    Placement(Card("3d"), Position::dead)};

  vector<Decision> decisions = {decision1};
  vector<Hand> otherHands = {otherHand};

  Decision answer = Solver().solve(myHand, myPull, otherHands, decisions, GameType::regular);
  cout << answer;
}
