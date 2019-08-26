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
  std::set<Card> parseCards(std::string cards) {
    std::set<Card> parsedCards;
    std::vector<std::string> tokens = absl::StrSplit(cards, " ");
    for (auto token : tokens) {
      assert(token.size() == 2);
      parsedCards.insert(Card(token));
    }

    return parsedCards;
  }

  // void TearDown() override {}
  PokerHandEvaluator *evaluator;
};

TEST_F(PokerHandEvaluatorTest, Basic) {
  std::set<Card> cards = parseCards("Ah Kh Qh Jh Th");
  PokerHandInfo *res = evaluator->eval(cards, Position::bottom);
  ASSERT_EQ(res->handType, PokerHandType::ROYAL_FLUSH);  
  ASSERT_EQ(res->royalties, 25);  
}
