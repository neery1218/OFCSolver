#include <benchmark/benchmark.h>
#include <iostream>
#include <set>
#include <vector>

#include "card.h"
#include "fast_poker_hand_evaluator.h"
#include "gametype.h"
#include "hand.h"
#include "position.h"
#include "pull.h"
#include "solver.h"

static FastPokerHandEvaluator evaluator(GameType::Regular);

static void FastPokerHandEvaluator(benchmark::State& state)
{
  // Perform setup here
  std::vector<Card> cards_1 = CardUtils::parseCards("Ah Kh Qh Jh Th");
  std::vector<Card> cards_2 = CardUtils::parseCards("As Kh Qh Jh Th");
  std::vector<Card> cards_3 = CardUtils::parseCards("4h 4s 4c 4d As");
  std::vector<Card> cards_4 = CardUtils::parseCards("2c 4d 4c 2s Ah");

  for (auto _ : state) {
    PokerHandInfo res_1 = evaluator.evalBottom(cards_1);
    /*
    PokerHandInfo *res_2 = evaluator->evalBottom(cards_2, Position::bottom);
    PokerHandInfo *res_3 = evaluator->evalBottom(cards_3, Position::bottom);
    PokerHandInfo *res_4 = evaluator->evalBottom(cards_4, Position::bottom);
    */
  }
}

static void Solver_2(benchmark::State& state)
{
  Hand hand(
      CardUtils::parseCards("Ac"),
      CardUtils::parseCards("2c 2d"),
      CardUtils::parseCards("9h 9d"));

  Pull pull { CardUtils::parseCards("Ac 4c 4d") };
  for (auto _ : state) {
    double ev = Solver(&evaluator).solve(state.range(0), hand, pull, std::vector<Hand>(), std::vector<Card>());
  }
}

static void Solver_3(benchmark::State& state)
{
  Hand hand(
      CardUtils::parseCards("Ac"),
      CardUtils::parseCards("2c 2d 3d"),
      CardUtils::parseCards("9h 9d 9c"));

  Pull pull { CardUtils::parseCards("Ac 4c 4d") };
  for (auto _ : state) {
    double ev = Solver(&evaluator).solve(state.range(0), hand, pull, std::vector<Hand>(), std::vector<Card>());
  }
}

static void Solver_4(benchmark::State& state)
{
  Hand hand(
      CardUtils::parseCards("Ac"),
      CardUtils::parseCards("2c 2d 3d 4s"),
      CardUtils::parseCards("9h 9d 9c 9s"));

  Pull pull { CardUtils::parseCards("Ac 4c 4d") };
  for (auto _ : state) {
    double ev = Solver(&evaluator).solve(state.range(0), hand, pull, std::vector<Hand>(), std::vector<Card>());
  }
}

// Register the function as a benchmark
// BENCHMARK(FastPokerHandEvaluator)->Unit(benchmark::kMicrosecond);

BENCHMARK(Solver_4)->RangeMultiplier(4)->Range(8, 15000)->Unit(benchmark::kMicrosecond);
BENCHMARK(Solver_3)->RangeMultiplier(4)->Range(8, 15000)->Unit(benchmark::kMicrosecond);
BENCHMARK(Solver_2)->RangeMultiplier(4)->Range(8, 15000)->Unit(benchmark::kMicrosecond);

// Run the benchmark
BENCHMARK_MAIN();
