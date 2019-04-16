#include <boost/algorithm/string.hpp>
#include <iostream>
#include "hand.h"

using namespace std;

Hand::Hand(vector<Card> _top, vector<Card> _middle, vector<Card> _bottom): top{_top}, middle{_middle}, bottom{_bottom} {
  _size = _top.size() + _middle.size() + _bottom.size();
}

Hand::Hand(const Hand &obj) {
  top = obj.top;
  middle = obj.middle;
  bottom = obj.bottom;
  _size = obj._size;
}

void Hand::addTop(Card card) { 
  assert(top.size() < 3);
  top.push_back(card); 
  _size++;
}

void Hand::addMiddle(Card card) { 
  assert(middle.size() < 5);
  middle.push_back(card); 
  _size++;
}

void Hand::addBottom(Card card) { 
  assert(bottom.size() < 5);
  bottom.push_back(card); 
  _size++;
}

int Hand::size() {
  return _size;
}

Hand Hand::applyDecision(Decision decision) {
  Hand newHand(*this);
  for (auto &placement : decision.placements) {
    if (placement.position == Position::top) { newHand.addTop(placement.card); }
    else if (placement.position == Position::middle) { newHand.addMiddle(placement.card); }
    else if (placement.position == Position::bottom) { newHand.addBottom(placement.card); }
  }

  return newHand;
}

ostream& operator<<(ostream& os, const Hand& hand) {
  for (const Card &c : hand.top) {
    os << c;
  }

  os << endl;

  for (const Card &c : hand.middle) {
    os << c;
  }

  os << endl;

  for (const Card &c : hand.bottom) {
    os << c;
  }

  return os;
}

CompletedHand Hand::constructOptimalHand(vector<Card> &cards, PokerHandEvaluator * pokerHandEvaluator) {
  /* This is inefficient af. More research needs to be done here. */
  sort(cards.begin(), cards.end()); // must sort to use next_permutation.

  int topCardsMissing  = 3 - top.size();
  int middleCardsMissing  = 5 - middle.size();
  int bottomCardsMissing  = 5 - bottom.size();

  CompletedHand bestHand;
  int highestRoyalties = -1;

  do {
    Hand tmp(*this); // expensive copy.

    tmp.top.insert(tmp.top.end(), cards.begin(), cards.begin() + topCardsMissing);
    tmp.middle.insert(tmp.middle.end(), cards.begin() + topCardsMissing,
       cards.begin() + topCardsMissing + middleCardsMissing);
       

    PokerHandInfo topInfo = pokerHandEvaluator->eval(tmp.top, Position::top);
    PokerHandInfo middleInfo = pokerHandEvaluator->eval(tmp.middle, Position::middle);
    if (middleInfo.overallRank < topInfo.overallRank) continue; // fouled hand

    tmp.bottom.insert(tmp.bottom.end(), cards.begin() + topCardsMissing + middleCardsMissing,
        cards.begin() + topCardsMissing + middleCardsMissing + bottomCardsMissing);     

    PokerHandInfo bottomInfo = pokerHandEvaluator->eval(tmp.bottom, Position::bottom);
    if (bottomInfo.overallRank < middleInfo.overallRank) continue; // fouled hand

    int royalties = topInfo.royalties + middleInfo.royalties + bottomInfo.royalties;
    if (royalties > highestRoyalties) {
      highestRoyalties = royalties;
      bestHand = CompletedHand(tmp, topInfo, middleInfo, bottomInfo);
    }
  } while (next_permutation(cards.begin(), cards.end()));

  return bestHand;
}

int CompletedHand::calculatePoints(const CompletedHand &otherHand) {
  int royaltyPoints = topInfo.royalties + middleInfo.royalties + bottomInfo.royalties - otherHand.topInfo.royalties - otherHand.middleInfo.royalties - otherHand.bottomInfo.royalties;

  int gtBonus = 0;

  if (topInfo.overallRank < otherHand.topInfo.overallRank) gtBonus -= 1;
  else if (otherHand.topInfo.overallRank < topInfo.overallRank) gtBonus += 1;
  
  if (middleInfo.overallRank < otherHand.middleInfo.overallRank) gtBonus -= 1;
  else if (otherHand.middleInfo.overallRank < middleInfo.overallRank) gtBonus += 1;

  if (bottomInfo.overallRank < otherHand.bottomInfo.overallRank) gtBonus -= 1;
  else if (otherHand.bottomInfo.overallRank < bottomInfo.overallRank) gtBonus += 1;

  if (gtBonus == -3) gtBonus = -6;
  if (gtBonus == 3) gtBonus = 6;

  return royaltyPoints + gtBonus;
}

  
