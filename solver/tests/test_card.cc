#include "gtest/gtest.h"
#include "absl/strings/str_split.h"
#include <vector>
#include <set>
#include <iostream>

#include "gametype.h"
#include "card.h"

TEST(CardTest, Basic) {
  char suits[] = {'d', 'c', 'h', 's'};
  char ranks[] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
  for (auto suit : suits) {
    for (auto rank : ranks) {
        std::string card_str = std::string() + rank + suit;
        Card c = CardUtils::parseCard(card_str);
        std::string parsed_str = CardUtils::cardToString(c);
        ASSERT_EQ(card_str, parsed_str);
    }
  }
}
