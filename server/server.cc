#include <httplib.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "advanced_decision_finder.h"
#include "card.h"
#include "decision.h"
#include "decision_finder.h"
#include "fantasy_solver.h"
#include "gametype.h"
#include "hand.h"
#include "placement.h"
#include "position.h"
#include "pull.h"
#include "server/include/single_include/nlohmann/json.hpp"
#include "solver.h"

Hand parseHand(std::string hand_str) {
  std::vector<std::string> tokens;
  boost::split(tokens, hand_str, boost::is_any_of("/"));
  if (tokens.size() != 3)
    throw std::runtime_error("Must be exactly 2 forward slashes in hand");

  std::vector<Card> top = (tokens[0].size() > 0)
                              ? CardUtils::parseCards(tokens[0])
                              : std::vector<Card>();
  std::vector<Card> mid = (tokens[1].size() > 0)
                              ? CardUtils::parseCards(tokens[1])
                              : std::vector<Card>();
  std::vector<Card> bot = (tokens[2].size() > 0)
                              ? CardUtils::parseCards(tokens[2])
                              : std::vector<Card>();

  return Hand{top, mid, bot};
}

int main(int argc, char* argv[]) {
  using namespace httplib;
  using json = nlohmann::json;

  if (argc != 2) {
    std::cout << "Pass in path to hand strength file! Exiting..." << std::endl;
    return 0;
  }

  char* filename = argv[1];
  std::cout << "Filename is : " << filename << std::endl;

  Server svr;
  FastPokerHandEvaluator* eval_progressive =
      new FastPokerHandEvaluator(GameType::Progressive, filename);
  FastPokerHandEvaluator* eval_regular =
      new FastPokerHandEvaluator(GameType::Regular, filename);
  FastPokerHandEvaluator* eval_ultimate =
      new FastPokerHandEvaluator(GameType::Ultimate, filename);
  FastPokerHandEvaluator* eval_fantasy =
      new FastPokerHandEvaluator(GameType::Fantasy, filename);
  std::cout << "Ready!\n\n";

  svr.Get("/eval", [eval_regular, eval_progressive, eval_ultimate,
                    eval_fantasy](const Request& req, Response& res) {
    try {
      std::string eval_type = req.params.find("type")->second;
      FastPokerHandEvaluator* eval = eval_regular;

      if (eval_type == "progressive") {
        eval = eval_progressive;
        std::cout << "Progressive\n";
      } else if (eval_type == "ultimate") {
        eval = eval_ultimate;
        std::cout << "Ultimate\n";
      }

      Pull my_pull =
          Pull{CardUtils::parseCards(req.params.find("pull")->second)};
      std::cout << "Pull: " << my_pull << "\n";

      std::string my_hand_str = req.params.find("my_hand")->second;
      Hand my_hand = parseHand(my_hand_str);
      std::cout << "My hand: " << my_hand << "\n";

      std::vector<Hand> other_hands;
      auto ret = req.params.equal_range("other_hand");
      for (auto it = ret.first; it != ret.second; it++) {
        Hand other_hand = parseHand(it->second);
        if (other_hand.size() > 0) {
          other_hands.push_back(other_hand);
          std::cout << "Other hand: " << other_hand << "\n";
        }
      }

      std::string n_solves_str = req.params.find("n_solves")->second;
      std::string n_decision_solves_str =
          req.params.find("n_decision_solves")->second;
      int n_solves = std::stoi(n_solves_str);
      int n_decision_solves = std::stoi(n_decision_solves_str);
      std::cout << "High level solves:" << n_solves << std::endl;
      std::cout << "Intermediate solves:" << n_decision_solves << std::endl;

      std::vector<Card> dead_cards;
      auto it = req.params.find("dead_cards");
      if (it != req.params.end()) {
        std::vector<Card> tmp = CardUtils::parseCards(it->second);
        dead_cards.insert(dead_cards.end(), tmp.begin(), tmp.end());
      }
      std::cout << dead_cards.size() << "dead cards. \n";

      GameState game_state{my_hand,    other_hands, my_pull,
                           dead_cards, n_solves,    n_decision_solves};
      // fantasy solver
      if (my_pull.cards.size() > 13) {
        Decision d = FantasySolver().solve(eval_fantasy, my_pull.cards);
        std::stringstream ss;
        json output;
        ss << d;
        output["best"] = ss.str();
        res.set_content(output.dump(), "text/plain");
      } else {
        auto decisions =
            AdvancedDecisionFinder(eval).findBestDecision(game_state);

        json output;
        {
          std::stringstream ss;
          ss << decisions[0].second;
          output["best"] = ss.str();
        }
        for (const auto& p : decisions) {
          std::stringstream ss;
          ss << p.second;
          output[ss.str()] = p.first;
        }

        res.set_content(output.dump(), "text/plain");
      }
    } catch (const std::exception& e) {
      std::cout << e.what() << "\n";
    }
  });
  svr.listen("0.0.0.0", 9001);
  std::cout << "Done.\n";
}
