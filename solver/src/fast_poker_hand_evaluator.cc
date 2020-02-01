#include "fast_poker_hand_evaluator.h"
#include <boost/algorithm/string.hpp>

#include <assert.h> /* assert */
#include <fstream>
#include <iostream>
#include <string>

unsigned int convertHandStr(std::string hand)
{
  unsigned int index = 0;
  for (const char& c : hand) {
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

FastPokerHandEvaluator::FastPokerHandEvaluator(GameType _game_type)
    : game_type { _game_type }
{
  // std::ifstream f("/root/src/solver/src/no_suit_hand_strength.csv");
  std::ifstream f("/home/neerajen/Projects/OFCSolver/solver/src/no_suit_hand_strength.csv");

  if (!f.is_open()) {
    std::cout << "File doesn't exist!. set the value as an environment variable." << std::endl;
    throw 1;
  }

  std::string line;

  /*
  top_eval_info.reserve(2197);
  middle_eval_info.reserve(371293);
  bottom_eval_info.reserve(371293);
  flush_middle_eval_info.reserve(371293);
  flush_bottom_eval_info.reserve(371293);
  */

  std::cout << "Starting FastPokerHandEvaluator..." << std::endl;

  while (getline(f, line)) {
    // sample line: "2224J,5467,3,0" =>
    // hand = "2224J"
    // overall rank = "5467" (relative rank to all other hands)
    // hand type = 3
    // royalties = 2

    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(","));

    std::string hand = tokens[0];
    unsigned int overall_rank = stoi(tokens[1]);
    unsigned int hand_type = stoi(tokens[2]);
    unsigned int royalties = stoi(tokens[3]);
    unsigned int royalties_middle = (hand_type == static_cast<int>(HandType::TRIPS)) ? 2 : (royalties * 2);

    unsigned int hand_index = convertHandStr(hand);

    if (hand.size() == 3) { // top row
      royalties += calculateFantasyBonus(overall_rank);

      top_eval_info[hand_index] = PokerHandUtils::createPokerHandInfo(overall_rank, hand_type, royalties);
    } else {
      middle_eval_info[hand_index] = PokerHandUtils::createPokerHandInfo(overall_rank, hand_type, royalties_middle);
      bottom_eval_info[hand_index] = PokerHandUtils::createPokerHandInfo(overall_rank, hand_type, royalties);

      if (hand_type == static_cast<int>(HandType::HIGH_CARD)) {
        flush_bottom_eval_info[hand_index] = PokerHandUtils::createPokerHandInfo(overall_rank, static_cast<int>(HandType::FLUSH), 4);
        flush_middle_eval_info[hand_index] = PokerHandUtils::createPokerHandInfo(overall_rank, static_cast<int>(HandType::FLUSH), 8);
      } else if (hand_type == static_cast<int>(HandType::STRAIGHT)) {
        if (hand.find("T") != std::string::npos && hand.find("A") != std::string::npos) {
          flush_bottom_eval_info[hand_index] = PokerHandUtils::createPokerHandInfo(overall_rank, static_cast<int>(HandType::ROYAL_FLUSH), 25);
          flush_middle_eval_info[hand_index] = PokerHandUtils::createPokerHandInfo(overall_rank, static_cast<int>(HandType::ROYAL_FLUSH), 50);
        } else {
          flush_bottom_eval_info[hand_index] = PokerHandUtils::createPokerHandInfo(overall_rank, static_cast<int>(HandType::STRAIGHT_FLUSH), 15);
          flush_middle_eval_info[hand_index] = PokerHandUtils::createPokerHandInfo(overall_rank, static_cast<int>(HandType::STRAIGHT_FLUSH), 30);
        }
      }
    }
  }
  std::cout << "Done." << std::endl;

  f.close();
}

FastPokerHandEvaluator::~FastPokerHandEvaluator() {}

unsigned int FastPokerHandEvaluator::calculateFantasyBonus(unsigned int overall_rank) const
{
  if (overall_rank >= 3883) {
    if (game_type == GameType::Regular)
      return 9;
    else if (game_type == GameType::Progressive) {
      if (overall_rank < 4115)
        return 9;
      else if (overall_rank < 4347)
        return 14;
      else
        return 19;
    } else if (game_type == GameType::Ultimate) {
      if (overall_rank < 4115)
        return 8;
      else if (overall_rank < 4347)
        return 15;
      else
        return 25;
    }
  }
  return 0;
}

PokerHandInfo FastPokerHandEvaluator::evalBottom(const std::vector<Card>& row) const
{
  int index = 28561 * (row[0] & 15) + 2197 * (row[1] & 15) + 169 * (row[2] & 15) + 13 * (row[3] & 15) + (row[4] & 15);
  if ((row[0] & row[1] & row[2] & row[3] & row[4] & SUIT_MASK) > 0) { // flush
    return flush_bottom_eval_info[index];
  }

  return bottom_eval_info[index];
}

PokerHandInfo FastPokerHandEvaluator::evalMiddle(const std::vector<Card>& row) const
{
  int index = 28561 * (row[0] & 15) + 2197 * (row[1] & 15) + 169 * (row[2] & 15) + 13 * (row[3] & 15) + (row[4] & 15);
  if ((row[0] & row[1] & row[2] & row[3] & row[4] & SUIT_MASK) > 0) { // flush
    return flush_middle_eval_info[index];
  }

  return middle_eval_info[index];
}

PokerHandInfo FastPokerHandEvaluator::evalTop(const std::vector<Card>& row) const
{
  int index = 169 * (row[0] & 15) + 13 * (row[1] & 15) + (row[2] & 15);
  return top_eval_info[index];
}
