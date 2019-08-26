#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "poker_hand_evaluator.h"
#include "gametype.h"
#include <string>
#include <set>
#include <boost/algorithm/string.hpp>
#include "card.h"
#include <vector>

PokerHandEvaluator eval_ultimate(GameType::Ultimate);

std::set<Card> parse(std::string cards) {
  std::vector<std::string> tokens;
  std::set<Card> parsedCards;

  boost::split(tokens, cards, boost::is_any_of(" "));
  for (auto token : tokens) {
    parsedCards.insert(Card(token));
  }

  return parsedCards;
}

