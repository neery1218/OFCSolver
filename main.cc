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
#include "set_decision_finder.h"
#include <fstream>
#include <cassert>

using namespace std;

set<Card> parseCards(string cards) {
  vector<string> tokens;
  set<Card> parsedCards;

  boost::split(tokens, cards, boost::is_any_of(" "));
  for (auto token : tokens) {
    assert(token.size() == 2);
    parsedCards.insert(Card(token));
  }

  return parsedCards;
}

int main(int argc, char *argv[]) {
  int command;
  char game_mode;
  GameType type;

  cout << "Game mode? (r, p, u) : ";
  cin >> game_mode;

  if (game_mode == 'r') type = GameType::Regular;
  else if (game_mode == 'p') type = GameType::Progressive;
  else if (game_mode == 'u') type = GameType::Ultimate;
  else throw "wtf";

  PokerHandEvaluator evaluator = PokerHandEvaluator(type);

  cout << "Enter number of players : ";
  cin >> command;

  while (command) {
    ifstream f("input.txt");
    string line;

    // parse pull
    getline(f, line);
    Pull pull{parseCards(line)};
    cout << "My pull :" <<  pull << "\n\n";

    getline(f, line);

    // parse my hand
    getline(f, line);
    set<Card> m_top = line == "x" ? set<Card>() : parseCards(line);

    getline(f, line);
    set<Card> m_mid = line == "x" ? set<Card>() : parseCards(line);

    getline(f, line);
    set<Card> m_bot = line == "x" ? set<Card>() : parseCards(line);

    Hand myHand(m_top, m_mid, m_bot);
    cout << "My hand: \n" << myHand << "\n\n";

    getline(f, line);
    vector<Hand> other_hands;
    for (int i = 0; i < command - 1; ++i) {
      getline(f, line);
      set<Card> o_top = line == "x" ? set<Card>() : parseCards(line);

      getline(f, line);
      set<Card> o_mid = line == "x" ? set<Card>() : parseCards(line);

      getline(f, line);
      set<Card> o_bot = line == "x" ? set<Card>() : parseCards(line);
      
      if (o_top.size() + o_mid.size() + o_bot.size() > 0) {
        other_hands.push_back(Hand(o_top, o_mid, o_bot));
        cout << "Other hand " << i + 1 << " : \n" << other_hands[i] << "\n\n";
      }
    }

    getline(f, line);

    // parse dead cards
    getline(f, line);
    vector<Card> deadCards;
    if (line != "x") {
      set<Card> tmp = parseCards(line);
      deadCards.insert(deadCards.end(), tmp.begin(), tmp.end());
    }
    cout << deadCards.size() << " dead cards. \n";

    // evaluate decisions
    // set solver
    if (myHand.size() == 0) {
      cout << "Set finder.\n";
      Decision d = SetDecisionFinder(evaluator).findBestDecision(
          pull, other_hands);
      cout << "Best decision is: " << d << "\n";
    } else {
      cout << "Draw Decision finder.\n";
      Decision d = DecisionFinder(evaluator).findBestDecision(
          myHand, pull, other_hands, deadCards); 
      cout << "Best decision is: " << d << "\n";
    }

    cout << "Enter number of players : ";
    cin >> command;
    if (command == 0) return 0;
  }
  // create ofc hands
  /*
  Hand myHand(
      parseCards("Ac"), 
      parseCards("2c 2d"), 
      parseCards("9h 9d 7d 7s"));

  Hand otherHand(
      parseCards("As Qc Qd"), 
      parseCards("3c 3d 3s"), 
      parseCards("Th Td Ts"));

  Pull myPull = {parseCards("Ah 2s 9s")};

  vector<Hand> otherHands = {otherHand};
  vector<Card> deadCards = {Card("Kd")};

  Decision d = DecisionFinder(GameType::Regular).findBestDecision(myHand, myPull, otherHands, deadCards);
  */
  /*
  Decision d = SetDecisionFinder(GameType::Progressive).findBestDecision(
      Pull{parseCards("Qd 4s 5s 9s As")},
      vector<Hand> ());
  */
}
