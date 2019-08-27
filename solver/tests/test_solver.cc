#include "gtest/gtest.h"
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

class SolverTest : public ::testing::Test {
 protected:
  void SetUp() override {
    evaluator = new PokerHandEvaluator(GameType::Ultimate);;
  }
  std::set<Card> parseCards(std::string cards) {
    std::set<Card> parsedCards;
    std::vector<std::string> tokens = absl::StrSplit(cards, " ");
    for (auto token : tokens) {
      assert(token.size() == 2);
      parsedCards.insert(Card(token));
    }

    return parsedCards;
  }

  // void TearDown() override {}
  PokerHandEvaluator *evaluator;
};

TEST_F( SolverTest, Basic) {
  Hand hand(
      parseCards("Ac"),
      parseCards("2c 2d 3d 4s"),
      parseCards("9h 9d 9c 9s"));

  Pull pull{parseCards("Ac 4c 4d")};

  double ev = Solver(evaluator).solve(10, hand, pull, std::vector<Hand> (), std::vector<Card> ());
  std::cout << "Solver ev: " << ev << std::endl;
  ASSERT_GT(ev, 10);

}

TEST_F(SolverTest, DeadHand) {
  // all jacks are dead
  Hand hand(
      parseCards("Kh Ad"),
      parseCards("2c 2d 3d 3s 5d"),
      parseCards("Ks Qh Ts 9s"));

  Hand other_hand(
      parseCards("Ac Kd Qd"),
      parseCards("6d 6s 4c 4d Jh"),
      parseCards("7d 7s 2h 2s Jc"));

  Pull pull{parseCards("Ts 4s Jd")};
  std::set<Card> dead_card_set = parseCards("Qd Js As Ah");

  std::vector<Card> dead_cards(dead_card_set.begin(), dead_card_set.end());
  std::vector<Hand> other_hands{other_hand};

  double ev = Solver(evaluator).solve(10, hand, pull, other_hands, dead_cards);
  ASSERT_EQ(ev, -6.0);

}
