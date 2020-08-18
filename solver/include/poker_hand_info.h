#ifndef _POKER_HAND_INFO_
#define _POKER_HAND_INFO_

#include <cstdint>

enum class HandType {
  HIGH_CARD = 0,
  PAIR = 1,
  TWO_PAIR = 2,
  TRIPS = 3,
  STRAIGHT = 4,
  FLUSH = 5,
  FULL_HOUSE = 6,
  QUADS = 7,
  STRAIGHT_FLUSH = 8,
  ROYAL_FLUSH = 9
};

// yeah, enums are sketch. ill fix this later.
/*
#define HIGH_CARD 0
#define PAIR 1
#define TWO_PAIR 2
#define TRIPS 3
#define STRAIGHT 4
#define FLUSH 5
#define FULL_HOUSE 6
#define QUADS 7
#define STRAIGHT_FLUSH 8
#define ROYAL_FLUSH 9
*/

// [24-31: HandType | 8-23: OverallRank | 0-7: ROYALTY_MASK]
typedef unsigned int PokerHandInfo;

namespace PokerHandUtils {

const int ROYALTY_TYPE_SHIFT = 0;
const int HAND_TYPE_SHIFT = 24;
const int RANK_TYPE_SHIFT = 8;

const int ROYALTY_MASK = 0x000000FF;
const int HAND_TYPE_MASK = 0xFF000000;
const int OVERALL_RANK_MASK = 0x00FFFF00;

constexpr PokerHandInfo createPokerHandInfo(int rank, int hand_type,
                                            int royalties) {
  return 0 | (rank << RANK_TYPE_SHIFT) | (hand_type << HAND_TYPE_SHIFT) |
         (royalties << ROYALTY_TYPE_SHIFT);
}

constexpr unsigned int getRoyalties(PokerHandInfo info) {
  return (info & ROYALTY_MASK) >> ROYALTY_TYPE_SHIFT;
}

constexpr unsigned int getHandType(PokerHandInfo info) {
  return (info & HAND_TYPE_MASK) >> HAND_TYPE_SHIFT;
}

constexpr unsigned int getOverallRank(PokerHandInfo info) {
  return (info & OVERALL_RANK_MASK) >> RANK_TYPE_SHIFT;
}
}  // namespace PokerHandUtils
#endif
