#include "gtest/gtest.h"
#include "absl/strings/str_split.h"
#include <vector>
#include <set>
#include <string>
#include <iostream>

#include "fast_poker_hand_evaluator.h"
#include "poker_hand_evaluator.h"
#include "gametype.h"
#include "position.h"
#include "card.h"

class FastPokerHandEvaluatorTest : public ::testing::Test {
  protected:

    void SetUp() override { 
      evaluator = new FastPokerHandEvaluator(GameType::Regular);
    }

    PokerHandInfo evalHand(std::string cards, Position pos) {
      std::vector<Card> parsedCards;
      std::vector<std::string> tokens = absl::StrSplit(cards, " ");
      for (auto token : tokens) {
        assert(token.size() == 2);
        parsedCards.push_back(CardUtils::parseCard(token));
      }

      if (pos == Position::bottom) {
        PokerHandInfo res = evaluator->evalBottom(parsedCards);
        return res;
      } 
      else if (pos == Position::middle) {
        PokerHandInfo res = evaluator->evalMiddle(parsedCards);
        return res;
      }
      else if (pos == Position::top) {
        PokerHandInfo res = evaluator->evalTop(parsedCards);
        return res;
      }

    }

    // void TearDown() override {}
    FastPokerHandEvaluator *evaluator;
};

TEST_F(FastPokerHandEvaluatorTest, HighestStraight) {
  PokerHandInfo info = evalHand("As Kh Qh Jh Th", Position::bottom);
  ASSERT_EQ(PokerHandInfoUtils::getRoyalties(info), 2);
  ASSERT_EQ(PokerHandInfoUtils::getHandType(info), STRAIGHT);
  ASSERT_GT(PokerHandInfoUtils::getRank(info), 0);

  PokerHandInfo middle_info = evalHand("As Kh Qh Jh Th", Position::middle);
  ASSERT_EQ(PokerHandInfoUtils::getRoyalties(middle_info), 4);
  ASSERT_EQ(PokerHandInfoUtils::getHandType(middle_info), STRAIGHT);
  ASSERT_GT(PokerHandInfoUtils::getRank(middle_info), 0);
}

TEST_F(FastPokerHandEvaluatorTest, ExtensiveBottom) {
  // Use old PokerhandEvaluator to test FastPokerhandEvaluator's values.
  PokerHandEvaluator *old_evaluator = new PokerHandEvaluator(GameType::Regular);
  for (auto p : old_evaluator->bottom_eval_info) {
    std::string row = p.first;
    PokerHandInfo expected_info = p.second;

    PokerHandInfo info = evalHand(row, Position::bottom);
    ASSERT_EQ(PokerHandInfoUtils::getRoyalties(info), PokerHandInfoUtils::getRoyalties(expected_info)) << row;
    ASSERT_EQ(PokerHandInfoUtils::getHandType(info), PokerHandInfoUtils::getHandType(expected_info)) << row;

  }
}
