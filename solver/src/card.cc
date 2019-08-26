#include "card.h"

using namespace std; 

Card::Card(string _val): val{_val} {}
/*
int Card::parseSuit(char suit) {
  switch(suit) {
    case 'c': 
      return 0;
    case 'd':
      return 1;
    case 'h':
      return 2;
    case 's':
      return 3;
    default:
      throw 1;
  }
}

int Card::parseRank(char rank) {
  switch (rank) {
    case 'A': 
      return 12;
    case 'K':
      return 11;
    case 'Q':
      return 10;
    case 'J':
      return 9;
    case 'T':
      return 8;
    case '9': 
      return 7;
    case '8':
      return 6;
    case '7':
      return 5;
    case '6':
      return 4;
    case '5':
      return 3;
    case '4':
      return 2;
    case '3':
      return 1;
    case '2':
      return 0;
    default:
      throw 1;
  }
}

char Card::toSuit() const {
  int quot = val % 4;
  switch (quot) {
    case 0:
      return 'd';
    case 1:
      return 'c';
    case 2:
      return 'h';
    case 3:
      return 's';
    default:
      throw 1;
  }
}

char Card::toRank() const {
  int rem = val / 4;
  if (rem <= 8) {
    return '0' + rem + 2;
  }

  switch (rem) {
    case 12:
      return 'A';
    case 11:
      return 'K';
    case 10:
      return 'Q';
    case 9:
      return 'J';
    default:
      throw 1;
  }
}
*/
bool Card::operator <(const Card &obj) const {
  return val < obj.val;
}

std::ostream& operator<<(std::ostream& os, const Card& c) {
  os << " " << c.val;
  return os;
}

string to_string(const Card &c) {
  return c.val;
}
