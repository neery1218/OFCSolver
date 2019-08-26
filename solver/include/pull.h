#ifndef _PULL_H_
#define _PULL_H_

#include <set> // bad way of implementing Pull, as Pull only has 3 cards.
#include <string>
#include "card.h"

struct Pull {
  std::set<Card> cards;
};

std::ostream& operator<<(std::ostream& os, const Pull &p);

#endif
