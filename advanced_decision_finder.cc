#include "decision_finder.h"
#include "advanced_decision_finder.h"
#include "advanced_solver.h"
#include "solver.h"
#include <utility>
#include <future>
#include <algorithm>
#include <cassert>
#include <unordered_set>

using namespace std;

AdvancedDecisionFinder::AdvancedDecisionFinder(const PokerHandEvaluator *t_evaluator): evaluator{t_evaluator} {}

SolverParams AdvancedDecisionFinder::getSolverParams(const GameState &game_state) const {
  if (game_state.my_hand.size() == 0) {
    if (game_state.other_hands.size() == 0) return SolverParams{50, 1000, 9};
    else if (game_state.other_hands.size() == 1) return SolverParams{50, 600, 9};
    else if (game_state.other_hands.size() == 2) return SolverParams{50, 600, 9};
    else throw runtime_error("Too many other hands u fool");
  }
  if (game_state.my_hand.size() == 5) return SolverParams{50, 1200, 9};
  else if (game_state.my_hand.size() == 7) return SolverParams{50, 2000, 11};
  else if (game_state.my_hand.size() == 9) return SolverParams{50, 2000, 11};
  else if (game_state.my_hand.size() == 11) return SolverParams{50, 2000, 11};
  else throw runtime_error("Hand size is not valid!");
}

// TODO: factor in other hands size
/*
SolverParams AdvancedDecisionFinder::getSolverParams(const GameState &game_state) const {
  if (game_state.my_hand.size() == 5) return SolverParams{50, 100};
  else if (game_state.my_hand.size() == 7) return SolverParams{50, 100};
  else if (game_state.my_hand.size() == 9) return SolverParams{50, 100};
  else if (game_state.my_hand.size() == 11) return SolverParams{50, 100};
  else if (game_state.my_hand.size() == 13) return SolverParams{1, 1};
  else throw runtime_error("Hand size is not valid!");
}
*/

Decision AdvancedDecisionFinder::findBestDecision(const GameState &game_state) {

  vector<Decision> all_decisions = (game_state.my_hand.size() > 0) ? 
    findAllDrawDecisions(game_state) : findAllSetDecisions(game_state);

  cout << "Decision size: " << all_decisions.size() << "\n";

  int top_n = (all_decisions.size() > 12) ? 12 : all_decisions.size();
  SolverParams params = getSolverParams(game_state);

  vector<Decision> top_n_decisions_stage_one = stageOneEvaluation(
      all_decisions, top_n, game_state, params);

  Decision best_decision = stageTwoEvaluation(
      top_n_decisions_stage_one, game_state, params);

  return best_decision;
}

vector<Decision> AdvancedDecisionFinder::stageOneEvaluation(const vector<Decision> &all_decisions, unsigned int n, const GameState &game_state, const SolverParams &solver_params) {
  vector< future<double> > futures;
  vector< pair<double, Decision> > ev_to_decision;

  vector<Card> dead_cards;
  for (auto &h : game_state.other_hands) {
    dead_cards.insert(dead_cards.end(), h.top.begin(), h.top.end());
    dead_cards.insert(dead_cards.end(), h.middle.begin(), h.middle.end());
    dead_cards.insert(dead_cards.end(), h.bottom.begin(), h.bottom.end());
  }

  Deck initial_deck(game_state);
  initial_deck.remove(dead_cards);

  for (Decision d : all_decisions) {
    const PokerHandEvaluator *local_eval = evaluator;
    futures.push_back(
        async(
          std::launch::async,
          [d, local_eval, solver_params, game_state, &initial_deck, dead_cards] () {
          GameState new_state{
            game_state.my_hand.applyDecision(d),
            vector<Hand>(),
            //game_state.other_hands, // TODO: experiment with removing this. 
            game_state.my_pull,
            dead_cards};
          return AdvancedSolver(local_eval).solve(
              solver_params.stage_one_iterations,
              new_state, initial_deck, solver_params.search_level);
          }));
  }
    /*
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
    */

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
Decision AdvancedDecisionFinder::stageTwoEvaluation(const vector<Decision> &all_decisions, const GameState &game_state, 
    const SolverParams &solver_params) {

  vector< future<double> > futures;
  vector< pair<double, Decision> > ev_to_decision;
  int split = 8;

  Deck initial_deck(game_state);

  for (Decision d : all_decisions) {
    for (int i = 0; i < split; ++i) {
      const PokerHandEvaluator *local_eval = evaluator;
      int num_iterations_split = solver_params.stage_two_iterations / split;
      futures.push_back(
          async(
            std::launch::async,
            [d, local_eval, num_iterations_split, game_state, &initial_deck, solver_params] () {
            GameState new_state{
              game_state.my_hand.applyDecision(d),
              game_state.other_hands,
              game_state.my_pull,
              game_state.dead_cards};
            return AdvancedSolver(local_eval).solve(
                num_iterations_split,
                new_state, initial_deck, solver_params.search_level);
            }));
    }
  }

  for (unsigned int i = 0; i < all_decisions.size(); ++i) {
    double total = 0;
    for (int j = 0; j < split; ++j) { total += futures[split * i + j].get(); }
    ev_to_decision.emplace_back(total / split, all_decisions[i]);
  }

  sort(ev_to_decision.begin(), ev_to_decision.end(),
      [](auto &left, auto &right) { return right.first < left.first; });

  cout << "Stage two (Advanced): \n";
  for (unsigned int i = 0; i < ev_to_decision.size(); ++i) { 
    cout << ev_to_decision[i].first << " : " << ev_to_decision[i].second;
  }

  return ev_to_decision[0].second;
}

vector<Decision> AdvancedDecisionFinder::findAllSetDecisionsHelper(const set<Card> &cards, const vector<Placement> &acc) {
  vector<Decision> out;

  if (cards.empty()) {
    assert(acc.size() == 5);
    return vector<Decision>{Decision(acc)};
  }

  Card c = *cards.begin();
  set<Card> subCards(cards);
  subCards.erase(c);

  
  vector<Placement> topAcc(acc);
  topAcc.push_back(Placement{c, Position::top});
  vector<Decision> topDecisions = findAllSetDecisionsHelper(subCards, topAcc);
  out.insert(out.end(), topDecisions.begin(), topDecisions.end());


  vector<Placement> midAcc(acc);
  midAcc.push_back(Placement{c, Position::middle});
  vector<Decision> midDecisions = findAllSetDecisionsHelper(subCards, midAcc);
  out.insert(out.end(), midDecisions.begin(), midDecisions.end());

  
  vector<Placement> botAcc(acc);
  botAcc.push_back(Placement{c, Position::bottom});
  vector<Decision> botDecisions = findAllSetDecisionsHelper(subCards, botAcc);
  out.insert(out.end(), botDecisions.begin(), botDecisions.end());

  return out;
}

vector<Decision> AdvancedDecisionFinder::findAllSetDecisions(const GameState &game_state) {
  assert(game_state.my_pull.cards.size() == 5);

  vector<Decision> decisions = findAllSetDecisionsHelper(game_state.my_pull.cards, vector<Placement> ());
  unordered_set<string> fantasy_cards = {"Ah", "Ad", "Ac", "As", "Kh", "Kd", "Kc", "Ks", "Qh", "Qd", "Qc", "Qs"};
  vector<Decision> validDecisions;

  for (auto &d : decisions) {
    int numTop = 0, numMid = 0, numBot = 0;
    int numNonFantasyCardsTop = 0;
    for (auto &p : d.placements) {
      if (p.position == Position::top) {
        ++numTop;
        if (fantasy_cards.count(p.card.val) == 0) ++numNonFantasyCardsTop;
      }
      else if (p.position == Position::middle) ++numMid;
      else if (p.position == Position::bottom) ++numBot;

    }
    if (numTop <= 3 && numMid <= 5 && numBot <= 5 && numNonFantasyCardsTop < 2) validDecisions.push_back(d);
  }

  return validDecisions;
}

/*
int DecisionFinder::findIterationsRequired(const Hand &h) {
  if (h.size() == 5) return 1000;
  else if (h.size() == 7) return 1000;
  else if (h.size() == 9) return 1000;
  return 1000;
}
*/

vector<Decision> AdvancedDecisionFinder::findAllDrawDecisions(const GameState &game_state) {
  assert(game_state.my_pull.cards.size() == 3);

  int topCardsMissing = 3 - game_state.my_hand.top.size();
  int midCardsMissing = 5 - game_state.my_hand.middle.size();
  int botCardsMissing = 5 - game_state.my_hand.bottom.size();

  vector<Card> cards(game_state.my_pull.cards.begin(), game_state.my_pull.cards.end());
  vector<Decision> decisions;

  for (int i = 0; i < cards.size(); ++i) {

    if (topCardsMissing > 0) {
      --topCardsMissing;
      Placement p1(cards[i], Position::top);
      for (int j = i + 1; j < cards.size(); ++j) {
        if (topCardsMissing > 0) {
          decisions.emplace_back(Decision{p1, Placement(cards[j], Position::top)});
        }

        if (midCardsMissing > 0) {
          decisions.emplace_back(Decision{p1, Placement(cards[j], Position::middle)});
        }

        if (botCardsMissing > 0) {
          decisions.emplace_back(Decision{p1, Placement(cards[j], Position::bottom)});
        }
      }
      ++topCardsMissing;
    }


    if (midCardsMissing > 0) {
      --midCardsMissing;
      Placement p1(cards[i], Position::middle);
      for (int j = i + 1; j < cards.size(); ++j) {
        if (topCardsMissing > 0) {
          decisions.emplace_back(Decision{p1, Placement(cards[j], Position::top)});
        }

        if (midCardsMissing > 0) {
          decisions.emplace_back(Decision{p1, Placement(cards[j], Position::middle)});
        }

        if (botCardsMissing > 0) {
          decisions.emplace_back(Decision{p1, Placement(cards[j], Position::bottom)});
        }
      }
      ++midCardsMissing;
    }

    if (botCardsMissing > 0) {
      --botCardsMissing;
      Placement p1(cards[i], Position::bottom);
      for (int j = i + 1; j < cards.size(); ++j) {
        if (topCardsMissing > 0) {
          decisions.emplace_back(Decision{p1, Placement(cards[j], Position::top)});
        }

        if (midCardsMissing > 0) {
          decisions.emplace_back(Decision{p1, Placement(cards[j], Position::middle)});
        }

        if (botCardsMissing > 0) {
          decisions.emplace_back(Decision{p1, Placement(cards[j], Position::bottom)});
        }
      }
      ++botCardsMissing;
    }
  }

  return decisions;
}
