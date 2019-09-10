#include "absl/strings/str_split.h"
#include <vector>
#include <set>
#include <iostream>

#include "poker_hand_evaluator.h"
#include "gametype.h"
#include "position.h"
#include "card.h"
#include "hand.h"
#include "solver.h"
#include "pull.h"


static std::set<Card> parseCards(std::string cards) {
    std::set<Card> parsedCards;
    std::vector<std::string> tokens = absl::StrSplit(cards, " ");
    for (auto token : tokens) {
      assert(token.size() == 2);
      parsedCards.insert(Card(token));
    }

    return parsedCards;
}

int main() {
  PokerHandEvaluator *evaluator = new PokerHandEvaluator(GameType::Regular);
  Hand hand(
      parseCards("Ac"),
      parseCards("2c 2d"),
      parseCards("9h 9d"));

  Pull pull{parseCards("Ac 4c 4d")};
	double ev = Solver(evaluator).solve(
			10000,
			hand,
			pull,
			std::vector<Hand> (),
			std::vector<Card> ()
	);
  std::cout<< "Ev: " << ev << std::endl;
}
