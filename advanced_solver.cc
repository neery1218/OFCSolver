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

double AdvancedSolver::solve(int iterations, const GameState &game_state) const {
  // simulate the hand until the end!
  Deck deck(game_state);

  double total = 0;
  for (int i = 0; i < iterations; ++i) {
    stack<Hand> hands;
    hands.push(game_state.my_hand);

    vector<Card> dead_cards(game_state.dead_cards);

    Deck sim_deck(deck);
    while (hands.top().size() != 13) {
      Pull pull = Pull{sim_deck.select(3)};
      sim_deck.remove(pull.cards); 

      dead_cards.insert(dead_cards.end(), pull.cards.begin(), pull.cards.end());

      GameState new_state{hands.top(), game_state.other_hands, pull, dead_cards};
      Decision d = DecisionFinder(evaluator).findBestDecision(new_state, 5);
      hands.push(hands.top().applyDecision(d));
    }
    //cout << "hand: " << hands.top() << "\n\n";

    GameState final_state{hands.top(), game_state.other_hands, Pull{set<Card>()}, dead_cards};
    CompletedHand my_final_hand(hands.top(), evaluator);

    Deck final_deck{final_state};

    double sub_total = 0;
    for (int j = 0; j < 10; ++j) {
      if (game_state.other_hands.empty()) sub_total += my_final_hand.calculatePoints();
      else for (unsigned int k = 0; k < final_state.other_hands.size(); ++k) {
        int cards_needed = 13 - final_state.other_hands[k].size() + 1;
        set<Card> draw = deck.select(cards_needed);
        CompletedHand final_other_hand = final_state.other_hands[k].constructOptimalHand(draw, evaluator);
        sub_total += my_final_hand.calculatePoints(final_other_hand);
      }
    }
    // cout << "points: " << (sub_total / 10) << "\n";
    total += (sub_total / 10);

  }

  return total / iterations;
}
