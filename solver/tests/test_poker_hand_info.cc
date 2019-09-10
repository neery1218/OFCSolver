#include "gtest/gtest.h"
#include "poker_hand_info.h"

TEST(PokerHandInfoTest, Basic) {
  PokerHandInfo info = PokerHandInfoUtils::createPokerHandInfo(4000, 1, 20);
  ASSERT_EQ(PokerHandInfoUtils::getHandType(info), 1);
  ASSERT_EQ(PokerHandInfoUtils::getRank(info), 4000);
  ASSERT_EQ(PokerHandInfoUtils::getRoyalties(info), 20);
}
