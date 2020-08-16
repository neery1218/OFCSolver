#ifndef _ACTION_ENUMERATOR_
#define _ACTION_ENUMERATOR_

#include <set>
#include <vector>

#include "fast_poker_hand_evaluator.h"
#include "game_state.h"
#include "placement.h"

namespace ActionEnumerator {
std::vector<Decision> findAllSetDecisions(const GameState &game_state);
std::vector<Decision> findAllDrawDecisions(
    const GameState &game_state, const FastPokerHandEvaluator *evaluator);
std::vector<Decision> findAllSetDecisionsHelper(
    const std::set<Card> &cards, const std::vector<Placement> &acc);
};  // namespace ActionEnumerator
#endif
