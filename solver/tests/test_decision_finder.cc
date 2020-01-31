#include "card.h"
#include "decision_finder.h"
#include "gametype.h"
#include "hand.h"
#include "poker_hand_evaluator.h"
#include "gtest/gtest.h"
#include <set>

class DecisionFinderTest : public ::testing::Test {
  protected:
  void SetUp() override
  {
    evaluator = new FastPokerHandEvaluator(GameType::Ultimate);
  }

  // void TearDown() override {}
  FastPokerHandEvaluator* evaluator;
};

TEST_F(DecisionFinderTest, Basic)
{
  Hand my_hand(
      CardUtils::parseCards("Ac Ks"),
      CardUtils::parseCards("2c 2d 2s 3s 3d"),
      CardUtils::parseCards("9h 9d 9c Js"));

  Hand other_hand(
      CardUtils::parseCards("As Kc Qd"),
      CardUtils::parseCards("4s 4d 5d 5s Jc"),
      CardUtils::parseCards("6s 6d 7d 7s Jd"));
  std::vector<Hand> other_hands { other_hand };

  Pull my_pull = { CardUtils::parseCards("Ah 9s Kd") };
  GameState game_state { my_hand, other_hands, my_pull, std::vector<Card>() };
  Decision d = DecisionFinder(evaluator).findBestDecision(game_state);
}
