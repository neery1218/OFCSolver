#include <set>
#include "hand.h"
#include "gtest/gtest.h"
#include "poker_hand_evaluator.h"
#include "gametype.h"
#include "card.h"
#include <boost/algorithm/string.hpp>
#include "decision_finder.h"

class DecisionFinderTest : public ::testing::Test {
 protected:
  void SetUp() override {
    evaluator = new FastPokerHandEvaluator(GameType::Ultimate);
  }
  std::vector<Card> parseCards(std::string cards) {
    std::vector<Card> parsedCards;
    std::vector<std::string> tokens = absl::StrSplit(cards, " ");
    for (auto token : tokens) {
      assert(token.size() == 2);
      parsedCards.push_back(CardUtils::parseCard(token));
    }

    return parsedCards;
  }

  // void TearDown() override {}
  FastPokerHandEvaluator *evaluator;
};

TEST_F(DecisionFinderTest, Basic) {
  Hand my_hand(
      parseCards("Ac Ks"), 
      parseCards("2c 2d 2s 3s 3d"), 
      parseCards("9h 9d 9c Js"));

  Hand other_hand(
      parseCards("As Kc Qd"),
      parseCards("4s 4d 5d 5s Jc"),
      parseCards("6s 6d 7d 7s Jd"));
  std::vector<Hand> other_hands{other_hand};

  Pull my_pull = {parseCards("Ah 9s Kd")};
  GameState game_state{my_hand, other_hands, my_pull, std::vector<Card>()};
  Decision d = DecisionFinder(evaluator).findBestDecision(game_state);
}
