#include "gtest/gtest.h"
#include "absl/strings/str_split.h"
#include <vector>
#include <set>

#include "poker_hand_evaluator.h"
#include "gametype.h"
#include "position.h"
#include "card.h"

class PokerHandEvaluatorTest : public ::testing::Test {
 protected:
  void SetUp() override { 
    evaluator = new PokerHandEvaluator(GameType::Regular);
  }
  std::vector<Card> parseCards(std::string cards) {
    std::vector<Card> cards;
    std::vector<std::string> tokens = absl::StrSplit(cards, " ");
    for (auto token : tokens) {
      assert(token.size() == 2);
      cards.push_back(parseCard(token));
    }

    return parsedCards;
  }

  // void TearDown() override {}
  PokerHandEvaluator *evaluator;
};

TEST_F(PokerHandEvaluatorTest, Basic) {
  std::vector<Card> cards = parseCards("As Kh Qh Jh Th");
  PokerHandInfo *res = evaluator->eval(cards, Position::bottom);
  ASSERT_EQ(res->handType, PokerHandType::STRAIGHT);  
  ASSERT_EQ(res->royalties, 25);  
}
