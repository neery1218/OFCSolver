#include "advanced_decision_finder.h"
#include "card.h"
#include "decision.h"
#include "decision_finder.h"
#include "gametype.h"
#include "hand.h"
#include "placement.h"
#include "position.h"
#include "pull.h"
#include "solver.h"
#include <iostream>
#include <vector>

int main()
{

  FastPokerHandEvaluator eval(GameType::Ultimate);
  const FastPokerHandEvaluator* ptr = &eval;

  /*
  Hand hand(
      CardUtils::parseCards("Ac"),
      CardUtils::parseCards("2c 2d"),
      CardUtils::parseCards("9h 9d"));
      */
  Hand hand;
  Pull pull { CardUtils::parseCards("Ac Ad Ah Kd Kc") };

  GameState game_state { hand, std::vector<Hand>(), pull, std::vector<Card>() };
  Decision d = AdvancedDecisionFinder(ptr).findBestDecision(game_state);
  std::cout << "Best decision is: " << d << "\n";
}
