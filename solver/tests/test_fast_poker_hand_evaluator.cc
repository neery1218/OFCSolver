#include "gtest/gtest.h"
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "card.h"
#include "fast_poker_hand_evaluator.h"
#include "gametype.h"
#include "poker_hand_evaluator.h"
#include "position.h"

class FastPokerHandEvaluatorTest : public ::testing::Test {
  protected:
  void SetUp() override
  {
    evaluator = new FastPokerHandEvaluator(GameType::Regular);
    evaluator_progressive = new FastPokerHandEvaluator(GameType::Progressive);
    evaluator_ultimate = new FastPokerHandEvaluator(GameType::Ultimate);
  }

  PokerHandInfo evalHand(std::string cards, Position pos)
  {
    std::vector<Card> parsedCards = CardUtils::parseCards(cards);

    if (pos == Position::bottom) {
      PokerHandInfo res = evaluator->evalBottom(parsedCards);
      return res;
    } else if (pos == Position::middle) {
      PokerHandInfo res = evaluator->evalMiddle(parsedCards);
      return res;
    } else if (pos == Position::top) {
      PokerHandInfo res = evaluator->evalTop(parsedCards);
      return res;
    }
  }

  // void TearDown() override {}
  FastPokerHandEvaluator* evaluator;
  FastPokerHandEvaluator* evaluator_progressive;
  FastPokerHandEvaluator* evaluator_ultimate;
};

TEST_F(FastPokerHandEvaluatorTest, HighestStraight)
{
  PokerHandInfo info = evalHand("As Kh Qh Jh Th", Position::bottom);
  ASSERT_EQ(PokerHandUtils::getRoyalties(info), 2);
  ASSERT_EQ(PokerHandUtils::getHandType(info), static_cast<int>(HandType::STRAIGHT));
  ASSERT_GT(PokerHandUtils::getOverallRank(info), 0);

  PokerHandInfo middle_info = evalHand("As Kh Qh Jh Th", Position::middle);
  ASSERT_EQ(PokerHandUtils::getRoyalties(middle_info), 4);
  ASSERT_EQ(PokerHandUtils::getHandType(middle_info), static_cast<int>(HandType::STRAIGHT));
  ASSERT_GT(PokerHandUtils::getOverallRank(middle_info), 0);
}

TEST_F(FastPokerHandEvaluatorTest, ExtensiveBottom)
{
  // Use old PokerhandEvaluator to test FastPokerhandEvaluator's values.
  PokerHandEvaluator* old_evaluator = new PokerHandEvaluator(GameType::Regular);
  for (auto p : old_evaluator->bottom_eval_info) {
    std::string row = p.first;
    PokerHandInfo expected_info = p.second;

    PokerHandInfo info = evalHand(row, Position::bottom);
    ASSERT_EQ(PokerHandUtils::getRoyalties(info), PokerHandUtils::getRoyalties(expected_info)) << row;
    ASSERT_EQ(PokerHandUtils::getHandType(info), PokerHandUtils::getHandType(expected_info)) << row;
  }
}

TEST_F(FastPokerHandEvaluatorTest, ExtensiveMiddle)
{
  // Use old PokerhandEvaluator to test FastPokerhandEvaluator's values.
  PokerHandEvaluator* old_evaluator = new PokerHandEvaluator(GameType::Regular);
  for (auto p : old_evaluator->middle_eval_info) {
    std::string row = p.first;
    PokerHandInfo expected_info = p.second;

    PokerHandInfo info = evalHand(row, Position::middle);
    ASSERT_EQ(PokerHandUtils::getRoyalties(info), PokerHandUtils::getRoyalties(expected_info)) << row;
    ASSERT_EQ(PokerHandUtils::getHandType(info), PokerHandUtils::getHandType(expected_info)) << row;
  }
}

TEST_F(FastPokerHandEvaluatorTest, ExtensiveTop)
{
  // Use old PokerhandEvaluator to test FastPokerhandEvaluator's values.
  PokerHandEvaluator* old_evaluator = new PokerHandEvaluator(GameType::Regular);
  for (auto p : old_evaluator->top_eval_info) {
    std::string row = p.first;
    PokerHandInfo expected_info = p.second;

    PokerHandInfo info = evalHand(row, Position::top);
    ASSERT_EQ(PokerHandUtils::getRoyalties(info), PokerHandUtils::getRoyalties(expected_info)) << row;
    ASSERT_EQ(PokerHandUtils::getHandType(info), PokerHandUtils::getHandType(expected_info)) << row;
  }
}

TEST_F(FastPokerHandEvaluatorTest, ExtensiveTopProgressive)
{
  // Use old PokerhandEvaluator to test FastPokerhandEvaluator's values.
  PokerHandEvaluator* old_evaluator = new PokerHandEvaluator(GameType::Progressive);
  for (auto p : old_evaluator->top_eval_info) {
    std::string row = p.first;
    PokerHandInfo expected_info = p.second;

    PokerHandInfo info = evaluator_progressive->evalTop(CardUtils::parseCards(row));
    ASSERT_EQ(PokerHandUtils::getRoyalties(info), PokerHandUtils::getRoyalties(expected_info)) << row;
    ASSERT_EQ(PokerHandUtils::getHandType(info), PokerHandUtils::getHandType(expected_info)) << row;
  }
}

TEST_F(FastPokerHandEvaluatorTest, ExtensiveUltimate)
{
  // Use old PokerhandEvaluator to test FastPokerhandEvaluator's values.
  PokerHandEvaluator* old_evaluator = new PokerHandEvaluator(GameType::Ultimate);
  for (auto p : old_evaluator->top_eval_info) {
    std::string row = p.first;
    PokerHandInfo expected_info = p.second;

    PokerHandInfo info = evaluator_ultimate->evalTop(CardUtils::parseCards(row));
    ASSERT_EQ(PokerHandUtils::getRoyalties(info), PokerHandUtils::getRoyalties(expected_info)) << row;
    ASSERT_EQ(PokerHandUtils::getHandType(info), PokerHandUtils::getHandType(expected_info)) << row;
  }
}
