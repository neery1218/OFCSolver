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

Hand::Hand() {
  _size = 0;
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
    os << CardUtils::cardToString(c) << " ";
  }
  if (hand.top.empty()) os << "x";
  os << endl;

  for (const Card &c : hand.middle) {
    os << CardUtils::cardToString(c) << " ";
  }
  if (hand.middle.empty()) os << "x";
  os << endl;

  for (const Card &c : hand.bottom) {
    os << CardUtils::cardToString(c) << " ";
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

CompletedHand Hand::constructOptimalHand(set<Card> &cards, const FastPokerHandEvaluator *evaluator) const {
  int topCardsMissing  = 3 - top.size();
  int middleCardsMissing  = 5 - middle.size();
  int bottomCardsMissing  = 5 - bottom.size();

  CompletedHand bestHand;
  int highestRoyalties = -1;

  vector< set<Card> > botCombos = comb(cards, bottomCardsMissing);
  if (botCombos.empty()) botCombos.push_back(set<Card>());

  for (auto &botCombo : botCombos) {
    vector<Card> completed_bottom(bottom.begin(), bottom.end());
    completed_bottom.insert(completed_bottom.end(), botCombo.begin(), botCombo.end());
    PokerHandInfo botInfo = evaluator->evalBottom(completed_bottom);

    if (_size <= 7 && highestRoyalties >= 0 && botInfo < CREATE_POKER_HAND_INFO(4118, PAIR, 0)) continue; // don't continue if bottom is less than KK

    set<Card> remainingCards;
    set_difference(
        cards.begin(), cards.end(), 
        botCombo.begin(), botCombo.end(), 
        inserter(remainingCards, remainingCards.begin()));

    vector< set<Card> > midCombos = comb(remainingCards, middleCardsMissing);
    if (midCombos.empty()) midCombos.push_back(set<Card>());

    for (auto &midCombo : midCombos) {
      vector<Card> completed_middle(middle.begin(), middle.end());
      completed_middle.insert(completed_middle.end(), midCombo.begin(), midCombo.end());
      PokerHandInfo midInfo = evaluator->evalMiddle(completed_middle);

      if (GET_OVERALL_RANK(botInfo) < GET_OVERALL_RANK(midInfo)) continue; // fouled hand
      if (_size <= 7 && highestRoyalties > (GET_ROYALTIES(botInfo) + GET_ROYALTIES(midInfo)) && midInfo < CREATE_POKER_HAND_INFO(3886, PAIR, 0)) continue; // don't continue if mid is less than 66

      set<Card> topRemainingCards;
      set_difference(remainingCards.begin(), remainingCards.end(), midCombo.begin(), 
          midCombo.end(), inserter(topRemainingCards, topRemainingCards.begin()));

      vector< set<Card> > topCombos = comb(topRemainingCards, topCardsMissing);
      if (topCombos.empty()) topCombos.push_back(set<Card>());

      for (auto &topCombo : topCombos) {
        vector<Card> completed_top(top.begin(), top.end());
        completed_top.insert(completed_top.end(), topCombo.begin(), topCombo.end());
        PokerHandInfo topInfo = evaluator->evalTop(completed_top);

        if (GET_OVERALL_RANK(midInfo) < GET_OVERALL_RANK(topInfo)) continue; // fouled hand

        int royalties = GET_ROYALTIES(topInfo) + GET_ROYALTIES(midInfo) + GET_ROYALTIES(botInfo);
        if (royalties > highestRoyalties) {
          highestRoyalties = royalties;
          bestHand = CompletedHand(topInfo, midInfo, botInfo);
        }
      }
    }
  }

  if (highestRoyalties == -1) { // all possible hands are fouled
    return CompletedHand(
      CREATE_POKER_HAND_INFO(0, 0, 0),
      CREATE_POKER_HAND_INFO(0, 0, 0),
      CREATE_POKER_HAND_INFO(0, 0, 0)
      );
  }

  return bestHand;
}

CompletedHand::CompletedHand(const Hand &h, const FastPokerHandEvaluator *eval) {
  topInfo = eval->evalTop(h.top);
  middleInfo = eval->evalMiddle(h.middle);
  bottomInfo = eval->evalBottom(h.bottom);

  if (topInfo > middleInfo || middleInfo > bottomInfo) {
    topInfo = CREATE_POKER_HAND_INFO(0, 0, 0);
    middleInfo = CREATE_POKER_HAND_INFO(0, 0, 0);
    bottomInfo = CREATE_POKER_HAND_INFO(0, 0, 0);
  }
}

int CompletedHand::calculatePoints() const { // note: can't call this with a fouled hand. tightly coupled with solver.h
  return GET_ROYALTIES(topInfo) + GET_ROYALTIES(middleInfo) + GET_ROYALTIES(bottomInfo);
}

int CompletedHand::calculatePoints(const CompletedHand &otherHand) const {
  int royaltyPoints = GET_ROYALTIES(topInfo) + GET_ROYALTIES(middleInfo) + GET_ROYALTIES(bottomInfo) - GET_ROYALTIES(otherHand.topInfo) - GET_ROYALTIES(otherHand.middleInfo) - GET_ROYALTIES(otherHand.bottomInfo);

  int gtBonus = 0;
  if (topInfo < otherHand.topInfo) gtBonus -= 1;
  else if (otherHand.topInfo < topInfo) gtBonus += 1;

  if (middleInfo < otherHand.middleInfo) gtBonus -= 1;
  else if (otherHand.middleInfo < middleInfo) gtBonus += 1;

  if (bottomInfo < otherHand.bottomInfo) gtBonus -= 1;
  else if (otherHand.bottomInfo < bottomInfo) gtBonus += 1;

  if (gtBonus == -3) gtBonus = -6;
  if (gtBonus == 3) gtBonus = 6;

  return royaltyPoints + gtBonus;
}
