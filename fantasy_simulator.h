#ifndef _FANTASY_SIMULATOR_H_
#define _FANTASY_SIMULATOR_H_

#include "gametype.h"
#include "poker_hand_evaluator.h"

/* This class was used to find the EV of making fantasy in all different game types */
struct FantasySimulator {
  PokerHandEvaluator evaluator;
  FantasySimulator();
  double run(int numCards, int numIterations);    
};

#endif
