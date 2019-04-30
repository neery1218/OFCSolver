#include "set_decision_finder.h"
#include "solver.h"
#include <utility>
#include <future>
#include <algorithm>
#include <cassert>

using namespace std;

SetDecisionFinder::SetDecisionFinder(const PokerHandEvaluator *t_evaluator): evaluator{t_evaluator} {}

Decision SetDecisionFinder::findBestDecision(const Pull &myPull, const vector<Hand> &otherHands) {
  set<Card> cards(myPull.cards.begin(), myPull.cards.end());

  vector<Decision> allDecisions = findAllDecisions(cards);
  vector< future<double> > futures;
  int numIterations = 100;
  vector<pair<double, Decision>> evToDecision;

  cout << "size : " << allDecisions.size() << "\n\n";
  for (Decision d : allDecisions) {
    const PokerHandEvaluator *localEval = evaluator;
    futures.push_back(
        async(
          std::launch::async,
          [d, localEval, numIterations, myPull, otherHands] () {
          return Solver(localEval).solve(
              numIterations,
              Hand().applyDecision(d),
              myPull,
              otherHands,
              vector<Card>());
          }));
  }

  for (int i = 0; i < futures.size(); ++i) {
    evToDecision.emplace_back(futures[i].get(), allDecisions[i]);
  }

  sort(evToDecision.begin(), evToDecision.end(),
      [](auto &left, auto &right) { return right.first < left.first; });

  for (int i = 0; i < evToDecision.size(); ++i) {
    cout << evToDecision[i].first << " : " << evToDecision[i].second << "\n\n";
  }

  return evToDecision[0].second;
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
