#include <boost/algorithm/string.hpp>
#include <iostream>
#include "hand.h"

using namespace std;

Hand::Hand(set<Card> _top, set<Card> _middle, set<Card> _bottom): top{_top}, middle{_middle}, bottom{_bottom} {
  _size = _top.size() + _middle.size() + _bottom.size();
}

Hand::Hand(const Hand &obj) {
  top = obj.top;
  middle = obj.middle;
  bottom = obj.bottom;
  _size = obj._size;
}

Hand::Hand() {
  _size = 0;
}

void Hand::addTop(Card card) { 
  assert(top.size() < 3);
  top.insert(card); 
  _size++;
}

void Hand::addMiddle(Card card) { 
  assert(middle.size() < 5);
  middle.insert(card); 
  _size++;
}

void Hand::addBottom(Card card) { 
  assert(bottom.size() < 5);
  bottom.insert(card); 
  _size++;
}

int Hand::size() const {
  return _size;
}

Hand Hand::applyDecision(Decision decision) const {
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
  if (hand.top.empty()) os << "x";
  os << endl;

  for (const Card &c : hand.middle) {
    os << c;
  }
  if (hand.middle.empty()) os << "x";
  os << endl;

  for (const Card &c : hand.bottom) {
    os << c;
  }
  if (hand.bottom.empty()) os << "x";

  return os;
}

vector<set<Card>> comb(const set<Card> &_cards, int K)
{
  vector<Card> cards(_cards.begin(), _cards.end());
  std::string bitmask(K, 1); // K leading 1's
  bitmask.resize(cards.size(), 0); // N-K trailing 0's

  vector<set<Card>> out;
  // print integers and permute bitmask
  do {
    set<Card> tmp;

    for (unsigned int i = 0; i < cards.size(); ++i) // [0..N-1] integers
    {
      if (bitmask[i]) tmp.insert(cards[i]);
    }
    out.emplace_back(tmp); // TODO: verify move semantics
  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

  return out;
}

CompletedHand Hand::constructOptimalHand(set<Card> &cards, const PokerHandEvaluator *evaluator) const {
  int topCardsMissing  = 3 - top.size();
  int middleCardsMissing  = 5 - middle.size();
  int bottomCardsMissing  = 5 - bottom.size();

  CompletedHand bestHand;
  int highestRoyalties = -1;

  vector< set<Card> > botCombos = comb(cards, bottomCardsMissing);
  if (botCombos.empty()) botCombos.push_back(set<Card>());

  for (auto &botCombo : botCombos) {

    PokerHandInfo *botInfo = bottomCardsMissing > 0 ? evaluator->eval(bottom, botCombo, Position::bottom) : evaluator->eval(bottom, Position::bottom);

    if (_size <= 7 && highestRoyalties >= 0 && botInfo->overallRank < 4346) continue; // don't continue if bottom is less than KK

    set<Card> remainingCards;
    set_difference(
        cards.begin(), cards.end(), 
        botCombo.begin(), botCombo.end(), 
        inserter(remainingCards, remainingCards.begin()));

    vector< set<Card> > midCombos = comb(remainingCards, middleCardsMissing);
    if (midCombos.empty()) midCombos.push_back(set<Card>());

    for (auto &midCombo : midCombos) {

      PokerHandInfo * midInfo = middleCardsMissing > 0 ? evaluator->eval(middle, midCombo, Position::middle) : evaluator->eval(middle, Position::middle);

      if (botInfo->overallRank < midInfo->overallRank) continue; // fouled hand
      if (_size <= 7 && highestRoyalties > botInfo->royalties + midInfo->royalties && midInfo->overallRank < 2722) continue; // don't continue if mid is less than 66

      set<Card> topRemainingCards;
      set_difference(remainingCards.begin(), remainingCards.end(), midCombo.begin(), 
          midCombo.end(), inserter(topRemainingCards, topRemainingCards.begin()));

      vector< set<Card> > topCombos = comb(topRemainingCards, topCardsMissing);
      if (topCombos.empty()) topCombos.push_back(set<Card>());

      for (auto &topCombo : topCombos) {
        PokerHandInfo * topInfo = topCardsMissing > 0 ? evaluator->eval(top, topCombo, Position::top) : evaluator->eval(top, Position::top);

        if (midInfo->overallRank < topInfo->overallRank) continue; // fouled hand

        int royalties = topInfo->royalties + midInfo->royalties + botInfo->royalties;
        if (royalties > highestRoyalties) {
          highestRoyalties = royalties;
          bestHand = CompletedHand(*topInfo, *midInfo, *botInfo);
        }
      }
    }
  }

  if (highestRoyalties == -1) { // all possible hands are fouled
    return CompletedHand{
      PokerHandInfo{-1, PokerHandType::HIGH_CARD, 0},
        PokerHandInfo{-1, PokerHandType::HIGH_CARD, 0},
        PokerHandInfo{-1, PokerHandType::HIGH_CARD, 0}};
  }

  return bestHand;
}

CompletedHand::CompletedHand(const Hand &h, const PokerHandEvaluator *eval) {
  topInfo = *eval->eval(h.top, Position::top);
  middleInfo = *eval->eval(h.middle, Position::middle);
  bottomInfo = *eval->eval(h.bottom, Position::bottom);

  if (topInfo.overallRank > middleInfo.overallRank || middleInfo.overallRank > bottomInfo.overallRank) {
    topInfo = PokerHandInfo{-1, PokerHandType::HIGH_CARD, 0};
    middleInfo = PokerHandInfo{-1, PokerHandType::HIGH_CARD, 0};
    bottomInfo = PokerHandInfo{-1, PokerHandType::HIGH_CARD, 0};
  }
}

int CompletedHand::calculatePoints() const { // note: can't call this with a fouled hand. tightly coupled with solver.h
  return topInfo.royalties + middleInfo.royalties + bottomInfo.royalties;
}

int CompletedHand::calculatePoints(const CompletedHand &otherHand) const {

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
