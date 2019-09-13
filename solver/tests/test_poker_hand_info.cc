#include "gtest/gtest.h"
#include "poker_hand_info.h"

TEST(PokerHandInfoTest, Basic) {
  PokerHandInfo info = CREATE_POKER_HAND_INFO(4000, 1, 20);
  ASSERT_EQ(GET_HAND_TYPE(info), 1);
  ASSERT_EQ(GET_OVERALL_RANK(info), 4000);
  ASSERT_EQ(GET_ROYALTIES(info), 20);
}
