cc_library(
    name = "solver",
    srcs = glob(["src/*.cc"]),
    hdrs = glob(["include/*.h"]),
    copts = [
        "-Isolver/include/", 
        "-O3",
    ],
    deps = [
        "@com_google_absl//absl/strings",
    ],
)

# profile targets
cc_library(
    name = "profile_solver_lib",
    srcs = glob(["src/*.cc"]),
    hdrs = glob(["include/*.h"]),
    copts = [
        "-Isolver/include/", 
        "-O3", "-fno-omit-frame-pointer"
    ]
)

cc_binary(
    name = "profile_solver",
    srcs = ["profile/profile_solver.cc"],
    deps = [
        "profile_solver_lib",
        "@com_google_absl//absl/strings",
    ],
    copts = [
        "-Isolver/include/", 
        "-O3",
        "-fno-omit-frame-pointer",
    ],
)

cc_test(
    name = "test_poker_hand_evaluator",
    srcs = [
        "tests/test_poker_hand_evaluator.cc"
    ],
    copts = [
        "-Iexternal/gtest/include", 
        "-Isolver/include",
	"-O3",
    ],
    deps = [
        "@gtest//:main",
        "@com_google_absl//absl/strings",
    ]
)

cc_test(
    name = "test_solver",
    srcs = ["tests/test_solver.cc"],
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
    name = "benchmark_fast_poker_hand_evaluator",
    srcs = [
        "tests/benchmark_fast_poker_hand_evaluator.cc",
        "include/fast_poker_hand_evaluator.h",
        "include/position.h",
        "include/gametype.h",
        "include/card.h",
        "src/fast_poker_hand_evaluator.cc",
    ],
    copts = [
        "-Isolver/include",
        "-O3",
    ],
    tags = ["benchmark"],
    deps = [
        "@com_google_benchmark//:benchmark_main",
        "@com_google_absl//absl/strings",
    ]
)

cc_test(
    name = "benchmark_solver",
    srcs = [
        "tests/benchmark_poker_hand_evaluator.cc",
        "src/poker_hand_evaluator.cc",
        "include/card.h",
        "include/gametype.h",
        "include/position.h",
        "include/poker_hand_evaluator.h",
        "include/hand.h",
    ],
    copts = [
        "-Isolver/include",
    "-DCMAKE_BUILD_TYPE=Release"
    ],
    tags = ["benchmark"],
    deps = [
        "@com_google_benchmark//:benchmark_main",
        "@com_google_absl//absl/strings",
    ]
)

# tests
cc_test(
    name = "test_card",
    srcs = [
      "tests/test_card.cc", 
      "include/card.h",
      "include/gametype.h"
    ],
    tags = ["testcard"],
    copts = [
      "-Isolver/include",
      "-Iexternal/gtest/include", 
    ],
    deps = [
        "@gtest//:main",
        "@com_google_absl//absl/strings",
    ]
)

cc_test(
    name = "test_poker_hand_info",
    srcs = [
      "tests/test_poker_hand_info.cc", 
      "include/poker_hand_info.h",
    ],
    tags = ["test"],
    copts = [
      "-Isolver/include",
      "-Iexternal/gtest/include", 
    ],
    deps = [
        "@gtest//:main",
    ]
)

cc_test(
    name = "test_fast_poker_hand_evaluator",
    srcs = [
        "tests/test_fast_poker_hand_evaluator.cc",
        "src/fast_poker_hand_evaluator.cc",
        "src/poker_hand_evaluator.cc",
        "include/fast_poker_hand_evaluator.h",
        "include/poker_hand_evaluator.h",
        "include/poker_hand_info.h",
        "include/gametype.h",
        "include/card.h",
        "include/position.h"
    ],
    copts = [
        "-Iexternal/gtest/include", 
        "-Isolver/include",
	"-O3",
    ],
    deps = [
        "@gtest//:main",
        "@com_google_absl//absl/strings",
    ]
)
