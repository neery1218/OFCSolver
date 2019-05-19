#include <boost/algorithm/string.hpp>
#include "advanced_decision_finder.h"
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
#include <fstream>
#include <cassert>

using namespace std;

set<Card> parseCards(string cards) {
  if (cards.empty()) return set<Card> ();
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

  PokerHandEvaluator evaluator(type);

  cout << "Enter number of players : ";
  cin >> command;

  while (command) {
    ifstream f("input.txt");
    string line;

    // parse pull
    getline(f, line);
    Pull my_pull{parseCards(line)};
    cout << "My pull :" << my_pull << "\n\n";

    getline(f, line);

    // parse my hand
    getline(f, line);
    set<Card> m_top = line == "x" ? set<Card>() : parseCards(line);

    getline(f, line);
    set<Card> m_mid = line == "x" ? set<Card>() : parseCards(line);

    getline(f, line);
    set<Card> m_bot = line == "x" ? set<Card>() : parseCards(line);

    Hand my_hand(m_top, m_mid, m_bot);
    cout << "My hand: \n" << my_hand << "\n\n";

    vector<Hand> other_hands;
    for (int i = 0; i < command - 1; ++i) {
      getline(f, line);

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
    vector<Card> dead_cards;
    if (line != "x") {
      set<Card> tmp = parseCards(line);
      dead_cards.insert(dead_cards.end(), tmp.begin(), tmp.end());
    }
    cout << dead_cards.size() << " dead cards. \n";

    // evaluate decisions
    // set solver
    GameState game_state{my_hand, other_hands, my_pull, dead_cards};
    Decision d = AdvancedDecisionFinder(&evaluator).findBestDecision(game_state);
    cout << "Best decision is: " << d << "\n";

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
  vector<Card> dead_cards = {Card("Kd")};

  Decision d = DecisionFinder(GameType::Regular).findBestDecision(myHand, myPull, otherHands, dead_cards);
  */
  /*
  Decision d = SetDecisionFinder(GameType::Progressive).findBestDecision(
      Pull{parseCards("Qd 4s 5s 9s As")},
      vector<Hand> ());
  */
}
