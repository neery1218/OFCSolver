#ifndef _CARD_H_
#define _CARD_H_

#include <string>
#include <iostream>
#include <assert.h>
#include "absl/strings/str_split.h"

#define DIAMONDS       1
#define CLUBS          2
#define HEARTS         4
#define SPADES         8

#define SUIT_MASK   0xF0
#define RANK_MASK   0x0F

// [5-8: Suit Mask | 0-4: Rank]
typedef unsigned short int Card;

// never called in a high performance context. only for testing
class CardUtils{
    public:
	static std::vector<Card> parseCards(std::string cards) {
	    std::vector<Card> parsedCards;
	    std::vector<std::string> tokens = absl::StrSplit(cards, " ");
	    for (auto token : tokens) {
	      assert(token.size() == 2);
	      parsedCards.push_back(CardUtils::parseCard(token));
	    }

	    return parsedCards;
	}

        static Card parseCard(std::string card_str) {
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

        static std::string cardToString(Card c) {
          char ranks[] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
          char suit;

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
};
#endif
