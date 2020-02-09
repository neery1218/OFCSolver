#include "gtest/gtest.h"
#include <iostream>
#include <random>
#include <set>
#include <vector>

#include "card.h"
#include "deck.h"
#include "game_state.h"

class DeckTest : public ::testing::Test {
  protected:
  void SetUp() override
  {
  }
};

TEST_F(DeckTest, Basic)
{
  Deck deck;
  ASSERT_EQ(deck.size(), 52);
}

TEST_F(DeckTest, CardRemoval)
{
  Deck first_deck;
  first_deck.remove(CardUtils::parseCard("Ah"));
  ASSERT_EQ(first_deck.size(), 51);

  Deck second_deck(first_deck);
  ASSERT_EQ(second_deck.size(), 51);
}

TEST_F(DeckTest, GameStateRemoval)
{
  Hand my_hand(
      CardUtils::parseCards("Ah"),
      CardUtils::parseCards("2d 2c"),
      CardUtils::parseCards("9d 9c"));

  Hand other_hand(
      CardUtils::parseCards("Qd Qc"),
      CardUtils::parseCards("4d"),
      CardUtils::parseCards("8c 8d"));

  std::vector<Hand> other_hands { other_hand };

  GameState state {
    my_hand,
    other_hands,
    Pull { CardUtils::parseCards("Jc Jd Js") },
    CardUtils::parseCards("Kd")
  };

  Deck deck(state);

  ASSERT_EQ(deck.size(), 52 - 14);
}

TEST_F(DeckTest, SampleDeck)
{
  Deck deck;
  std::mt19937 rng { 1234 };
  std::vector<Card> cards = deck.select(10, &rng);
  std::set<Card> card_set(cards.begin(), cards.end());

  ASSERT_EQ(cards.size(), 10);    // test exactly 10 cards were returned
  ASSERT_EQ(card_set.size(), 10); // test 10 unique cards were returned
}
