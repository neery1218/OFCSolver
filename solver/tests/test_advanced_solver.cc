#include "advanced_solver.h"
#include "card.h"
#include "fast_poker_hand_evaluator.h"
#include "game_state.h"
#include "gametype.h"
#include "hand.h"
#include "pull.h"
#include "solver.h"
#include "gtest/gtest.h"
#include <iostream>
#include <set>

class AdvancedSolverTest : public ::testing::Test {
  protected:
  void SetUp() override
  {
    evaluator = new FastPokerHandEvaluator(GameType::Ultimate);
  }

  // void TearDown() override {}
  FastPokerHandEvaluator* evaluator;
};

TEST_F(AdvancedSolverTest, BasicTest)
{
  Hand hand(
      CardUtils::parseCards("Ac"),
      CardUtils::parseCards("2c 2d"),
      CardUtils::parseCards("9h 9d"));

  Pull pull { CardUtils::parseCards("Kd") };
  GameState state { hand, std::vector<Hand>(), pull, std::vector<Card>() };
  Deck deck;

  double ev = AdvancedSolver(evaluator).solve(100, state, deck, 11);
  ASSERT_GE(ev, 0) << ev;
}

TEST_F(AdvancedSolverTest, DeadHand)
{
  // all jacks are dead
  Hand hand(
      CardUtils::parseCards("Kh Ad"),
      CardUtils::parseCards("2c 2d 3d 3s 5d"),
      CardUtils::parseCards("Ks Qh Ts 9s"));

  Hand other_hand(
      CardUtils::parseCards("Ac Kd"),
      CardUtils::parseCards("6d 6s 4c 4d"),
      CardUtils::parseCards("7d 7s 2h 2s Jc"));

  Pull pull { CardUtils::parseCards("Js 4s Jd") };
  std::vector<Card> dead_cards = CardUtils::parseCards("Qd Js As");
  std::vector<Hand> other_hands { other_hand };
  GameState state { hand, other_hands, pull, dead_cards };
  Deck deck;

  double ev = AdvancedSolver(evaluator).solve(100, state, deck, 11);
  ASSERT_LT(ev, -6.0);
}
