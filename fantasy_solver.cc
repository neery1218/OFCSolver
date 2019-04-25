#include "fantasy_solver.h"

using namespace std;

FantasySolver::FantasySolver(): evaluator{PokerHandEvaluator(GameType::NoBonus)} {}

CompletedHand FantasySolver::solve(GameType type, set<Card> &cards) {
  return Hand().constructOptimalHand(cards, &evaluator); // doesn't take scooping into account
}
