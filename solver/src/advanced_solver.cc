#include "advanced_solver.h"

#include <boost/format.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <stack>
#include <vector>

#include "decision.h"
#include "decision_finder.h"
#include "deck.h"
#include "hand.h"
#include "pull.h"
#include "solver.h"

using namespace std;

AdvancedSolver::AdvancedSolver(const FastPokerHandEvaluator *t_evaluator,
                               uint32_t seed)
    : evaluator{t_evaluator}, rng{seed} {}

DecisionStats AdvancedSolver::solve(int iterations, const GameState &game_state,
                                    const Deck &initial_deck,
                                    int search_level) {
#ifdef RESEARCH
  vector<double> evs;
  // int val = std::random_device {}();
  std::stringstream ss;
  bool wrote_header = false;
#ifdef STUDENTCS
  ss << "/u1/n8sritharan/OFCSolver/data/hand+" << game_state.my_hand << "+"
     << game_state.n_solves << "+" << game_state.n_decision_solves << ".csv";
#else
  ss << "/home/neerajen/Projects/OFCSolver/data/hand+" << game_state.my_hand
     << "+" << game_state.n_solves << "+" << game_state.n_decision_solves
     << ".csv";
#endif
  std::ofstream outf;
  if (iterations > 50) {
    outf.open(ss.str(), std::ofstream::out);
  }
#endif

  double total = 0;
  DecisionStats dS = {0, 0, 0, 0};
  std::vector<double> evs;
  for (int i = 0; i < iterations; ++i) {
    stack<Hand> hands;
    hands.push(game_state.my_hand);

    vector<Card> dead_cards(game_state.dead_cards);
    dead_cards.insert(dead_cards.end(), game_state.my_pull.cards.begin(),
                      game_state.my_pull.cards.end());

    Deck sim_deck(initial_deck);

#ifdef RESEARCH
    // track pull, decision
    map<std::string, std::string> row;
#endif

    while (hands.top().size() < search_level) {
      Pull pull = Pull{sim_deck.select(3, &rng)};
      sim_deck.remove(pull.cards);

      dead_cards.insert(dead_cards.end(), pull.cards.begin(), pull.cards.end());

      GameState new_state{hands.top(), vector<Hand>(), pull, dead_cards};

      int n_iterations = iterations > 50 ? game_state.n_decision_solves : 1;
      Decision d = DecisionFinder(evaluator, &rng)
                       .findBestDecision(new_state, n_iterations);

#ifdef RESEARCH
      boost::format pull_key("Pull_%1%");
      boost::format decision_key("Decision_%1%");
      boost::format hand_key("Hand_%1%");

      int index = (hands.top().size() - 5) / 2;

      pull_key % index;
      decision_key % index;
      hand_key % index;

      std::stringstream pull_ss, decision_ss, hand_ss;
      pull_ss << pull;
      decision_ss << d;
      hand_ss << hands.top();

      row[pull_key.str()] = pull_ss.str();
      row[decision_key.str()] = decision_ss.str();
      row[hand_key.str()] = hand_ss.str();
#endif
      hands.push(hands.top().applyDecision(d));
    }

#ifdef RESEARCH
    // last hand
    boost::format hand_key("Hand_%1%");
    int index = (hands.top().size() - 5) / 2;
    hand_key % index;
    std::stringstream hand_ss;
    hand_ss << hands.top();

    row[hand_key.str()] = hand_ss.str();

#endif
#ifdef RESEARCH
    vector<double> evs =
        Solver(evaluator, &rng)
            .solve(10, hands.top(), Pull(), game_state.other_hands, dead_cards);
    for (int i = 0; i < 10; ++i) {
      boost::format ev_key("solver_ev_%1%");
      ev_key % i;
      row[ev_key.str()] = std::to_string(evs[i]);
    }
    double ev = std::accumulate(evs.begin(), evs.end(), 0) / evs.size();
    row["solver_ev_avg"] = std::to_string(ev);
    total += ev;

#else
    DecisionStats solverStats =
        Solver(evaluator, &rng)
            .solve(10, hands.top(), Pull(), game_state.other_hands, dead_cards);

    dS.mean += solverStats.mean;
    dS.queens_fantasy_pct += solverStats.queens_fantasy_pct;
    dS.kings_fantasy_pct += solverStats.kings_fantasy_pct;
    dS.aces_fantasy_pct += solverStats.aces_fantasy_pct;
    evs.push_back(solverStats.mean);
#endif

#ifdef RESEARCH
    if (iterations > 50) {
      // output to csv
      if (!wrote_header) {
        vector<std::string> headers;
        for (auto p : row) {
          headers.push_back(p.first);
        }
        std::string joined = boost::algorithm::join(headers, ",");
        outf << joined << "\n";
        wrote_header = true;
      }

      vector<std::string> vals;
      for (auto p : row) {
        vals.push_back(p.second);
        /* std::cout << key << " " << val << std::endl; */
      }

      std::string joined = boost::algorithm::join(vals, ",");
      outf << joined << "\n";
    }
#endif
  }

  dS.mean /= iterations;
  dS.queens_fantasy_pct /= iterations;
  dS.kings_fantasy_pct /= iterations;
  dS.aces_fantasy_pct /= iterations;

  double sum = 0;
  // calculate variance
  for (double ev : evs) {
    sum += pow((ev - dS.mean), 2);
  }
  dS.variance = sum / evs.size();

  return dS;
}
