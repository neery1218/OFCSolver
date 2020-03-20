#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include <vector>

#include "card.h"
#include "hand.h"
#include "pull.h"

struct GameState {
  const Hand my_hand;
  const std::vector<Hand> other_hands;
  const Pull my_pull;
  const std::vector<Card> dead_cards;
  const int n_solves;
  const int n_decision_solves;
};

#endif
