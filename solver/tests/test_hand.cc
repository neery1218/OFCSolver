#include "gtest/gtest.h"

#include "card.h"
#include "fast_poker_hand_evaluator.h"
#include "gametype.h"
#include "hand.h"
#include "position.h"
#include "pull.h"
#include "solver.h"

class HandTest : public ::testing::Test {
  protected:
  void SetUp() override
  {
    evaluator = new FastPokerHandEvaluator(GameType::Ultimate);
  }

  // void TearDown() override {}
  FastPokerHandEvaluator* evaluator;
};

TEST_F(HandTest, BasicCalculatePoints)
{
  CompletedHand my_hand(
      evaluator->evalTop(CardUtils::parseCards("Ac Kd Qd")),
      evaluator->evalMiddle(CardUtils::parseCards("2d 2c 3h 3d 4h")),
      evaluator->evalBottom(CardUtils::parseCards("4s 5s 6h 7d 8d")));

  CompletedHand other_hand(
      evaluator->evalTop(CardUtils::parseCards("Jd Td 9d")),
      evaluator->evalMiddle(CardUtils::parseCards("Ad Ah 2s 3s 8s")),
      evaluator->evalBottom(CardUtils::parseCards("5s 5h 6d 6s 9s")));

  ASSERT_EQ(my_hand.calculatePoints(other_hand), 8);
  ASSERT_EQ(other_hand.calculatePoints(my_hand), -8);
}

TEST_F(HandTest, ConstructOptimalFantasyHand)
{
  Hand hand(
      CardUtils::parseCards("Ac"),
      CardUtils::parseCards("2c 2d 3d 4s"),
      CardUtils::parseCards("9h 9d 9c 9s"));

  std::vector<Card> cards = CardUtils::parseCards("Ad Kc Ks 2s 3s 4d");
  std::set<Card> card_set(cards.begin(), cards.end());
  CompletedHand completedHand = hand.constructOptimalHand(card_set, evaluator);

  ASSERT_EQ(GET_ROYALTIES(completedHand.topInfo), 34);
  ASSERT_EQ(GET_ROYALTIES(completedHand.middleInfo), 2);
  ASSERT_EQ(GET_ROYALTIES(completedHand.bottomInfo), 10);
}

TEST_F(HandTest, ConstructOptimalHand2)
{
  Hand hand(
      CardUtils::parseCards("Ad"),
      CardUtils::parseCards("5s 2c"),
      CardUtils::parseCards("Td 9d"));

  std::vector<Card> cards = CardUtils::parseCards("Kd Qd 4d 5c 2s Kh Ks Jh As");
  std::set<Card> card_set(cards.begin(), cards.end());
  CompletedHand completedHand = hand.constructOptimalHand(card_set, evaluator);

  ASSERT_EQ(GET_ROYALTIES(completedHand.bottomInfo), 4);
  ASSERT_EQ(GET_ROYALTIES(completedHand.middleInfo), 0);
  ASSERT_EQ(GET_ROYALTIES(completedHand.topInfo), 34);
}

TEST_F(HandTest, FouledHand)
{
  Hand hand(
      CardUtils::parseCards("Ad Ac Qh"),
      CardUtils::parseCards("5s 2c 2d 3s 4s"),
      CardUtils::parseCards("Td"));

  std::vector<Card> cards = CardUtils::parseCards("Kd Qd 4d 5c 2s Kh Ks Jh As");
  std::set<Card> card_set(cards.begin(), cards.end());
  CompletedHand completedHand = hand.constructOptimalHand(card_set, evaluator);

  ASSERT_EQ(GET_ROYALTIES(completedHand.topInfo), 0);
  ASSERT_EQ(GET_OVERALL_RANK(completedHand.topInfo), 0);
  ASSERT_EQ(GET_ROYALTIES(completedHand.middleInfo), 0);
  ASSERT_EQ(GET_OVERALL_RANK(completedHand.middleInfo), 0);
  ASSERT_EQ(GET_ROYALTIES(completedHand.bottomInfo), 0);
  ASSERT_EQ(GET_OVERALL_RANK(completedHand.bottomInfo), 0);
}

TEST_F(HandTest, Regression)
{
  Hand hand;

  std::vector<Card> cards = CardUtils::parseCards("5h 6h 7h 8h 9h 5d 6d 7d 8d 9d Ad Ac Ah");
  std::set<Card> card_set(cards.begin(), cards.end());
  CompletedHand completedHand = hand.constructOptimalHand(card_set, evaluator);

  ASSERT_EQ(GET_ROYALTIES(completedHand.topInfo), 22 + 25);
  ASSERT_EQ(GET_ROYALTIES(completedHand.middleInfo), 30);
  ASSERT_EQ(GET_ROYALTIES(completedHand.bottomInfo), 15);
}
