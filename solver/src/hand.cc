#include "hand.h"

#include <iostream>

#include "completed_hand.h"

using namespace std;

Hand::Hand(vector<Card> _top, vector<Card> _middle, vector<Card> _bottom)
    : top{_top}, middle{_middle}, bottom{_bottom} {
  _size = _top.size() + _middle.size() + _bottom.size();
}

Hand::Hand(const Hand &obj) {
  top = obj.top;
  middle = obj.middle;
  bottom = obj.bottom;
  _size = obj._size;
}

Hand::Hand() { _size = 0; }

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

int Hand::size() const { return _size; }

Hand Hand::applyDecision(Decision decision) const {
  Hand newHand(*this);
  for (auto &placement : decision.placements) {
    if (placement.position == Position::top) {
      newHand.addTop(placement.card);
    } else if (placement.position == Position::middle) {
      newHand.addMiddle(placement.card);
    } else if (placement.position == Position::bottom) {
      newHand.addBottom(placement.card);
    }
  }

  return newHand;
}

ostream &operator<<(ostream &os, const Hand &hand) {
  for (const Card &c : hand.top) {
    os << CardUtils::cardToString(c);
  }

  os << "-";

  for (const Card &c : hand.middle) {
    os << CardUtils::cardToString(c);
  }

  os << "-";

  for (const Card &c : hand.bottom) {
    os << CardUtils::cardToString(c);
  }

  return os;
}
