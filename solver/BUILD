cc_library(
    name = "solver",
    srcs = glob(["src/*.cc"]),
    hdrs = glob(["include/*.h"]),
    copts = ["-Isolver/include/", "-O3"] # This is the key part
)

cc_test(
    name = "test_poker_hand_evaluator",
    srcs = ["tests/test_poker_hand_evaluator.cc"],
    copts = [
        "-Iexternal/gtest/include", 
        "-Isolver/include",
	"-O3",
    ],
    deps = [
        "@gtest//:main",
        "@com_google_absl//absl/strings",
        "solver"
    ]
)

cc_test(
    name = "benchmark_poker_hand_evaluator",
    srcs = [
        "tests/benchmark_poker_hand_evaluator.cc",
    ],
    copts = [
        "-Isolver/include",
    "-DCMAKE_BUILD_TYPE=Release"
    ],
    tags = ["benchmark"],
    deps = [
        "@com_google_benchmark//:benchmark_main",
        "@com_google_absl//absl/strings",
        "solver"
    ]
)
