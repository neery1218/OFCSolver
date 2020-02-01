#include "fast_poker_hand_evaluator.h"
#include "poker_hand_info.h"
#include "gtest/gtest.h"

class PokerHandInfoTest : public ::testing::Test {
  protected:
  void SetUp() override
  {
    evaluator = new FastPokerHandEvaluator(GameType::Regular);
  }

  // void TearDown() override {}
  FastPokerHandEvaluator* evaluator;
};

TEST_F(PokerHandInfoTest, Basic)
{
  PokerHandInfo info = PokerHandUtils::createPokerHandInfo(15, 1, 20);
  ASSERT_EQ(PokerHandUtils::getHandType(info), 1);
  ASSERT_EQ(PokerHandUtils::getOverallRank(info), 0x01000F);
  ASSERT_EQ(PokerHandUtils::getRoyalties(info), 20);

  ASSERT_EQ(info, 0x01000F14);
}

TEST_F(PokerHandInfoTest, OverallRankInequalities)
{
  PokerHandInfo high_card = evaluator->evalBottom(CardUtils::parseCards("2h 3h As Kd Qd"));
  PokerHandInfo pair = evaluator->evalBottom(CardUtils::parseCards("As Ad Jd Kd Qd"));
  PokerHandInfo two_pair = evaluator->evalBottom(CardUtils::parseCards("2h 2d 3d 3h 4d"));
  PokerHandInfo trips = evaluator->evalBottom(CardUtils::parseCards("2h 2d 2s 4h 5h"));
  PokerHandInfo straight = evaluator->evalBottom(CardUtils::parseCards("2h 3h 4h 5h 6d"));
  PokerHandInfo flush = evaluator->evalBottom(CardUtils::parseCards("2h 3h Ah Kh Qh"));
  PokerHandInfo full_house = evaluator->evalBottom(CardUtils::parseCards("2h 2d 6h 6s 6d"));
  PokerHandInfo quads = evaluator->evalBottom(CardUtils::parseCards("2h Kd Ks Kc Kh"));
  PokerHandInfo straight_flush = evaluator->evalBottom(CardUtils::parseCards("2h 3h Ah 4h 5h"));
  PokerHandInfo royal_flush = evaluator->evalBottom(CardUtils::parseCards("Ah Kh Qh Jh Th"));

  ASSERT_LT(PokerHandUtils::getOverallRank(high_card), PokerHandUtils::getOverallRank(pair));
  ASSERT_LT(PokerHandUtils::getOverallRank(pair), PokerHandUtils::getOverallRank(two_pair));
  ASSERT_LT(PokerHandUtils::getOverallRank(two_pair), PokerHandUtils::getOverallRank(trips));
  ASSERT_LT(PokerHandUtils::getOverallRank(trips), PokerHandUtils::getOverallRank(straight));
  ASSERT_LT(PokerHandUtils::getOverallRank(straight), PokerHandUtils::getOverallRank(flush));
  ASSERT_LT(PokerHandUtils::getOverallRank(flush), PokerHandUtils::getOverallRank(full_house));
  ASSERT_LT(PokerHandUtils::getOverallRank(full_house), PokerHandUtils::getOverallRank(quads));
  ASSERT_LT(PokerHandUtils::getOverallRank(quads), PokerHandUtils::getOverallRank(straight_flush));
  ASSERT_LT(PokerHandUtils::getOverallRank(straight_flush), PokerHandUtils::getOverallRank(royal_flush));
}

TEST_F(PokerHandInfoTest, OverRankInequalitiesBottomMiddle)
{
  PokerHandInfo flush_middle = evaluator->evalMiddle(CardUtils::parseCards("2h 3h Ah Kh Qh"));
  PokerHandInfo flush_bottom = evaluator->evalBottom(CardUtils::parseCards("2h 3h Ah Kh Qh"));
  ASSERT_EQ(PokerHandUtils::getOverallRank(flush_middle), PokerHandUtils::getOverallRank(flush_bottom));
}
