#include <iostream>
#include <set>
#include <vector>

#include "card.h"
#include "fast_poker_hand_evaluator.h"
#include "gametype.h"
#include "hand.h"
#include "position.h"
#include "pull.h"
#include "solver.h"

int main()
{
  FastPokerHandEvaluator* evaluator = new FastPokerHandEvaluator(GameType::Regular);
  std::mt19937 rng { 1234 };
  Hand hand(
      CardUtils::parseCards("Ac"),
      CardUtils::parseCards("2c 2d"),
      CardUtils::parseCards("9h 9d"));

  Pull pull { CardUtils::parseCards("Ac 4c 4d") };
  double ev = Solver(evaluator, &rng).solve(10000, hand, pull, std::vector<Hand>(), std::vector<Card>());
  std::cout << "Ev: " << ev << std::endl;
}
