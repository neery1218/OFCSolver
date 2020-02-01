#include "completed_hand.h"
#include "hand.h"

CompletedHand::CompletedHand(const Hand& h, const FastPokerHandEvaluator* eval)
{
  topInfo = eval->evalTop(h.top);
  middleInfo = eval->evalMiddle(h.middle);
  bottomInfo = eval->evalBottom(h.bottom);

  if (topInfo > middleInfo || middleInfo > bottomInfo) {
    topInfo = PokerHandUtils::createPokerHandInfo(0, 0, 0);
    middleInfo = PokerHandUtils::createPokerHandInfo(0, 0, 0);
    bottomInfo = PokerHandUtils::createPokerHandInfo(0, 0, 0);
  }
}

int CompletedHand::calculatePoints() const
{ // note: can't call this with a fouled hand. tightly coupled with solver.h
  return PokerHandUtils::getRoyalties(topInfo) + PokerHandUtils::getRoyalties(middleInfo) + PokerHandUtils::getRoyalties(bottomInfo);
}

int CompletedHand::calculatePoints(const CompletedHand& otherHand) const
{
  int royaltyPoints = PokerHandUtils::getRoyalties(topInfo) + PokerHandUtils::getRoyalties(middleInfo) + PokerHandUtils::getRoyalties(bottomInfo) - PokerHandUtils::getRoyalties(otherHand.topInfo) - PokerHandUtils::getRoyalties(otherHand.middleInfo) - PokerHandUtils::getRoyalties(otherHand.bottomInfo);

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

  if (gtBonus == -3)
    gtBonus = -6;
  if (gtBonus == 3)
    gtBonus = 6;

  return royaltyPoints + gtBonus;
}
