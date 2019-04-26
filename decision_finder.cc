#include "decision_finder.h"
#include "decision.h"
#include "position.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <future>

using namespace std;

DecisionFinder::DecisionFinder(GameType _type): evaluator{PokerHandEvaluator(_type)} {}

Decision DecisionFinder::findBestDecision(const Hand &h, const Pull &myPull, vector<Hand> &otherHands, vector<Card> &deadCards) { 
  // how many iterations do we need?
  int numIterations = findIterationsRequired(h);

  vector<pair<double, Decision>> evToDecision;
  vector < future<double> > futures;

  vector<Decision> allDecisions = findAllDecisions(h, myPull);

  for (auto &d : allDecisions) {
    Hand candidate = h.applyDecision(d);
    Solver s(evaluator);
    futures.push_back(async(
        std::launch::async, 
        &Solver::solve,
        s,
        numIterations, candidate, myPull, otherHands, deadCards));
    //double estimated_ev = Solver(evaluator).solve(numIterations, candidate, myPull, otherHands, deadCards);
    //evToDecision.emplace_back(estimated_ev, d);

    //cout << d << " : " << estimated_ev << "\n\n";
  }

  for (int i = 0; i < allDecisions.size(); ++i) {
    evToDecision.emplace_back(futures[i].get(), allDecisions[i]);
  }

  sort(evToDecision.begin(), evToDecision.end(), 
      [](auto &left, auto &right) { return right.first < left.first; });

  for (int i = 0; i < evToDecision.size(); ++i) {
    cout << evToDecision[i].first << " : " << evToDecision[i].second << "\n\n";
  }

  return evToDecision[0].second;
}

int DecisionFinder::findIterationsRequired(const Hand &h) {
  if (h.size() == 5) return 1000;
  else if (h.size() == 7) return 1000;
  else if (h.size() == 9) return 1000;
  return 1000;
}

vector<Decision> DecisionFinder::findAllDecisions(const Hand &h, const Pull &myPull) {

  int topCardsMissing = 3 - h.top.size();
  int midCardsMissing = 5 - h.middle.size();
  int botCardsMissing = 5 - h.bottom.size();

  vector<Card> cards(myPull.cards.begin(), myPull.cards.end());
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
