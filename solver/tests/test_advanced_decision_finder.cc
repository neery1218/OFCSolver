#include "advanced_decision_finder.h"
#include "card.h"
#include "fast_poker_hand_evaluator.h"
#include "gametype.h"
#include "hand.h"
#include "gtest/gtest.h"
#include <set>

class AdvancedDecisionFinderTest : public ::testing::Test {
  protected:
  void SetUp() override
  {
    evaluator = new FastPokerHandEvaluator(GameType::Ultimate);
  }

  // void TearDown() override {}
  FastPokerHandEvaluator* evaluator;
};

TEST_F(AdvancedDecisionFinderTest, LastDraw)
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
  Decision d = AdvancedDecisionFinder(evaluator).findBestDecision(game_state);
  std::cout << "best decision is: " << d;
}

TEST_F(AdvancedDecisionFinderTest, SecondLastDraw)
{
  Hand my_hand(
      CardUtils::parseCards("Ac Ks"),
      CardUtils::parseCards("2c 2d 2s 3s"),
      CardUtils::parseCards("9h 9d 9c"));

  Hand other_hand(
      CardUtils::parseCards("As Kc Qd"),
      CardUtils::parseCards("4s 4d 5d 5s"),
      CardUtils::parseCards("6s 6d 7d 7s"));

  std::vector<Hand> other_hands { other_hand };

  Pull my_pull = { CardUtils::parseCards("Ah 9s 3s") };
  GameState game_state { my_hand, other_hands, my_pull, std::vector<Card>() };
  Decision d = AdvancedDecisionFinder(evaluator).findBestDecision(game_state);
  std::cout << "best decision is: " << d;
}

/*
TEST_F(AdvancedDecisionFinderTest, GreatSet) {
  Hand my_hand;

  Pull my_pull = {CardUtils::parseCards("Ah 9s 9d 2s 2c")};

  GameState game_state{my_hand, std::vector<Hand>(), my_pull, std::vector<Card>()};
  Decision d = AdvancedDecisionFinder(evaluator).findBestDecision(game_state);
  std::cout << "best decision is: " << d;
}
/*
TEST_F(AdvancedDecision, BadPull) {
  Hand my_hand;

  Pull my_pull = {CardUtils::parseCards("Jh 7s 6c 4h 4d")};

  GameState game_state{my_hand, std::vector<Hand>(), my_pull, std::vector<Card>()};
  Decision d = AdvancedDecisionFinder(evaluator).findBestDecision(game_state);
  std::cout << "best decision is: " << d;
}

TEST_F(AdvancedDecision, BadPullAgainstGreatHand) {
  Hand other_hand(
      CardUtils::parseCards("Ac As"), 
      CardUtils::parseCards("2c"), 
      CardUtils::parseCards("9h 9d"));
  Hand my_hand;

  std::vector<Hand> other_hands{other_hand};

  Pull my_pull = {CardUtils::parseCards("Jh 7s 6c 4h 4d")};

  GameState game_state{my_hand, other_hands, my_pull, std::vector<Card>()};
  Decision d = AdvancedDecisionFinder(evaluator).findBestDecision(game_state);
  std::cout << "best decision is: " << d;
}
*/
