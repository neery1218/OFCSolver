#ifndef _CARD_H_
#define _CARD_H_

#include <string>
#include <iostream>

class Card {
  int val;

  public:
    Card(std::string token);
    Card(int val);
    friend std::ostream& operator<<(std::ostream& os, const Card& c);

  private:
    int parseSuit(char suit);
    int parseRank(char rank);
    char toSuit() const;
    char toRank() const;
};

#endif
