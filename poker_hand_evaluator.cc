#include "poker_hand_evaluator.h"
#include <fstream>
#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>

using namespace std;

PokerHandEvaluator::PokerHandEvaluator(GameType _gameType): gameType{_gameType} {
  ifstream f("hand_strength.csv");
  string line;

  topEvalInfo.reserve(22100);
  middleEvalInfo.reserve(3000000);
  bottomEvalInfo.reserve(3000000);

  unordered_map<int, PokerHandType> intToPokerHandType = {
    {0, PokerHandType::HIGH_CARD},
    {1, PokerHandType::PAIR},
    {2, PokerHandType::TWO_PAIR},
    {3, PokerHandType::TRIPS},
    {4, PokerHandType::STRAIGHT},
    {5, PokerHandType::FLUSH},
    {6, PokerHandType::FULL_HOUSE},
    {7, PokerHandType::QUADS},
    {8, PokerHandType::STRAIGHT_FLUSH},
    {9, PokerHandType::ROYAL_FLUSH}
  };

  cout << "Starting..." << endl;

  while (getline(f, line)) {
    vector<string> tokens;
    boost::split(tokens, line, boost::is_any_of(","));

    int overallRank = stoi(tokens[1]);
    PokerHandType handType = intToPokerHandType[stoi(tokens[2])];
    int royalties = stoi(tokens[3]);

    if (tokens[0].size() == 6) { // top row
      if (overallRank >= 3883) { royalties += calculateFantasyBonus(overallRank); }
      topEvalInfo[tokens[0]] = new PokerHandInfo{overallRank, handType, royalties};
    } 
    else {
      bottomEvalInfo[tokens[0]] = new PokerHandInfo{overallRank, handType, royalties};

      if (handType == PokerHandType::TRIPS) royalties = 1; // special case...this feels bad
      middleEvalInfo[tokens[0]] = new PokerHandInfo{overallRank, handType, royalties * 2};
    }
  }

  f.close();
}

PokerHandEvaluator::~PokerHandEvaluator() {
  for ( const auto& n : topEvalInfo ) { delete n.second; }
  for ( const auto& n : middleEvalInfo) { delete n.second; }
  for ( const auto& n : bottomEvalInfo ) { delete n.second; }
}

int PokerHandEvaluator::calculateFantasyBonus(int overallRank) const {
  if (overallRank >= 3833) {
    if (gameType == GameType::Regular) return 9;
    else if (gameType == GameType::Progressive) {
      if (overallRank < 4115) return 9;
      else if (overallRank < 4347) return 14;
      else return 19;
    } 
    else if (gameType == GameType::Ultimate) {
      if (overallRank < 4115) return 8;
      else if (overallRank < 4347)return 15;
      else return 25;
    }
  }
  return 0;
}

PokerHandInfo * PokerHandEvaluator::eval(const set<Card> &completedRow, Position pos) const {
  string joined = "";
  for (auto &c : completedRow) { joined += c.val; }
  if (pos == Position::top) return topEvalInfo.at(joined);
  else if (pos == Position::middle) return middleEvalInfo.at(joined);
  else return bottomEvalInfo.at(joined);
}

PokerHandInfo * PokerHandEvaluator::eval(const set<Card> &partialRow, const set<Card> &cards, Position pos) const {
  vector<Card> tmp;

  std::merge(partialRow.begin(), partialRow.end(), cards.begin(), cards.end(), back_inserter(tmp));
  string joined = "";
  for (auto &c : tmp) { joined += c.val; }

  if (pos == Position::top) return topEvalInfo.at(joined);
  else if (pos == Position::middle) return middleEvalInfo.at(joined);
  else return bottomEvalInfo.at(joined);
}
