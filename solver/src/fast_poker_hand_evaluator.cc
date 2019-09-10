#include "fast_poker_hand_evaluator.h"
#include "absl/strings/str_split.h"

#include <fstream>
#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <assert.h>     /* assert */

unsigned int convertHandStr(std::string hand) {
  unsigned int index = 0;
  for (const char &c : hand) {
    unsigned int val = 0;
    switch (c) {
      case 'A':
        val = 12;
        break;
      case 'K':
        val = 11;
        break;
      case 'Q':
        val = 10;
        break;
      case 'J':
        val = 9;
        break;
      case 'T':
        val = 8;
        break;
      default:
        val = c - '2';
    }
    index = index * 13 + val;
  }

  return index;
}

FastPokerHandEvaluator::FastPokerHandEvaluator(GameType _game_type): game_type{_game_type} {
  std::ifstream f("/root/src/solver/src/no_suit_hand_strength.csv");
  std::string line;

  top_eval_info.reserve(2197);
  middle_eval_info.reserve(371293);
  bottom_eval_info.reserve(371293);

  std::cout << "Starting..." << std::endl;

  while (getline(f, line)) {
    // sample line: "2224J,5467,3,0" => 
    // hand = "2224J"
    // overall rank = "5467" (relative rank to all other hands)
    // hand type = 3
    // royalties = 2

    std::vector<std::string> tokens = absl::StrSplit(line, ",");

    std::string hand = tokens[0];
    unsigned int overall_rank = stoi(tokens[1]);
    unsigned int hand_type = stoi(tokens[2]);
    unsigned int royalties = stoi(tokens[3]);
    unsigned int royalties_middle = (hand_type == TRIPS) ? 2 : (royalties * 2);

    unsigned int hand_index = convertHandStr(hand);

    if (hand.size() == 3) { // top row
      royalties += calculateFantasyBonus(hand_type);

      top_eval_info[hand_index] = PokerHandInfoUtils::createPokerHandInfo(overall_rank, hand_type, royalties);
    }
    else {
      middle_eval_info[hand_index] = PokerHandInfoUtils::createPokerHandInfo(overall_rank, hand_type, royalties_middle);
      bottom_eval_info[hand_index] = PokerHandInfoUtils::createPokerHandInfo(overall_rank, hand_type, royalties);
    }
  }
  f.close();

}

FastPokerHandEvaluator::~FastPokerHandEvaluator() {}

unsigned int FastPokerHandEvaluator::calculateFantasyBonus(unsigned int overall_rank) const {
  if (overall_rank >= 3883) {
    if (game_type == GameType::Regular) return 9;
    else if (game_type == GameType::Progressive) {
      if (overall_rank < 4115) return 9;
      else if (overall_rank < 4347) return 14;
      else return 19;
    } 
    else if (game_type == GameType::Ultimate) {
      if (overall_rank < 4115) return 8;
      else if (overall_rank < 4347) return 15;
      else return 25;
    }
  }
  return 0;
}

PokerHandInfo FastPokerHandEvaluator::evalBottom(const std::vector<Card> &row) const {
  int index = 28561 * (row[0] & 15) + 2197 * (row[1] & 15) + 169 * (row[2] & 15) + 13 * (row[3] & 15) + (row[4] & 15);
  PokerHandInfo info = bottom_eval_info[index];
  if (row[0] & row[1] & row[2] & row[3] & row[4] & SUIT_MASK > 0) { // flush

  }
  return bottom_eval_info[index];
}

PokerHandInfo FastPokerHandEvaluator::evalMiddle(const std::vector<Card> &row) const {
  int index = 28561 * (row[0] & 15) + 2197 * (row[1] & 15) + 169 * (row[2] & 15) + 13 * (row[3] & 15) + (row[4] & 15);
  return middle_eval_info[index];
}

PokerHandInfo FastPokerHandEvaluator::evalTop(const std::vector<Card> &row) const {
  int index = 169 * (row[2] & 15) + 13 * (row[3] & 15) + (row[4] & 15);
  return top_eval_info[index];
}
