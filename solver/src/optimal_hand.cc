#include "optimal_hand.h"

namespace OptimalHand {

using namespace std;

vector<set<Card>> comb(const set<Card>& _cards, int K)
{
  vector<Card> cards(_cards.begin(), _cards.end());
  std::string bitmask(K, 1);       // K leading 1's
  bitmask.resize(cards.size(), 0); // N-K trailing 0's

  vector<set<Card>> out;
  // print integers and permute bitmask
  do {
    set<Card> tmp;

    for (unsigned int i = 0; i < cards.size(); ++i) // [0..N-1] integers
    {
      if (bitmask[i])
        tmp.insert(cards[i]);
    }
    out.emplace_back(tmp); // TODO: verify move semantics
  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

  return out;
}

CompletedHand constructOptimalHand(const Hand& h, std::set<Card>& cards, const FastPokerHandEvaluator* evaluator)
{
  int topCardsMissing = 3 - h.top.size();
  int middleCardsMissing = 5 - h.middle.size();
  int bottomCardsMissing = 5 - h.bottom.size();

  CompletedHand bestHand;
  int highestRoyalties = -1;

  vector<set<Card>> botCombos = comb(cards, bottomCardsMissing);
  if (botCombos.empty())
    botCombos.push_back(set<Card>());

  for (auto& botCombo : botCombos) {
    vector<Card> completed_bottom(h.bottom.begin(), h.bottom.end());
    completed_bottom.insert(completed_bottom.end(), botCombo.begin(), botCombo.end());
    PokerHandInfo botInfo = evaluator->evalBottom(completed_bottom);

    if (h.size() <= 7 && highestRoyalties >= 0 && botInfo < PokerHandUtils::createPokerHandInfo(4118, static_cast<int>(HandType::PAIR), 0))
      continue; // don't continue if bottom is less than KK

    set<Card> remainingCards;
    std::set_difference(
        cards.begin(), cards.end(),
        botCombo.begin(), botCombo.end(),
        inserter(remainingCards, remainingCards.begin()));

    vector<set<Card>> midCombos = comb(remainingCards, middleCardsMissing);
    if (midCombos.empty())
      midCombos.push_back(set<Card>());

    for (auto& midCombo : midCombos) {
      vector<Card> completed_middle(h.middle.begin(), h.middle.end());
      completed_middle.insert(completed_middle.end(), midCombo.begin(), midCombo.end());
      PokerHandInfo midInfo = evaluator->evalMiddle(completed_middle);

      if (PokerHandUtils::getOverallRank(botInfo) < PokerHandUtils::getOverallRank(midInfo))
        continue; // fouled hand
      if (h.size() <= 7 && highestRoyalties > (PokerHandUtils::getRoyalties(botInfo) + PokerHandUtils::getRoyalties(midInfo)) && midInfo < PokerHandUtils::createPokerHandInfo(3886, static_cast<int>(HandType::PAIR), 0))
        continue; // don't continue if mid is less than 66

      set<Card> topRemainingCards;
      std::set_difference(remainingCards.begin(), remainingCards.end(), midCombo.begin(),
          midCombo.end(), inserter(topRemainingCards, topRemainingCards.begin()));

      vector<set<Card>> topCombos = comb(topRemainingCards, topCardsMissing);
      if (topCombos.empty())
        topCombos.push_back(set<Card>());

      for (auto& topCombo : topCombos) {
        vector<Card> completed_top(h.top.begin(), h.top.end());
        completed_top.insert(completed_top.end(), topCombo.begin(), topCombo.end());
        PokerHandInfo topInfo = evaluator->evalTop(completed_top);

        if (PokerHandUtils::getOverallRank(midInfo) < PokerHandUtils::getOverallRank(topInfo))
          continue; // fouled hand

        int royalties = PokerHandUtils::getRoyalties(topInfo) + PokerHandUtils::getRoyalties(midInfo) + PokerHandUtils::getRoyalties(botInfo);
        if (royalties > highestRoyalties) {
          highestRoyalties = royalties;
          bestHand = CompletedHand(topInfo, midInfo, botInfo);
        }
      }
    }
  }

  if (highestRoyalties == -1) { // all possible hands are fouled
    return CompletedHand(
        PokerHandUtils::createPokerHandInfo(0, 0, 0),
        PokerHandUtils::createPokerHandInfo(0, 0, 0),
        PokerHandUtils::createPokerHandInfo(0, 0, 0));
  }

  return bestHand;
}
}
