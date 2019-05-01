#ifndef _POKER_HAND_EVALUATOR_
#define _POKER_HAND_EVALUATOR_

#include <vector>
#include <set>
#include <utility>
#include <unordered_map>
#include "card.h"
#include "gametype.h"
#include "position.h"

enum PokerHandType {
  HIGH_CARD = 0,
  PAIR = 1, 
  TWO_PAIR = 2, 
  TRIPS = 3,
  STRAIGHT = 4,
  FLUSH = 5,
  FULL_HOUSE = 6,
  QUADS = 7,
  STRAIGHT_FLUSH = 8,
  ROYAL_FLUSH = 9,
};

struct PokerHandInfo { // these names are gross
  int overallRank;
  PokerHandType handType;
  int royalties;
};

class PokerHandEvaluator {
  GameType gameType;

  public: 
    PokerHandEvaluator(GameType type);
    PokerHandEvaluator(const PokerHandEvaluator&) = delete;
    PokerHandEvaluator& operator=(PokerHandEvaluator&) = delete;
    ~PokerHandEvaluator();
    PokerHandInfo * eval(const std::set<Card> &completedRow, Position pos) const;
    PokerHandInfo * eval(const std::set<Card> &partialRow, const std::set<Card> &cards, Position pos) const;

  private:
    int calculateFantasyBonus(int overallRank) const;
    std::unordered_map<std::string, PokerHandInfo*> topEvalInfo;
    std::unordered_map<std::string, PokerHandInfo*> middleEvalInfo;
    std::unordered_map<std::string, PokerHandInfo*> bottomEvalInfo;
};
#endif
