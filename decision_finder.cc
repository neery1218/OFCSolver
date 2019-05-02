#include "decision_finder.h"
#include "solver.h"
#include <utility>
#include <future>
#include <algorithm>
#include <cassert>

using namespace std;

DecisionFinder::DecisionFinder(const PokerHandEvaluator &t_evaluator): evaluator{&t_evaluator} {}

Decision DecisionFinder::findBestDecision(const GameState &game_state) {

  vector<Decision> all_decisions = (game_state.my_hand.size() > 0) ? 
    findAllDrawDecisions(game_state) : findAllSetDecisions(game_state);

  vector<Decision> top_n_decisions_stage_one = stageOneEvaluation(all_decisions, 24, game_state, 50);
  Decision best_decision = stageTwoEvaluation(top_n_decisions_stage_one, game_state, 2000);

  return best_decision;
}

vector<Decision> DecisionFinder::stageOneEvaluation(const vector<Decision> &all_decisions, int n, const GameState &game_state, int num_iterations) {
  vector< future<double> > futures;
  vector< pair<double, Decision> > ev_to_decision;

  cout << "size : " << all_decisions.size() << "\n\n";

  /* Ignore other hands for now. We want to cheaply narrow down the search space first. */
  for (Decision d : all_decisions) {
    const PokerHandEvaluator *local_eval = evaluator;
    futures.push_back(
        async(
          std::launch::async,
          [d, local_eval, num_iterations, game_state] () {
          return Solver(local_eval).solve(
              num_iterations,
              game_state.my_hand.applyDecision(d),
              game_state.my_pull,
              vector<Hand>(),
              game_state.dead_cards);
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
      const PokerHandEvaluator *local_eval = evaluator;
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

vector<Decision> DecisionFinder::findAllSetDecisionsHelper(const set<Card> &cards, const vector<Placement> &acc) {
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

vector<Decision> DecisionFinder::findAllSetDecisions(const GameState &game_state) {
  assert(game_state.my_pull.cards.size() == 5);

  vector<Decision> decisions = findAllSetDecisionsHelper(game_state.my_pull.cards, vector<Placement> ());
  vector<Decision> validDecisions;

  for (auto &d : decisions) {
    int numTop = 0, numMid = 0, numBot = 0;
    for (auto &p : d.placements) {
      if (p.position == Position::top) ++numTop;
      else if (p.position == Position::middle) ++numMid;
      else if (p.position == Position::bottom) ++numBot;

    }
    if (numTop <= 3 && numMid <= 5 && numBot <= 5) validDecisions.push_back(d);
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

vector<Decision> DecisionFinder::findAllDrawDecisions(const GameState &game_state) {
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
