#ifndef _POKER_HAND_INFO_
#define _POKER_HAND_INFO_

// yeah, enums are sketch. ill fix this later.
#define HIGH_CARD      0
#define PAIR           1
#define TWO_PAIR       2
#define TRIPS          3
#define STRAIGHT       4
#define FLUSH          5
#define FULL_HOUSE     6
#define QUADS          7
#define STRAIGHT_FLUSH 8
#define ROYAL_FLUSH    9

#define ROYALTY_TYPE_SHIFT     0
#define HAND_TYPE_SHIFT        24
#define RANK_TYPE_SHIFT        8

#define ROYALTY_MASK           0x000000FF
#define HAND_TYPE_MASK         0xFF000000
#define OVERALL_RANK_MASK      0xFFFFFF00

#define CREATE_POKER_HAND_INFO(rank, hand_type, royalties) (0 | (rank << RANK_TYPE_SHIFT) | (hand_type << HAND_TYPE_SHIFT) | (royalties << ROYALTY_TYPE_SHIFT))
#define GET_ROYALTIES(info) ((info & ROYALTY_MASK) >> ROYALTY_TYPE_SHIFT)
#define GET_HAND_TYPE(info) ((info & HAND_TYPE_MASK) >> HAND_TYPE_SHIFT)
#define GET_OVERALL_RANK(info) ((info & OVERALL_RANK_MASK) >> RANK_TYPE_SHIFT)

// [24-31: HandType | 8-23: OverallRank | 0-7: ROYALTY_MASK]
typedef unsigned int PokerHandInfo; 
#endif
