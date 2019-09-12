#include "gtest/gtest.h"
#include "absl/strings/str_split.h"

#include "fast_poker_hand_evaluator.h"
#include "gametype.h"
#include "position.h"
#include "card.h"
#include "hand.h"
#include "solver.h"
#include "pull.h"

class HandTest : public ::testing::Test {
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

TEST_F(HandTest, BasicCalculatePoints) {
  CompletedHand my_hand(
      evaluator->evalTop(parseCards("Ac Kd Qd")),
      evaluator->evalMiddle(parseCards("2d 2c 3h 3d 4h")),
      evaluator->evalBottom(parseCards("4s 5s 6h 7d 8d"))
  );

  CompletedHand other_hand(
      evaluator->evalTop(parseCards("Jd Td 9d")),
      evaluator->evalMiddle(parseCards("Ad Ah 2s 3s 8s")),
      evaluator->evalBottom(parseCards("5s 5h 6d 6s 9s"))
  );

  ASSERT_EQ(my_hand.calculatePoints(other_hand), 8);
  ASSERT_EQ(other_hand.calculatePoints(my_hand), -8);

}

TEST_F(HandTest, ConstructOptimalFantasyHand) {
  Hand hand(
      parseCards("Ac"),
      parseCards("2c 2d 3d 4s"),
      parseCards("9h 9d 9c 9s")
  );

  std::vector<Card> cards = parseCards("Ad Kc Ks 2s 3s 4d");
  std::set<Card> card_set(cards.begin(), cards.end());
  CompletedHand completedHand = hand.constructOptimalHand(card_set, evaluator);

  ASSERT_EQ(GET_ROYALTIES(completedHand.topInfo), 34);
  ASSERT_EQ(GET_ROYALTIES(completedHand.middleInfo), 2);
  ASSERT_EQ(GET_ROYALTIES(completedHand.bottomInfo), 10);

}

TEST_F(HandTest, ConstructOptimalHand2) {
  Hand hand(
      parseCards("Ad"),
      parseCards("5s 2c"),
      parseCards("Td 9d"));

  std::vector<Card> cards = parseCards("Kd Qd 4d 5c 2s Kh Ks Jh As");
  std::set<Card> card_set(cards.begin(), cards.end());
  CompletedHand completedHand = hand.constructOptimalHand(card_set, evaluator);

  ASSERT_EQ(GET_ROYALTIES(completedHand.bottomInfo), 4);
  ASSERT_EQ(GET_ROYALTIES(completedHand.middleInfo), 0);
  ASSERT_EQ(GET_ROYALTIES(completedHand.topInfo), 34); 
}

TEST_F(HandTest, FouledHand) {
  Hand hand(
      parseCards("Ad Ac Qh"),
      parseCards("5s 2c 2d 3s 4s"),
      parseCards("Td"));

  std::vector<Card> cards = parseCards("Kd Qd 4d 5c 2s Kh Ks Jh As");
  std::set<Card> card_set(cards.begin(), cards.end());
  CompletedHand completedHand = hand.constructOptimalHand(card_set, evaluator);

  ASSERT_EQ(GET_ROYALTIES(completedHand.topInfo) ,0 );
  ASSERT_EQ(GET_OVERALL_RANK(completedHand.topInfo), 0);
  ASSERT_EQ(GET_ROYALTIES(completedHand.middleInfo), 0);
  ASSERT_EQ(GET_OVERALL_RANK(completedHand.middleInfo), 0);
  ASSERT_EQ(GET_ROYALTIES(completedHand.bottomInfo), 0);
  ASSERT_EQ(GET_OVERALL_RANK(completedHand.bottomInfo), 0);

}
