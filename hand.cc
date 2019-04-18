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

vector<set<Card>> comb(set<Card> &_cards, int K)
{
    vector<Card> cards(_cards.begin(), _cards.end());
    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(cards.size(), 0); // N-K trailing 0's

    vector<set<Card>> out;
    // print integers and permute bitmask
    do {
      set<Card> tmp; 

      for (int i = 0; i < cards.size(); ++i) // [0..N-1] integers
      {
          if (bitmask[i]) tmp.insert(cards[i]);
      }
      out.emplace_back(tmp); // TODO: verify move semantics
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

    return out; // TODO: move semantics?
}

CompletedHand Hand::constructOptimalHand(set<Card> &cards, PokerHandEvaluator * pokerHandEvaluator) {
  int topCardsMissing  = 3 - top.size();
  int middleCardsMissing  = 5 - middle.size();
  int bottomCardsMissing  = 5 - bottom.size();

  CompletedHand bestHand;
  int highestRoyalties = -1;

  vector< set<Card> > topCombos = comb(cards, topCardsMissing);
  if (topCombos.empty()) topCombos.push_back(set<Card>());

  for (auto &topCombo : topCombos) {

    PokerHandInfo * topInfo = topCardsMissing > 0 ? pokerHandEvaluator->eval(top, topCombo, Position::top) : pokerHandEvaluator->eval(top, Position::top);
    set<Card> remainingCards;

    set_difference(cards.begin(), cards.end(), topCombo.begin(), topCombo.end(), inserter(remainingCards, remainingCards.begin()));

    vector< set<Card> > midCombos = comb(remainingCards, middleCardsMissing);
    if (midCombos.empty()) midCombos.push_back(set<Card>());

    for (auto &midCombo : midCombos) {

      PokerHandInfo * middleInfo = pokerHandEvaluator->eval(middle, midCombo, Position::middle);
      if (middleInfo->overallRank < topInfo->overallRank) continue; // fouled hand

      set<Card> bottomRemainingCards;
      set_difference(remainingCards.begin(), remainingCards.end(), midCombo.begin(), 
          midCombo.end(), inserter(bottomRemainingCards, bottomRemainingCards.begin()));

      vector< set<Card> > botCombos = comb(bottomRemainingCards, bottomCardsMissing);
      if (botCombos.empty()) botCombos.push_back(set<Card>());

      for (auto &botCombo : botCombos) {
        PokerHandInfo * bottomInfo = pokerHandEvaluator->eval(bottom, botCombo, Position::bottom);
        if (bottomInfo->overallRank < middleInfo->overallRank) continue; // fouled hand

        int royalties = topInfo->royalties + middleInfo->royalties + bottomInfo->royalties;
        if (royalties > highestRoyalties) {
          highestRoyalties = royalties;
          bestHand = CompletedHand(*this, topInfo, middleInfo, bottomInfo);
        }
      }
    }
  }

  return bestHand;
}

int CompletedHand::calculatePoints(const CompletedHand &otherHand) {
  int royaltyPoints = topInfo->royalties + middleInfo->royalties + bottomInfo->royalties - otherHand.topInfo->royalties - otherHand.middleInfo->royalties - otherHand.bottomInfo->royalties;

  int gtBonus = 0;

  if (topInfo->overallRank < otherHand.topInfo->overallRank) gtBonus -= 1;
  else if (otherHand.topInfo->overallRank < topInfo->overallRank) gtBonus += 1;
  
  if (middleInfo->overallRank < otherHand.middleInfo->overallRank) gtBonus -= 1;
  else if (otherHand.middleInfo->overallRank < middleInfo->overallRank) gtBonus += 1;

  if (bottomInfo->overallRank < otherHand.bottomInfo->overallRank) gtBonus -= 1;
  else if (otherHand.bottomInfo->overallRank < bottomInfo->overallRank) gtBonus += 1;

  if (gtBonus == -3) gtBonus = -6;
  if (gtBonus == 3) gtBonus = 6;

  return royaltyPoints + gtBonus;
}

  
