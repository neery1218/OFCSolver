#include "poker_hand_evaluator.h"
#include <fstream>
#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>

using namespace std;

PokerHandEvaluator::PokerHandEvaluator(GameType _gameType): gameType{_gameType} {
  ifstream f("hand_strength.csv");
  string line;

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
  int counter = 0;

  while (getline(f, line)) {
    ++counter;
    if (counter % 10000 == 0) cout << counter << endl;

    vector<string> tokens;
    boost::split(tokens, line, boost::is_any_of(","));

    int overallRank = stoi(tokens[1]);
    PokerHandType handType = intToPokerHandType[stoi(tokens[2])];
    int royalties = stoi(tokens[3]);

    if (tokens[0].size() == 6) { // top row
      if (overallRank >= 14883) { // fantasy land bonus
        if (gameType == GameType::regular) royalties += 7.5; 
        else if (gameType == GameType::progressive) {
          if (overallRank < 16215) royalties += 7.5; // Queens fantasy
          else if (overallRank < 17547) royalties += 10; // Kings fantasy
          else royalties += 20; // Aces fantasy
        }
      }

      topEvalInfo[tokens[0]] = PokerHandInfo{overallRank, handType, royalties};
    } 
    else {
      bottomEvalInfo[tokens[0]] = PokerHandInfo{overallRank, handType, royalties};

      if (handType == PokerHandType::TRIPS) royalties = 1; // special case...this feels bad
      middleEvalInfo[tokens[0]] = PokerHandInfo{overallRank, handType, royalties * 2};
    }
  }

  f.close();
}

PokerHandInfo PokerHandEvaluator::eval(vector<Card> &cards, Position pos) {
  sort(cards.begin(), cards.end());

  vector<string> cardStr;
  transform(cards.begin(), cards.end(), back_inserter(cardStr),
      [] (Card c) { return c.val; });

  string joined = boost::algorithm::join(cardStr, "");

  if (pos == Position::top) return topEvalInfo.at(joined);
  else if (pos == Position::middle) return middleEvalInfo.at(joined);
  else return bottomEvalInfo.at(joined);
}
