#include "absl/strings/str_split.h"
#include <benchmark/benchmark.h>
#include <vector>
#include <set>
#include <iostream>

#include "poker_hand_evaluator.h"
#include "gametype.h"
#include "position.h"
#include "card.h"

static PokerHandEvaluator *evaluator = new PokerHandEvaluator(GameType::Regular);

static std::set<Card> parseCards(std::string cards) {
    std::set<Card> parsedCards;
    std::vector<std::string> tokens = absl::StrSplit(cards, " ");
    for (auto token : tokens) {
      assert(token.size() == 2);
      parsedCards.insert(Card(token));
    }

    return parsedCards;
}

static void BM_PokerHandEvaluator(benchmark::State& state) {
  // Perform setup here
  std::set<Card> cards_1 = parseCards("Ah Kh Qh Jh Th");
  std::set<Card> cards_2 = parseCards("As Kh Qh Jh Th");
  std::set<Card> cards_3 = parseCards("4h 4s 4c 4d As");
  std::set<Card> cards_4 = parseCards("2c 4d 4c 2s Ah");

  int a[4];
  a[0] = 1;
  a[1] = 2;
  a[2] = 3;
  a[3] = 4;

  for (auto _ : state) {
    // PokerHandInfo *res_1 = evaluator->eval(cards_1, Position::bottom);
    int product = 1; 
    for (int i = 0; i < 4; ++i) {
      product *= a[i];

    }
    /*
    PokerHandInfo *res_2 = evaluator->eval(cards_2, Position::bottom);
    PokerHandInfo *res_3 = evaluator->eval(cards_3, Position::bottom);
    PokerHandInfo *res_4 = evaluator->eval(cards_4, Position::bottom);
    */
  }
}

// Register the function as a benchmark
BENCHMARK(BM_PokerHandEvaluator);
// Run the benchmark
BENCHMARK_MAIN();
