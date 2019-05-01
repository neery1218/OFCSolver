#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include "hand.h"
#include "card.h"
#include "pull.h"
#include <vector>

struct GameState {
  const Hand my_hand;
  const std::vector<Hand> other_hands;
  const Pull my_pull;
  const std::vector<Card> dead_cards;
};

#endif
