#include "action_enumerator.h"

#include <cassert>
#include <stdexcept>
#include <unordered_set>

#include "completed_hand.h"

using namespace std;

vector<Decision> ActionEnumerator::findAllSetDecisionsHelper(
    const set<Card>& cards, const vector<Placement>& acc) {
  vector<Decision> out;

  if (cards.empty()) {
    return vector<Decision>{Decision(acc)};
  }

  Card c = *cards.begin();
  set<Card> subCards(cards);
  subCards.erase(c);

  vector<Placement> topAcc(acc);
  topAcc.push_back(Placement{c, Position::top});
  vector<Decision> topDecisions = findAllSetDecisionsHelper(subCards, topAcc);
  out.insert(out.end(), topDecisions.begin(), topDecisions.end());

  vector<Placement> midAcc(acc);
  midAcc.push_back(Placement{c, Position::middle});
  vector<Decision> midDecisions = findAllSetDecisionsHelper(subCards, midAcc);
  out.insert(out.end(), midDecisions.begin(), midDecisions.end());

  vector<Placement> botAcc(acc);
  botAcc.push_back(Placement{c, Position::bottom});
  vector<Decision> botDecisions = findAllSetDecisionsHelper(subCards, botAcc);
  out.insert(out.end(), botDecisions.begin(), botDecisions.end());

  return out;
}

vector<Decision> ActionEnumerator::findAllSetDecisions(
    const GameState& game_state) {
  if (game_state.my_pull.cards.size() != 5) {
    throw std::runtime_error("My pull cards size != 5!");
  }
  set<Card> pull_set(game_state.my_pull.cards.begin(),
                     game_state.my_pull.cards.end());
  vector<Decision> decisions =
      findAllSetDecisionsHelper(pull_set, vector<Placement>());
  unordered_set<string> fantasy_cards = {"Ah", "Ad", "Ac", "As", "Kh", "Kd",
                                         "Kc", "Ks", "Qh", "Qd", "Qc", "Qs"};
  vector<Decision> validDecisions;

  for (auto& d : decisions) {
    int numTop = 0, numMid = 0, numBot = 0;
    int numNonFantasyCardsTop = 0;
    for (auto& p : d.placements) {
      if (p.position == Position::top) {
        ++numTop;
        if (fantasy_cards.count(CardUtils::cardToString(p.card)) == 0)
          ++numNonFantasyCardsTop;
      } else if (p.position == Position::middle)
        ++numMid;
      else if (p.position == Position::bottom)
        ++numBot;
    }
    if (numTop <= 3 && numMid <= 5 && numBot <= 5 &&
        (numNonFantasyCardsTop < 1 ||
         game_state.other_hands.size() > 0 && numNonFantasyCardsTop < 2))
      validDecisions.push_back(d);
  }

  return validDecisions;
}

vector<Decision> ActionEnumerator::findAllDrawDecisions(
    const GameState& game_state, const FastPokerHandEvaluator* evaluator) {
  if (game_state.my_pull.cards.size() != 3) {
    throw std::runtime_error("My pull cards size != 3!");
  }

  int topCardsMissing = 3 - game_state.my_hand.top.size();
  int midCardsMissing = 5 - game_state.my_hand.middle.size();
  int botCardsMissing = 5 - game_state.my_hand.bottom.size();

  vector<Card> cards(game_state.my_pull.cards.begin(),
                     game_state.my_pull.cards.end());
  vector<Decision> decisions;

  for (int i = 0; i < cards.size(); ++i) {
    if (topCardsMissing > 0) {
      --topCardsMissing;
      Placement p1(cards[i], Position::top);
      for (int j = i + 1; j < cards.size(); ++j) {
        if (topCardsMissing > 0) {
          decisions.emplace_back(
              Decision{p1, Placement(cards[j], Position::top)});
        }

        if (midCardsMissing > 0) {
          decisions.emplace_back(
              Decision{p1, Placement(cards[j], Position::middle)});
        }

        if (botCardsMissing > 0) {
          decisions.emplace_back(
              Decision{p1, Placement(cards[j], Position::bottom)});
        }
      }
      ++topCardsMissing;
    }

    if (midCardsMissing > 0) {
      --midCardsMissing;
      Placement p1(cards[i], Position::middle);
      for (int j = i + 1; j < cards.size(); ++j) {
        if (topCardsMissing > 0) {
          decisions.emplace_back(
              Decision{p1, Placement(cards[j], Position::top)});
        }

        if (midCardsMissing > 0) {
          decisions.emplace_back(
              Decision{p1, Placement(cards[j], Position::middle)});
        }

        if (botCardsMissing > 0) {
          decisions.emplace_back(
              Decision{p1, Placement(cards[j], Position::bottom)});
        }
      }
      ++midCardsMissing;
    }

    if (botCardsMissing > 0) {
      --botCardsMissing;
      Placement p1(cards[i], Position::bottom);
      for (int j = i + 1; j < cards.size(); ++j) {
        if (topCardsMissing > 0) {
          decisions.emplace_back(
              Decision{p1, Placement(cards[j], Position::top)});
        }

        if (midCardsMissing > 0) {
          decisions.emplace_back(
              Decision{p1, Placement(cards[j], Position::middle)});
        }

        if (botCardsMissing > 0) {
          decisions.emplace_back(
              Decision{p1, Placement(cards[j], Position::bottom)});
        }
      }
      ++botCardsMissing;
    }
  }

  // special case: if the hand has 11 cards, never consider a fouled decision.
  if (game_state.my_hand.size() == 11) {
    std::vector<Decision> final_decisions;
    for (auto decision : decisions) {
      Hand h = game_state.my_hand.applyDecision(decision);
      CompletedHand ch(h, evaluator);
      if (!ch.is_fouled) {
        final_decisions.push_back(decision);
      }
    }

    if (final_decisions.size() == 0) {
      std::cout << "All decisions are fouled!\n";
      return decisions;
    }

    return final_decisions;
  }

  return decisions;
}
