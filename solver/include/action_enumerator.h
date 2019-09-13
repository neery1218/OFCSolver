#ifndef _ACTION_ENUMERATOR_
#define _ACTION_ENUMERATOR_

#include "game_state.h"
#include "placement.h"
#include <set>
#include <vector>

namespace ActionEnumerator {
    std::vector<Decision> findAllSetDecisions(const GameState &game_state);
    std::vector<Decision> findAllDrawDecisions(const GameState &game_state);
    std::vector<Decision> findAllSetDecisionsHelper(const std::set<Card> &cards, const std::vector<Placement> &acc);
};
#endif
