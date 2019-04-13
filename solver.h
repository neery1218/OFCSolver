#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <string>
#include <vector>
#include <array>
#include <ostream>
#include <unordered_map>
#include <utility>
#include "gametype.h"
#include "pull.h"
#include "hand.h"
#include "deck.h"
#include "card.h"
#include "decision.h"

/*
 * tldr: Pass possible decisions to the solver, and the solver picks the best decision.
 * */


enum Method {
  CPU = 0,
  GPU = 1
};

class Solver { // maybe this should be a standalone function?
  Method method;
  GameType type;
  int numIterations;
  public:
    Solver(Method method, GameType type, int numIterations);
    std::vector<std::pair<Decision, double>> solve(Hand &myHand, Pull &myPull, std::vector<Hand> otherHands, 
        std::vector<Decision> decisions, std::vector<Card> deadCards);
  private:
    double estimateEV(Hand &myHand, Decision decision, 
        std::vector<Hand> otherHands, Deck &deck);
};

#endif
