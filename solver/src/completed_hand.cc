#include "completed_hand.h"

#include <bitset>

#include "hand.h"

CompletedHand::CompletedHand(const Hand& h,
                             const FastPokerHandEvaluator* eval) {
  topInfo = eval->evalTop(h.top);
  middleInfo = eval->evalMiddle(h.middle);
  bottomInfo = eval->evalBottom(h.bottom);
  is_fouled = false;
  fantasyType = FantasyType::NONE;

  if (topInfo > middleInfo || middleInfo > bottomInfo) {
    is_fouled = true;
    topInfo = PokerHandUtils::createPokerHandInfo(0, 0, 0);
    middleInfo = PokerHandUtils::createPokerHandInfo(0, 0, 0);
    bottomInfo = PokerHandUtils::createPokerHandInfo(0, 0, 0);
  }

  // are we in fantasy?
  if (!is_fouled && PokerHandUtils::getRoyalties(topInfo) >= 7) {
    // tightly coupled with no_suit_hand_strength
    unsigned int rank = PokerHandUtils::getOverallRank(topInfo);
    if (rank >= 4347) {
      fantasyType = FantasyType::ACES;
    } else if (rank >= 4115) {
      fantasyType = FantasyType::KINGS;
    } else {
      fantasyType = FantasyType::QUEENS;
    }
  }
}

CompletedHand::CompletedHand(const PokerHandInfo topInfo,
                             const PokerHandInfo middleInfo,
                             const PokerHandInfo bottomInfo)
    : topInfo{topInfo}, middleInfo{middleInfo}, bottomInfo{bottomInfo} {
  is_fouled = false;
  fantasyType = FantasyType::NONE;

  if (topInfo > middleInfo || middleInfo > bottomInfo) {
    is_fouled = true;
  }

  // are we in fantasy?
  if (!is_fouled && PokerHandUtils::getRoyalties(topInfo) >= 7) {
    // tightly coupled with no_suit_hand_strength
    unsigned int rank = PokerHandUtils::getOverallRank(topInfo);
    if (rank >= 4347) {
      fantasyType = FantasyType::ACES;
    } else if (rank >= 4115) {
      fantasyType = FantasyType::KINGS;
    } else {
      fantasyType = FantasyType::QUEENS;
    }
  }
}

int CompletedHand::calculatePoints()
    const {  // note: can't call this with a fouled hand. tightly coupled with
             // solver.h
  if (is_fouled) return 0;
  return PokerHandUtils::getRoyalties(topInfo) +
         PokerHandUtils::getRoyalties(middleInfo) +
         PokerHandUtils::getRoyalties(bottomInfo);
}

int CompletedHand::calculatePoints(const CompletedHand& otherHand) const {
  int royaltyPoints = PokerHandUtils::getRoyalties(topInfo) +
                      PokerHandUtils::getRoyalties(middleInfo) +
                      PokerHandUtils::getRoyalties(bottomInfo) -
                      PokerHandUtils::getRoyalties(otherHand.topInfo) -
                      PokerHandUtils::getRoyalties(otherHand.middleInfo) -
                      PokerHandUtils::getRoyalties(otherHand.bottomInfo);

  int gtBonus = 0;
  if (topInfo < otherHand.topInfo)
    gtBonus -= 1;
  else if (otherHand.topInfo < topInfo)
    gtBonus += 1;

  if (middleInfo < otherHand.middleInfo)
    gtBonus -= 1;
  else if (otherHand.middleInfo < middleInfo)
    gtBonus += 1;

  if (bottomInfo < otherHand.bottomInfo)
    gtBonus -= 1;
  else if (otherHand.bottomInfo < bottomInfo)
    gtBonus += 1;

  if (gtBonus == -3) gtBonus = -6;
  if (gtBonus == 3) gtBonus = 6;

  return royaltyPoints + gtBonus;
}
