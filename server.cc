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
#include <fstream>
#include <httplib.h>
#include <stdexcept>

using namespace std;

set<Card> parseCards(string cards) {
  vector<string> tokens;
  set<Card> parsedCards;

  boost::split(tokens, cards, boost::is_any_of(" "));
  for (auto token : tokens) {
    if (token.size() != 2) throw runtime_error("card size has to be 2 u fool");
    parsedCards.insert(Card(token));
  }

  return parsedCards;
}

Hand parseHand(string hand_str) {
  vector<string> tokens;
  boost::split(tokens, hand_str, boost::is_any_of("/"));
  if (tokens.size() != 3) throw runtime_error("Must be exactly 2 forward slashes in hand");

  set<Card> top = (tokens[0].size() > 0) ? parseCards(tokens[0]) : set<Card>();
  set<Card> mid = (tokens[1].size() > 0) ? parseCards(tokens[1]) : set<Card>();
  set<Card> bot = (tokens[2].size() > 0) ? parseCards(tokens[2]) : set<Card>();

  return Hand{top, mid, bot};
}

int main(int argc, char *argv[]) {
  using namespace httplib;

  Server svr;
  PokerHandEvaluator *eval_progressive = new PokerHandEvaluator(GameType::Progressive);
  PokerHandEvaluator *eval_regular = new PokerHandEvaluator(GameType::Regular);
  PokerHandEvaluator *eval_ultimate = new PokerHandEvaluator(GameType::Ultimate);
  cout << "Ready!\n\n";

  svr.Get("/eval", [eval_regular, eval_progressive, eval_ultimate] (const Request & req, Response &res) {
    try {
      string eval_type = req.params.find("type")->second;
      PokerHandEvaluator *eval = eval_regular;

      if (eval_type == "progressive") {
        eval = eval_progressive;
        cout << "Progressive\n";
      }
      else if (eval_type == "ultimate") {
        eval = eval_ultimate;
        cout << "Ultimate\n";
      }

      Pull my_pull = Pull{parseCards(req.params.find("pull")->second)};
      cout << "Pull: " << my_pull << "\n";

      string my_hand_str = req.params.find("my_hand")->second;
      Hand my_hand = parseHand(my_hand_str);
      cout << "My hand: " << my_hand << "\n";

      vector<Hand> other_hands;
      auto ret = req.params.equal_range("other_hand");
      for (auto it = ret.first; it != ret.second; it++) {
        cout <<"candidate: " << it->second;
        Hand other_hand = parseHand(it->second);
        if (other_hand.size() > 0) {
          other_hands.push_back(other_hand);
          cout << "Other hand: " << other_hand << "\n";
        }
      }

      vector<Card> dead_cards;
      auto it = req.params.find("dead_cards");
      if (it != req.params.end()) {
        set<Card> tmp = parseCards(it->second);
        dead_cards.insert(dead_cards.end(), tmp.begin(), tmp.end());
      }
      cout << dead_cards.size() << "dead cards. \n";

      GameState game_state{my_hand, other_hands, my_pull, dead_cards};
      Decision d = DecisionFinder(eval).findBestDecision(game_state);
      cout << "Best decision is: " << d << "\n";
    } catch (const std::exception &e) { cout << e.what() << "\n"; }
  });
  svr.listen("0.0.0.0", 8080);
  cout << "Done.\n";
}
