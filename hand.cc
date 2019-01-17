#include <boost/algorithm/string.hpp>
#include <iostream>
#include "hand.h"

using namespace std;

Hand::Hand(vector<Card> _top, vector<Card> _middle, vector<Card> _bottom): top{_top}, middle{_middle}, bottom{_bottom} {
  _size = _top.size() + _middle.size() + _bottom.size();
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
