#include "card.h"

namespace CardUtils {
Card parseCard(std::string card_str) {
  unsigned short int suit;
  unsigned short int rank;

  switch (card_str[1]) {
    case 'd':
      suit = DIAMONDS;
      break;
    case 'c':
      suit = CLUBS;
      break;
    case 'h':
      suit = HEARTS;
      break;
    case 's':
      suit = SPADES;
      break;
    default:
      throw 1;
  }

  switch (card_str[0]) {
    case 'A':
      rank = 12;
      break;
    case 'K':
      rank = 11;
      break;
    case 'Q':
      rank = 10;
      break;
    case 'J':
      rank = 9;
      break;
    case 'T':
      rank = 8;
      break;
    default:
      rank = card_str[0] - '2';
      break;
  }

  Card card = 0;
  card |= (suit << 4);
  card |= rank;
  return card;
}

std::vector<Card> parseCards(std::string cards) {
  std::vector<Card> parsedCards;
  std::vector<std::string> tokens;
  boost::split(tokens, cards, boost::is_any_of(" "));
  for (auto token : tokens) {
    if (token.size() != 2) {
      throw std::runtime_error("Token size != 2!");
    }
    parsedCards.push_back(parseCard(token));
  }

  return parsedCards;
}

std::string cardToString(Card c) {
  char ranks[] = {'2', '3', '4', '5', '6', '7', '8',
                  '9', 'T', 'J', 'Q', 'K', 'A'};
  char suit = -1;

  unsigned short int suit_rep = (c & SUIT_MASK) >> 4;

  switch (suit_rep) {
    case DIAMONDS:
      suit = 'd';
      break;
    case CLUBS:
      suit = 'c';
      break;
    case HEARTS:
      suit = 'h';
      break;
    case SPADES:
      suit = 's';
      break;
  }

  char rank = ranks[c & RANK_MASK];
  std::string str = std::string() + rank + suit;
  return str;
}
}  // namespace CardUtils
