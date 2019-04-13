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
    if (placement.position == Position::Top) { newHand.addTop(placement.card); }
    else if (placement.position == Position::Middle) { newHand.addBottom(placement.card); }
    else if (placement.position == Position::Bottom) { newHand.addBottom(placement.card); }
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

vector< vector<int> > toIntegerRepresentation (Hand &h) {
  vector< vector<int> > out(3, vector<int>());

  for (auto &card : h.top) { out[0].push_back(card.getVal()); }
  for (auto &card : h.middle) { out[1].push_back(card.getVal()); }
  for (auto &card : h.bottom) { out[2].push_back(card.getVal()); }

  return out;
}

Hand Hand::constructOptimalHand(vector<Card> cards) {
  return *this;
}

int Hand::calculatePoints(std::vector<Hand> otherHands) {
  int total = 0;
  for (auto &hand : otherHands) {
    total += calculatePoints(hand);
  }

  return total;
}

int Hand::calculatePoints(Hand otherHand) { return 0; } // TODO: finish dat shit
