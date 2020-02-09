#include "decision_finder.h"
#include "action_enumerator.h"
#include "solver.h"
#include <algorithm>
#include <cassert>
#include <future>
#include <utility>

using namespace std;

DecisionFinder::DecisionFinder(const FastPokerHandEvaluator* t_evaluator, std::mt19937* rng)
    : evaluator { t_evaluator }
    , rng { rng }
{
}

Decision DecisionFinder::findBestDecision(const GameState& game_state, int iterations)
{

  vector<Decision> all_decisions = (game_state.my_hand.size() > 0) ? ActionEnumerator::findAllDrawDecisions(game_state) : ActionEnumerator::findAllSetDecisions(game_state);

  vector<pair<double, Decision>> ev_to_decision;

  for (Decision d : all_decisions) {
    double ev = Solver(evaluator, rng).solve(iterations, game_state.my_hand.applyDecision(d), game_state.my_pull, game_state.other_hands, game_state.dead_cards);
    ev_to_decision.emplace_back(ev, d);
  }

  auto max_pair = max_element(ev_to_decision.begin(), ev_to_decision.end(),
      [](auto& left, auto& right) { return left.first < right.first; });
  return max_pair->second;
}
