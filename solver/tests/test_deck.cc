#include "gtest/gtest.h"
#include "absl/strings/str_split.h"
#include <vector>
#include <set>
#include <iostream>

#include "card.h"
#include "deck.h"
#include "game_state.h"

class DeckTest : public ::testing::Test {
 protected:
  void SetUp() override {
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
};

TEST_F(DeckTest, Basic) {
  Deck deck;
  ASSERT_EQ(deck.size(), 52);
}

TEST_F(DeckTest, CardRemoval) {
  Deck first_deck;
  first_deck.remove(CardUtils::parseCard("Ah"));
  ASSERT_EQ(first_deck.size(), 51);

  Deck second_deck(first_deck);
  ASSERT_EQ(second_deck.size(), 51);
}

TEST_F(DeckTest, GameStateRemoval) {
  Hand my_hand(
    parseCards("Ah"),
    parseCards("2d 2c"),
    parseCards("9d 9c")
  );
    
  Hand other_hand(
    parseCards("Qd Qc"),
    parseCards("4d"),
    parseCards("8c 8d")
  );

  std::vector<Hand> other_hands{other_hand};

  GameState state{
    my_hand,
    other_hands,
    Pull{parseCards("Jc Jd Js")},
    parseCards("Kd")
  };

  Deck deck(state);

  ASSERT_EQ(deck.size(), 52 - 14);
}

TEST_F(DeckTest, SampleDeck) {
  Deck deck;
  std::vector<Card> cards = deck.select(10);
  std::set<Card> card_set(cards.begin(), cards.end());

  ASSERT_EQ(cards.size(), 10); // test exactly 10 cards were returned
  ASSERT_EQ(card_set.size(), 10); // test 10 unique cards were returned
}
