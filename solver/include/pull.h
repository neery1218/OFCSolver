#ifndef _PULL_H_
#define _PULL_H_

#include "card.h"
#include <string>

struct Pull {
  std::vector<Card> cards;
};

std::ostream& operator<<(std::ostream& os, const Pull& p);

#endif
