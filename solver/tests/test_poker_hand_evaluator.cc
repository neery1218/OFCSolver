#include "gtest/gtest.h"
#include <set>
#include <vector>

#include "card.h"
#include "gametype.h"
#include "poker_hand_evaluator.h"
#include "position.h"

class PokerHandEvaluatorTest : public ::testing::Test {
  protected:
  void SetUp() override
  {
    evaluator = new PokerHandEvaluator(GameType::Regular);
  }

  // void TearDown() override {}
  PokerHandEvaluator* evaluator;
};

TEST_F(PokerHandEvaluatorTest, Basic)
{
  std::vector<Card> cards = CardUtils::parseCards("As Kh Qh Jh Th");
  PokerHandInfo* res = evaluator->eval(cards, Position::bottom);
  ASSERT_EQ(res->handType, PokerHandType::STRAIGHT);
  ASSERT_EQ(res->royalties, 25);
}
