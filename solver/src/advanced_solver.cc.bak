#include "advanced_solver.h"
#include <stack>
#include "deck.h"
#include "pull.h"
#include "decision.h"
#include <vector>
#include "solver.h"
#include "hand.h"
#include "decision_finder.h"
#include <iostream>


using namespace std;

AdvancedSolver::AdvancedSolver(const PokerHandEvaluator *t_evaluator): evaluator{t_evaluator} {}

double AdvancedSolver::solve(int iterations, const GameState &game_state, const Deck &initial_deck, int search_level) const {
  double total = 0;
  for (int i = 0; i < iterations; ++i) {
    stack<Hand> hands;
    hands.push(game_state.my_hand);

    vector<Card> dead_cards(game_state.dead_cards);
    dead_cards.insert(dead_cards.end(), game_state.my_pull.cards.begin(), game_state.my_pull.cards.end());

    Deck sim_deck(initial_deck);
    while (hands.top().size() < search_level) {
      Pull pull = Pull{sim_deck.select(3)};
      sim_deck.remove(pull.cards); 

      dead_cards.insert(dead_cards.end(), pull.cards.begin(), pull.cards.end());

      GameState new_state{hands.top(), vector<Hand>(), pull, dead_cards};
      Decision d = DecisionFinder(evaluator).findBestDecision(new_state, 5);
      hands.push(hands.top().applyDecision(d));
    }
    total += Solver(evaluator).solve(10, hands.top(), Pull(), game_state.other_hands, dead_cards);
  }
  return total / iterations;
}
