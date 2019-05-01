#include "set_decision_finder.h"
#include "solver.h"
#include <utility>
#include <future>
#include <algorithm>
#include <cassert>

using namespace std;

SetDecisionFinder::SetDecisionFinder(const PokerHandEvaluator &t_evaluator): evaluator{&t_evaluator} {}

Decision SetDecisionFinder::findBestDecision(const Pull &my_pull, const vector<Hand> &other_hands) {
  set<Card> cards(my_pull.cards.begin(), my_pull.cards.end());

  vector<Decision> all_decisions = findAllDecisions(cards);

  vector<Decision> top_n_decisions_stage_one = stageOneEvaluation(all_decisions, 24, my_pull, 100);
  Decision best_decision = stageTwoEvaluation(top_n_decisions_stage_one, my_pull, 1000, other_hands);

  return best_decision;
}

vector<Decision> SetDecisionFinder::stageOneEvaluation(const vector<Decision> &all_decisions, int n, const Pull &my_pull, 
    int num_iterations) {
  vector< future<double> > futures;
  vector<pair<double, Decision>> ev_to_decision;

  cout << "size : " << all_decisions.size() << "\n\n";

  for (Decision d : all_decisions) {
    const PokerHandEvaluator *local_eval = evaluator;
    futures.push_back(
        async(
          std::launch::async,
          [d, local_eval, num_iterations, my_pull] () {
          return Solver(local_eval).solve(
              num_iterations,
              Hand().applyDecision(d),
              my_pull,
              vector<Hand>(),
              vector<Card>());
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

Decision SetDecisionFinder::stageTwoEvaluation(const vector<Decision> &all_decisions, const Pull &my_pull, 
    int num_iterations, const vector<Hand> &other_hands) {

  vector< future<double> > futures;
  vector< pair<double, Decision> > ev_to_decision;

  for (Decision d : all_decisions) {
    for (int i = 0; i < 4; ++i) {
      const PokerHandEvaluator *local_eval = evaluator;
      int num_iterations_split = num_iterations / 4;
      futures.push_back(
          async(
            std::launch::async,
            [d, local_eval, num_iterations_split, my_pull, other_hands] () {
            return Solver(local_eval).solve(
                num_iterations_split,
                Hand().applyDecision(d),
                my_pull,
                other_hands,
                vector<Card>());
            }));
    }
  }

  for (unsigned int i = 0; i < all_decisions.size(); ++i) {
    double total = 0;
    for (int j = 0; i < 4; ++j) { total += futures[4 * i + j].get(); }
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

vector<Decision> SetDecisionFinder::findAllDecisionsHelper(const set<Card> &cards, const vector<Placement> &acc) {
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
  vector<Decision> topDecisions = findAllDecisionsHelper(subCards, topAcc);
  out.insert(out.end(), topDecisions.begin(), topDecisions.end());


  vector<Placement> midAcc(acc);
  midAcc.push_back(Placement{c, Position::middle});
  vector<Decision> midDecisions = findAllDecisionsHelper(subCards, midAcc);
  out.insert(out.end(), midDecisions.begin(), midDecisions.end());

  
  vector<Placement> botAcc(acc);
  botAcc.push_back(Placement{c, Position::bottom});
  vector<Decision> botDecisions = findAllDecisionsHelper(subCards, botAcc);
  out.insert(out.end(), botDecisions.begin(), botDecisions.end());

  return out;
}

vector<Decision> SetDecisionFinder::findAllDecisions(set<Card> cards) {
  vector<Decision> decisions = findAllDecisionsHelper(cards, vector<Placement> ());
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
