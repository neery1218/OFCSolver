#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
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
#include "advanced_decision_finder.h"
#include <fstream>
#include <httplib.h>
#include <stdexcept>

std::vector<Card> parseCards(std::string cards) {
  std::vector<Card> parsedCards;
  std::vector<std::string> tokens = absl::StrSplit(cards, " ");
  for (auto token : tokens) {
    assert(token.size() == 2);
    parsedCards.push_back(CardUtils::parseCard(token));
  }

  return parsedCards;
}

Hand parseHand(std::string hand_str) {
  std::vector<std::string> tokens = absl::StrSplit(hand_str, "/");
  if (tokens.size() != 3) throw std::runtime_error("Must be exactly 2 forward slashes in hand");

  std::vector<Card> top = (tokens[0].size() > 0) ? parseCards(tokens[0]) : std::vector<Card>();
  std::vector<Card> mid = (tokens[1].size() > 0) ? parseCards(tokens[1]) : std::vector<Card>();
  std::vector<Card> bot = (tokens[2].size() > 0) ? parseCards(tokens[2]) : std::vector<Card>();

  return Hand{top, mid, bot};
}

int main(int argc, char *argv[]) {
  using namespace httplib;

  Server svr;
  FastPokerHandEvaluator *eval_progressive = new FastPokerHandEvaluator(GameType::Progressive);
  FastPokerHandEvaluator *eval_regular = new FastPokerHandEvaluator(GameType::Regular);
  FastPokerHandEvaluator *eval_ultimate = new FastPokerHandEvaluator(GameType::Ultimate);
  std::cout << "Ready!\n\n";

  svr.Get("/eval", [eval_regular, eval_progressive, eval_ultimate] (const Request & req, Response &res) {
    try {
      std::string eval_type = req.params.find("type")->second;
      FastPokerHandEvaluator *eval = eval_regular;

      if (eval_type == "progressive") {
        eval = eval_progressive;
        std::cout << "Progressive\n";
      }
      else if (eval_type == "ultimate") {
        eval = eval_ultimate;
        std::cout << "Ultimate\n";
      }

      Pull my_pull = Pull{parseCards(req.params.find("pull")->second)};
      std::cout << "Pull: " << my_pull << "\n";

      std::string my_hand_str = req.params.find("my_hand")->second;
      Hand my_hand = parseHand(my_hand_str);
      std::cout << "My hand: " << my_hand << "\n";

      std::vector<Hand> other_hands;
      auto ret = req.params.equal_range("other_hand");
      for (auto it = ret.first; it != ret.second; it++) {
        std::cout <<"candidate: " << it->second;
        Hand other_hand = parseHand(it->second);
        if (other_hand.size() > 0) {
          other_hands.push_back(other_hand);
          std::cout << "Other hand: " << other_hand << "\n";
        }
      }

      std::vector<Card> dead_cards;
      auto it = req.params.find("dead_cards");
      if (it != req.params.end()) {
        std::vector<Card> tmp = parseCards(it->second);
        dead_cards.insert(dead_cards.end(), tmp.begin(), tmp.end());
      }
      std::cout << dead_cards.size() << "dead cards. \n";

      GameState game_state{my_hand, other_hands, my_pull, dead_cards};
      Decision d = AdvancedDecisionFinder(eval).findBestDecision(game_state);

      std::stringstream ss;
      std::string out;
      ss << d;
      out = ss.str();
      res.set_content(out, "text/plain");
      std::cout << "Best decision is: " << d << "\n";
    } catch (const std::exception &e) { std::cout << e.what() << "\n"; }
  });
  svr.listen("0.0.0.0", 8080);
  std::cout << "Done.\n";
}
