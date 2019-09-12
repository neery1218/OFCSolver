#include "absl/strings/str_split.h"
#include <benchmark/benchmark.h>
#include <vector>
#include <set>
#include <iostream>

#include "fast_poker_hand_evaluator.h"
#include "gametype.h"
#include "position.h"
#include "card.h"
#include "hand.h"
#include "solver.h"
#include "pull.h"

static FastPokerHandEvaluator *evaluator = new FastPokerHandEvaluator(GameType::Regular);

static std::vector<Card> parseCards(std::string cards) {
    std::vector<Card> parsedCards;
    std::vector<std::string> tokens = absl::StrSplit(cards, " ");
    for (auto token : tokens) {
      assert(token.size() == 2);
      parsedCards.push_back(CardUtils::parseCard(token));
    }

    return parsedCards;
}

static void BM_FastPokerHandEvaluator(benchmark::State& state) {
  // Perform setup here
  std::vector<Card> cards_1 = parseCards("Ah Kh Qh Jh Th");
  std::vector<Card> cards_2 = parseCards("As Kh Qh Jh Th");
  std::vector<Card> cards_3 = parseCards("4h 4s 4c 4d As");
  std::vector<Card> cards_4 = parseCards("2c 4d 4c 2s Ah");

  for (auto _ : state) {
    PokerHandInfo res_1 = evaluator->evalBottom(cards_1);
    /*
    PokerHandInfo *res_2 = evaluator->evalBottom(cards_2, Position::bottom);
    PokerHandInfo *res_3 = evaluator->evalBottom(cards_3, Position::bottom);
    PokerHandInfo *res_4 = evaluator->evalBottom(cards_4, Position::bottom);
    */
  }
}

static void BM_Solver_2(benchmark::State& state) {
  Hand hand(
      parseCards("Ac"),
      parseCards("2c 2d"),
      parseCards("9h 9d"));

  Pull pull{parseCards("Ac 4c 4d")};
  for (auto _ : state) {
    double ev = Solver(evaluator).solve(
        state.range(0), 
        hand, 
        pull, 
        std::vector<Hand> (), 
        std::vector<Card> ()
    );
  }

}

static void BM_Solver_3(benchmark::State& state) {
  Hand hand(
      parseCards("Ac"),
      parseCards("2c 2d 3d"),
      parseCards("9h 9d 9c"));

  Pull pull{parseCards("Ac 4c 4d")};
  for (auto _ : state) {
    double ev = Solver(evaluator).solve(
        state.range(0), 
        hand, 
        pull, 
        std::vector<Hand> (), 
        std::vector<Card> ()
    );
  }

}

static void BM_Solver_4(benchmark::State& state) {
  Hand hand(
      parseCards("Ac"),
      parseCards("2c 2d 3d 4s"),
      parseCards("9h 9d 9c 9s"));

  Pull pull{parseCards("Ac 4c 4d")};
  for (auto _ : state) {
    double ev = Solver(evaluator).solve(
        state.range(0), 
        hand, 
        pull, 
        std::vector<Hand> (), 
        std::vector<Card> ()
    );
  }

}

// Register the function as a benchmark
BENCHMARK(BM_FastPokerHandEvaluator);

BENCHMARK(BM_Solver_4)->RangeMultiplier(4)->Range(8, 4096)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_Solver_3)->RangeMultiplier(4)->Range(8, 4096)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_Solver_2)->RangeMultiplier(4)->Range(8, 4096)->Unit(benchmark::kMicrosecond);

// Run the benchmark
BENCHMARK_MAIN();
