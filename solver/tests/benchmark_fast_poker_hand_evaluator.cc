#include <benchmark/benchmark.h>
#include <vector>
#include <set>
#include <iostream>

#include "fast_poker_hand_evaluator.h"
#include "gametype.h"
#include "position.h"
#include "card.h"

static FastPokerHandEvaluator *evaluator = new FastPokerHandEvaluator(GameType::Regular);

static void BM_PokerHandEvaluator(benchmark::State& state) {
  // Perform setup here
  std::vector<Card> cards_1 = CardUtils::parseCards("Ah Kh Qh Jh Th");

  for (auto _ : state) {
    PokerHandInfo res_1 = evaluator->evalBottom(cards_1);
    benchmark::DoNotOptimize(res_1);
  }
}

BENCHMARK(BM_PokerHandEvaluator);

// Run the benchmark
BENCHMARK_MAIN();
