#include "poker_hand_evaluator.h"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

PokerHandEvaluator::PokerHandEvaluator(GameType _gameType)
    : gameType { _gameType }
{
  ifstream f("/root/src/solver/src/hand_strength.csv");
  string line;

  top_eval_info.reserve(22100);
  middle_eval_info.reserve(3000000);
  bottom_eval_info.reserve(3000000);

  cout << "Starting..." << endl;

  while (getline(f, line)) {
    vector<string> tokens;
    boost::split(tokens, line, boost::is_any_of(" "));

    unsigned int overallRank = stoi(tokens[1]);
    unsigned int handType = stoi(tokens[2]);
    unsigned int royalties = stoi(tokens[3]);

    std::string new_row = "";
    for (int i = 0; i < tokens[0].size(); ++i) {
      if (i > 0 && i % 2 == 0)
        new_row += " ";
      new_row += tokens[0][i];
    }

    if (tokens[0].size() == 6) { // top row
      if (overallRank >= 3883) {
        royalties += calculateFantasyBonus(overallRank);
      }
      top_eval_info.push_back(make_pair(new_row, PokerHandUtils::createPokerHandInfo(overallRank, handType, royalties)));
    } else {
      bottom_eval_info.push_back(make_pair(new_row, PokerHandUtils::createPokerHandInfo(overallRank, handType, royalties)));

      if (handType == static_cast<int>(HandType::TRIPS))
        royalties = 1; // special case...this feels bad
      middle_eval_info.push_back(make_pair(new_row, PokerHandUtils::createPokerHandInfo(overallRank, handType, royalties * 2)));
    }
  }

  f.close();
}

unsigned int PokerHandEvaluator::calculateFantasyBonus(unsigned int overallRank) const
{
  if (overallRank >= 3833) {
    if (gameType == GameType::Regular)
      return 9;
    else if (gameType == GameType::Progressive) {
      if (overallRank < 4115)
        return 9;
      else if (overallRank < 4347)
        return 14;
      else
        return 19;
    } else if (gameType == GameType::Ultimate) {
      if (overallRank < 4115)
        return 8;
      else if (overallRank < 4347)
        return 15;
      else
        return 25;
    }
  }
  return 0;
}

vector<pair<string, PokerHandInfo>> PokerHandEvaluator::getTopInfos() { return top_eval_info; }
vector<pair<string, PokerHandInfo>> PokerHandEvaluator::getMiddleInfos() { return middle_eval_info; }
vector<pair<string, PokerHandInfo>> PokerHandEvaluator::getBottomInfos() { return bottom_eval_info; }
