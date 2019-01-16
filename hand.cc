#include <boost/algorithm/string.hpp>
#include <iostream>
#include "hand.h"

using namespace std;

Hand::Hand(vector<Card> _top, vector<Card> _middle, vector<Card> _bottom): top{_top}, middle{_middle}, bottom{_bottom}  {}

void Hand::addTop(Card card) { 
  assert(top.size() < 3);
  top.push_back(card); 
}

void Hand::addMiddle(Card card) { 
  assert(middle.size() < 5);
  middle.push_back(card); 
}

void Hand::addBottom(Card card) { 
  assert(bottom.size() < 5);
  bottom.push_back(card); 
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
