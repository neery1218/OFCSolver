#include "advanced_solver.h"
#include "decision.h"
#include "decision_finder.h"
#include "deck.h"
#include "hand.h"
#include "pull.h"
#include "solver.h"
#include <fstream>
#include <iostream>
#include <random>
#include <stack>
#include <vector>

using namespace std;

AdvancedSolver::AdvancedSolver(const FastPokerHandEvaluator* t_evaluator, uint32_t seed)
    : evaluator { t_evaluator }
    , rng { seed }
{
}

double AdvancedSolver::solve(int iterations, const GameState& game_state, const Deck& initial_deck, int search_level)
{
#ifdef RESEARCH
  vector<double> evs;
#endif

  double total = 0;
  for (int i = 0; i < iterations; ++i) {
    stack<Hand> hands;
    hands.push(game_state.my_hand);

    vector<Card> dead_cards(game_state.dead_cards);
    dead_cards.insert(dead_cards.end(), game_state.my_pull.cards.begin(), game_state.my_pull.cards.end());

    Deck sim_deck(initial_deck);
    while (hands.top().size() < search_level) {
      Pull pull = Pull { sim_deck.select(3, &rng) };
      sim_deck.remove(pull.cards);

      dead_cards.insert(dead_cards.end(), pull.cards.begin(), pull.cards.end());

      GameState new_state { hands.top(), vector<Hand>(), pull, dead_cards };
      Decision d = DecisionFinder(evaluator, &rng).findBestDecision(new_state, 5);
      hands.push(hands.top().applyDecision(d));
    }
    double ev = Solver(evaluator, &rng).solve(10, hands.top(), Pull(), game_state.other_hands, dead_cards);
    total += ev;
#ifdef RESEARCH
    evs.push_back(ev);
#endif
  }
#ifdef RESEARCH
  if (iterations > 50) { // only take stage two values
    //int val = std::random_device {}();
    std::stringstream ss;
    ss << "/home/neerajen/Projects/OFCSolver/data/" << game_state.my_hand << ".csv";
    // ss << val;
    std::ofstream outf(ss.str());

    outf << "ev"
         << "\n";
    for (auto ev : evs) {
      outf << ev << "\n";
    }
  }
#endif

  return total / iterations;
}
