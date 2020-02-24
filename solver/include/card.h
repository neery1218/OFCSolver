#ifndef _CARD_H_
#define _CARD_H_

#include <assert.h>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <vector>

#define DIAMONDS 1
#define CLUBS 2
#define HEARTS 4
#define SPADES 8

#define SUIT_MASK 0xF0
#define RANK_MASK 0x0F

// [5-8: Suit Mask | 0-4: Rank]
typedef unsigned short int Card;

// never called in production. only for testing
namespace CardUtils {

Card parseCard(std::string card_str);
std::vector<Card> parseCards(std::string cards);
std::string cardToString(Card c);
}
#endif
