#include "advanced_solver.h"
#include <stack>
#include "deck.h"
#include "pull.h"
#include "decision.h"
#include <vector>
#include "hand.h"
#include "decision_finder.h"


using namespace std;

AdvancedSolver::AdvancedSolver(const PokerHandEvaluator *t_evaluator): evaluator{t_evaluator} {}

double AdvancedSolver::solve(int iterations, const GameState &game_state, const Deck &initial_deck) const {

  double total = 0;
  for (int i = 0; i < iterations; ++i) {
    stack<Hand> hands;
    hands.push(game_state.my_hand);

    vector<Card> dead_cards(game_state.dead_cards);

    Deck sim_deck(initial_deck);
    while (hands.top().size() <= 7) {
      Pull pull = Pull{sim_deck.select(3)};
      sim_deck.remove(pull.cards); 

      dead_cards.insert(dead_cards.end(), pull.cards.begin(), pull.cards.end());

      GameState new_state{hands.top(), game_state.other_hands, pull, dead_cards};
      Decision d = DecisionFinder(evaluator).findBestDecision(new_state, 2);
      hands.push(hands.top().applyDecision(d));
    }
    //cout << "hand: " << hands.top() << "\n\n";

    GameState final_state{hands.top(), game_state.other_hands, Pull{set<Card>()}, dead_cards};
    set<Card> my_draw = sim_deck.select(13 - hands.top().size());
    CompletedHand my_final_hand = hands.top().constructOptimalHand(my_draw, evaluator);

    if (game_state.other_hands.empty()) total += my_final_hand.calculatePoints();
    else for (unsigned int k = 0; k < final_state.other_hands.size(); ++k) {
      int cards_needed = 13 - final_state.other_hands[k].size() + 1;
      set<Card> draw = sim_deck.select(cards_needed);
      CompletedHand final_other_hand = final_state.other_hands[k].constructOptimalHand(draw, evaluator);
      total += my_final_hand.calculatePoints(final_other_hand);
    }
  }
  return total / iterations;
}
