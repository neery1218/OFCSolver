#include <set>
#include "gtest/gtest.h"
#include "hand.h"
#include "fast_poker_hand_evaluator.h"
#include <iostream>
#include "gametype.h"
#include "solver.h"
#include "card.h"
#include "pull.h"
#include "advanced_solver.h"
#include "game_state.h"

class AdvancedSolverTest: public ::testing::Test {
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

TEST_F(AdvancedSolverTest, BasicTest) {
  Hand hand(
      parseCards("Ac"),
      parseCards("2c 2d"),
      parseCards("9h 9d"));

  Pull pull{parseCards("Kd")};
  GameState state{hand, std::vector<Hand>(), pull, std::vector<Card>()};
  Deck deck;

  double ev = AdvancedSolver(evaluator).solve(100, state, deck, 11);
  ASSERT_GE(ev, 0) << ev;
}

TEST_F(AdvancedSolverTest, DeadHand) {
  // all jacks are dead
  Hand hand(
      parseCards("Kh Ad"),
      parseCards("2c 2d 3d 3s 5d"),
      parseCards("Ks Qh Ts 9s"));

  Hand other_hand(
      parseCards("Ac Kd"),
      parseCards("6d 6s 4c 4d"),
      parseCards("7d 7s 2h 2s Jc"));

  Pull pull{parseCards("Js 4s Jd")};
  std::vector<Card> dead_cards = parseCards("Qd Js As");
  std::vector<Hand> other_hands{other_hand};
  GameState state{hand, other_hands, pull, dead_cards};
  Deck deck;

  double ev = AdvancedSolver(evaluator).solve(100, state, deck, 11);
  ASSERT_LT(ev, -6.0);
}
