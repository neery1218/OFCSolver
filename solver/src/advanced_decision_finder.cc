#include "advanced_decision_finder.h"

#include <algorithm>
#include <cassert>
#include <unordered_set>
#include <utility>

#include "action_enumerator.h"
#include "advanced_solver.h"
#include "solver.h"

#ifndef NUM_THREADS
#define NUM_THREADS 12
#endif

AdvancedDecisionFinder::AdvancedDecisionFinder(
    const FastPokerHandEvaluator* t_evaluator)
    : evaluator{ t_evaluator }
    , pool{ NUM_THREADS }
{
}

SolverParams AdvancedDecisionFinder::getSolverParams(
    const GameState& game_state) const
{
  if (game_state.my_hand.size() == 0) {
    if (game_state.other_hands.size() == 0)
      return SolverParams{ 50, 1000, 9 };
    else if (game_state.other_hands.size() == 1)
      return SolverParams{ 50, 1000, 9 };
    else if (game_state.other_hands.size() == 2)
      return SolverParams{ 50, 1000, 9 };
    else
      throw std::runtime_error("Too many other hands u fool");
  }
  if (game_state.my_hand.size() == 5)
    return SolverParams{ 50, 1500, 9 };
  else if (game_state.my_hand.size() == 7)
    return SolverParams{ 50, 3000, 11 };
  else if (game_state.my_hand.size() == 9)
    return SolverParams{ 50, 10000, 11 };
  else if (game_state.my_hand.size() == 11)
    return SolverParams{ 50, 1500, 11 };
  else
    throw std::runtime_error("Hand size is not valid!");

  return {};
}

// TODO: factor in other hands size
/*
SolverParams AdvancedDecisionFinder::getSolverParams(const GameState
&game_state) const { if (game_state.my_hand.size() == 5) return SolverParams{50,
100}; else if (game_state.my_hand.size() == 7) return SolverParams{50, 100};
  else if (game_state.my_hand.size() == 9) return SolverParams{50, 100};
  else if (game_state.my_hand.size() == 11) return SolverParams{50, 100};
  else if (game_state.my_hand.size() == 13) return SolverParams{1, 1};
  else throw runtime_error("Hand size is not valid!");
}
*/

std::vector<std::pair<double, Decision> >
AdvancedDecisionFinder::findBestDecision(const GameState& game_state)
{
  std::vector<Decision> all_decisions = (game_state.my_hand.size() > 0)
      ? ActionEnumerator::findAllDrawDecisions(game_state)
      : ActionEnumerator::findAllSetDecisions(game_state);

  std::cout << "Decision size: " << all_decisions.size() << "\n";

  int top_n = (all_decisions.size() > 12) ? 12 : all_decisions.size();
  SolverParams params = getSolverParams(game_state);

  std::vector<Decision> top_n_decisions_stage_one = stageOneEvaluation(all_decisions, top_n, game_state, params);

  return stageTwoEvaluation(top_n_decisions_stage_one, game_state, params);
}

std::vector<Decision> AdvancedDecisionFinder::stageOneEvaluation(
    const std::vector<Decision>& all_decisions, unsigned int n,
    const GameState& game_state, const SolverParams& solver_params)
{
  std::vector<std::pair<double, Decision> > ev_to_decision;
  std::vector<double> ev_from_threads(all_decisions.size(),
      std::numeric_limits<double>::min());

  std::vector<Card> dead_cards(game_state.dead_cards);
  for (auto& h : game_state.other_hands) {
    dead_cards.insert(dead_cards.end(), h.top.begin(), h.top.end());
    dead_cards.insert(dead_cards.end(), h.middle.begin(), h.middle.end());
    dead_cards.insert(dead_cards.end(), h.bottom.begin(), h.bottom.end());
  }

  Deck initial_deck(game_state);
  initial_deck.remove(dead_cards);

  // Create a random seed per AdvancedSolver instance
  std::vector<uint32_t> seeds;
  uint32_t val = rd();
  for (int i = 0; i < all_decisions.size(); ++i) {
    seeds.push_back(val);
  }

  for (int i = 0; i < all_decisions.size(); ++i) {
    const FastPokerHandEvaluator* local_eval = evaluator;
    boost::asio::post(
        pool, [all_decisions, local_eval, solver_params, game_state,
                  &initial_deck, dead_cards, seeds, i, &ev_from_threads]() {
          GameState new_state{
            game_state.my_hand.applyDecision(all_decisions[i]),
            std::vector<Hand>(),
            // game_state.other_hands, // TODO: experiment with removing this.
            game_state.my_pull, dead_cards
          };

          // write result to ev_from_threads
          ev_from_threads[i] = AdvancedSolver(local_eval, seeds[i])
                                   .solve(solver_params.stage_one_iterations, new_state,
                                       initial_deck, solver_params.search_level);
        });
  }

  pool.join();

  for (unsigned int i = 0; i < ev_from_threads.size(); ++i) {
    ev_to_decision.emplace_back(ev_from_threads[i], all_decisions[i]);
  }

  std::sort(ev_to_decision.begin(), ev_to_decision.end(),
      [](auto& left, auto& right) { return right.first < left.first; });

  std::vector<Decision> top_n_decisions;
  std::cout << "Stage one: \n";
  for (unsigned int i = 0; i < n; ++i) {
    std::cout << ev_to_decision[i].first << " : " << ev_to_decision[i].second
              << "\n";
    top_n_decisions.emplace_back(ev_to_decision[i].second);
  }

  return top_n_decisions;
}
std::vector<std::pair<double, Decision> >
AdvancedDecisionFinder::stageTwoEvaluation(
    const std::vector<Decision>& all_decisions, const GameState& game_state,
    const SolverParams& solver_params)
{
  std::vector<std::pair<double, Decision> > ev_to_decision;
  int split = 8;
  boost::asio::thread_pool new_pool(NUM_THREADS);

#ifdef RESEARCH
  split = 1;
#endif
  std::vector<double> ev_from_threads(all_decisions.size() * split,
      std::numeric_limits<double>::min());

  Deck initial_deck(game_state);

  // Create a random seed per AdvancedSolver instance
  std::vector<uint32_t> seeds;
  uint32_t val = rd();
#ifdef RESEARCH
  val = 1234;
#endif
  for (int i = 0; i < all_decisions.size(); ++i) {
    seeds.push_back(val);
  }

  for (int i = 0; i < all_decisions.size(); ++i) {
    for (int j = 0; j < split; ++j) {
      const FastPokerHandEvaluator* local_eval = evaluator;
      int num_iterations_split = solver_params.stage_two_iterations / split;
      boost::asio::post(new_pool, [all_decisions, local_eval,
                                      num_iterations_split, game_state,
                                      &initial_deck, solver_params, seeds, i, j,
                                      &ev_from_threads, split]() {
        GameState new_state{ game_state.my_hand.applyDecision(all_decisions[i]),
          game_state.other_hands, game_state.my_pull,
          game_state.dead_cards };
        double ev = AdvancedSolver(local_eval, seeds[i])
                        .solve(num_iterations_split, new_state, initial_deck,
                            solver_params.search_level);
        ev_from_threads[i * split + j] = ev;
      });
    }
  }

  new_pool.join();

  for (unsigned int i = 0; i < all_decisions.size(); ++i) {
    double total = 0;
    for (int j = 0; j < split; ++j) {
      total += ev_from_threads[split * i + j];
    }
    ev_to_decision.emplace_back(total / split, all_decisions[i]);
  }

  std::sort(ev_to_decision.begin(), ev_to_decision.end(),
      [](auto& left, auto& right) { return right.first < left.first; });

  std::cout << "Stage two (Advanced): \n";
  for (unsigned int i = 0; i < ev_to_decision.size(); ++i) {
    std::cout << ev_to_decision[i].first << " : " << ev_to_decision[i].second
              << "\n";
  }

  return ev_to_decision;
}
