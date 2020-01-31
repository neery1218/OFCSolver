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
  PokerHandInfo info = CREATE_POKER_HAND_INFO(15, 1, 20);
  ASSERT_EQ(GET_HAND_TYPE(info), 1);
  ASSERT_EQ(GET_OVERALL_RANK(info), 0x01000F);
  ASSERT_EQ(GET_ROYALTIES(info), 20);

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

  ASSERT_LT(GET_OVERALL_RANK(high_card), GET_OVERALL_RANK(pair));
  ASSERT_LT(GET_OVERALL_RANK(pair), GET_OVERALL_RANK(two_pair));
  ASSERT_LT(GET_OVERALL_RANK(two_pair), GET_OVERALL_RANK(trips));
  ASSERT_LT(GET_OVERALL_RANK(trips), GET_OVERALL_RANK(straight));
  ASSERT_LT(GET_OVERALL_RANK(straight), GET_OVERALL_RANK(flush));
  ASSERT_LT(GET_OVERALL_RANK(flush), GET_OVERALL_RANK(full_house));
  ASSERT_LT(GET_OVERALL_RANK(full_house), GET_OVERALL_RANK(quads));
  ASSERT_LT(GET_OVERALL_RANK(quads), GET_OVERALL_RANK(straight_flush));
  ASSERT_LT(GET_OVERALL_RANK(straight_flush), GET_OVERALL_RANK(royal_flush));
}

TEST_F(PokerHandInfoTest, OverRankInequalitiesBottomMiddle)
{
  PokerHandInfo flush_middle = evaluator->evalMiddle(CardUtils::parseCards("2h 3h Ah Kh Qh"));
  PokerHandInfo flush_bottom = evaluator->evalBottom(CardUtils::parseCards("2h 3h Ah Kh Qh"));
  ASSERT_EQ(GET_OVERALL_RANK(flush_middle), GET_OVERALL_RANK(flush_bottom));
}
