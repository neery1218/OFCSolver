#include "decision_finder.h"
#include "solver.h"
#include "action_enumerator.h"
#include <utility>
#include <future>
#include <algorithm>
#include <cassert>

using namespace std;

DecisionFinder::DecisionFinder(const FastPokerHandEvaluator *t_evaluator): evaluator{t_evaluator} {}

Decision DecisionFinder::findBestDecision(const GameState &game_state) {

  vector<Decision> all_decisions = (game_state.my_hand.size() > 0) ? 
    ActionEnumerator::findAllDrawDecisions(game_state) : ActionEnumerator::findAllSetDecisions(game_state);

  int n = (all_decisions.size() > 24) ? 24 : all_decisions.size();
  vector<Decision> top_n_decisions_stage_one = stageOneEvaluation(all_decisions, n, game_state, 50);
  Decision best_decision = stageTwoEvaluation(top_n_decisions_stage_one, game_state, 5000);

  return best_decision;
}

Decision DecisionFinder::findBestDecision(const GameState &game_state, int iterations) {

  vector<Decision> all_decisions = (game_state.my_hand.size() > 0) ? 
    ActionEnumerator::findAllDrawDecisions(game_state) : ActionEnumerator::findAllSetDecisions(game_state);

  vector< pair<double, Decision> > ev_to_decision;

  for (Decision d : all_decisions) {
    double ev = Solver(evaluator).solve(
        iterations, 
        game_state.my_hand.applyDecision(d),
        game_state.my_pull, 
        game_state.other_hands, 
        game_state.dead_cards);
    ev_to_decision.emplace_back(ev, d);
  }

  auto max_pair = max_element(ev_to_decision.begin(), ev_to_decision.end(),
      [](auto &left, auto &right) { return left.first < right.first; });
  return max_pair->second;
}

vector<Decision> DecisionFinder::stageOneEvaluation(const vector<Decision> &all_decisions, unsigned int n, const GameState &game_state, int num_iterations) {
  vector< future<double> > futures;
  vector< pair<double, Decision> > ev_to_decision;

  vector<Card> dead_cards{game_state.dead_cards};
  for (auto &h : game_state.other_hands) {
    dead_cards.insert(dead_cards.end(), h.top.begin(), h.top.end());
    dead_cards.insert(dead_cards.end(), h.middle.begin(), h.middle.end());
    dead_cards.insert(dead_cards.end(), h.bottom.begin(), h.bottom.end());
  }

  for (Decision d : all_decisions) {
    const FastPokerHandEvaluator *local_eval = evaluator;
    futures.push_back(
        async(
          std::launch::async,
          [d, local_eval, num_iterations, game_state, dead_cards] () {
          return Solver(local_eval).solve(
              num_iterations,
              game_state.my_hand.applyDecision(d),
              game_state.my_pull,
              vector<Hand>(),
              dead_cards);
          }));
  }

  for (unsigned int i = 0; i < futures.size(); ++i) {
    ev_to_decision.emplace_back(futures[i].get(), all_decisions[i]);
  }

  sort(ev_to_decision.begin(), ev_to_decision.end(),
      [](auto &left, auto &right) { return right.first < left.first; });

  vector<Decision> top_n_decisions;
  cout << "Stage one: \n";
  for (unsigned int i = 0; i < n; ++i) { 
    cout << ev_to_decision[i].first << " : " << ev_to_decision[i].second;
    top_n_decisions.emplace_back(ev_to_decision[i].second);
  }

  return top_n_decisions;
}

Decision DecisionFinder::stageTwoEvaluation(const vector<Decision> &all_decisions, const GameState &game_state, 
    int num_iterations) {

  vector< future<double> > futures;
  vector< pair<double, Decision> > ev_to_decision;

  for (Decision d : all_decisions) {
    for (int i = 0; i < 4; ++i) {
      const FastPokerHandEvaluator *local_eval = evaluator;
      int num_iterations_split = num_iterations / 4;
      futures.push_back(
          async(
            std::launch::async,
            [d, local_eval, num_iterations_split, game_state] () {
            return Solver(local_eval).solve(
                num_iterations_split,
                game_state.my_hand.applyDecision(d),
                game_state.my_pull,
                game_state.other_hands,
                game_state.dead_cards);
            }));
    }
  }

  for (unsigned int i = 0; i < all_decisions.size(); ++i) {
    double total = 0;
    for (int j = 0; j < 4; ++j) { total += futures[4 * i + j].get(); }
    ev_to_decision.emplace_back(total / 4, all_decisions[i]);
  }

  sort(ev_to_decision.begin(), ev_to_decision.end(),
      [](auto &left, auto &right) { return right.first < left.first; });

  cout << "Stage two: \n";
  for (unsigned int i = 0; i < ev_to_decision.size(); ++i) { 
    cout << ev_to_decision[i].first << " : " << ev_to_decision[i].second;
  }

  return ev_to_decision[0].second;
}
