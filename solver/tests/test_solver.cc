#include "gtest/gtest.h"
#include <iostream>
#include <set>
#include <vector>

#include "card.h"
#include "gametype.h"
#include "hand.h"
#include "poker_hand_evaluator.h"
#include "position.h"
#include "pull.h"
#include "solver.h"

class SolverTest : public ::testing::Test {
  protected:
  void SetUp() override
  {
    evaluator = new FastPokerHandEvaluator(GameType::Regular);
  }

  // void TearDown() override {}
  FastPokerHandEvaluator* evaluator;
};

TEST_F(SolverTest, Basic)
{
  Hand hand(
      CardUtils::parseCards("Ac"),
      CardUtils::parseCards("2c 2d 3d 4s"),
      CardUtils::parseCards("9h 9d 9c 9s"));

  Pull pull { CardUtils::parseCards("Ac 4c 4d") };

  double ev = Solver(evaluator).solve(10, hand, pull, std::vector<Hand>(), std::vector<Card>());
  ASSERT_GT(ev, 10);
}

TEST_F(SolverTest, DeadHand)
{
  // all jacks are dead
  Hand hand(
      CardUtils::parseCards("Kh Ad"),
      CardUtils::parseCards("2c 2d 3d 3s 5d"),
      CardUtils::parseCards("Ks Qh Ts 9s"));

  Hand other_hand(
      CardUtils::parseCards("Ac Kd Qd"),
      CardUtils::parseCards("6d 6s 4c 4d Jh"),
      CardUtils::parseCards("7d 7s 2h 2s Jc"));

  Pull pull { CardUtils::parseCards("Ts 4s Jd") };
  std::vector<Card> dead_card_set = CardUtils::parseCards("Qd Js As Ah");

  std::vector<Card> dead_cards(dead_card_set.begin(), dead_card_set.end());
  std::vector<Hand> other_hands { other_hand };

  double ev = Solver(evaluator).solve(10, hand, pull, other_hands, dead_cards);
  ASSERT_EQ(ev, -6.0);
}
