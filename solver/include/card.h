#ifndef _CARD_H_
#define _CARD_H_

#include <string>
#include <iostream>

struct Card {
  std::string val;

  public:
    Card(std::string token);
    friend std::ostream& operator<<(std::ostream& os, const Card& c);
    friend std::string to_string(const Card &c);
    bool operator <(const Card &obj) const;

  private:
    /*
    int parseSuit(char suit);
    int parseRank(char rank);
    char toSuit() const;
    char toRank() const;
    */
};
#endif
