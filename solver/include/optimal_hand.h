#include "card.h"
#include "completed_hand.h"
#include "fast_poker_hand_evaluator.h"
#include "hand.h"
#include <set>

// given a set of cards and a hand, construct the best hand possible
namespace OptimalHand {
CompletedHand constructOptimalHand(const Hand& h, std::set<Card>& cards, const FastPokerHandEvaluator* evaluator);
}
