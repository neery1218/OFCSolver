cc_library(
    name = "solver",
    srcs = glob(["src/*.cc"]),
    hdrs = glob(["include/*.h"]),
    copts = [
        "-Isolver/include/",
        # "-I ../include/",
        # "-I ../../include/",
        "-O3",
        "-DRESEARCH",
        # "-DSTUDENTCS",
        "-std=c++17",
        ],
    linkopts = [
        "-lpthread",
        ],
    deps = [
        "@com_google_absl//absl/strings",
        "@boost//:algorithm",
        "@boost//:pool",
        "@boost//:system",
        "@boost//:asio",
        ],
    visibility = ["//:__pkg__"],
    )

# # profile targets
# cc_library(
#     name = "profile_solver_lib",
#     srcs = glob(["src/*.cc"]),
#     hdrs = glob(["include/*.h"]),
#     copts = [
#         "-Isolver/include/",
#         "-O3", "-fno-omit-frame-pointer"
#         ],
#     deps = [
#         "@com_google_absl//absl/strings"
#         ]
#     )

# cc_binary(
#     name = "profile_solver",
#     srcs = ["profile/profile_solver.cc"],
#     deps = [
#         "profile_solver_lib",
#         "@com_google_absl//absl/strings",
#         ],
#     copts = [
#         "-Isolver/include/",
#         "-O3",
#         "-fno-omit-frame-pointer",
#         ],
#     )

# benchmarking
cc_test(
    name = "benchmark_fast_poker_hand_evaluator",
    srcs = [
        "tests/benchmark_fast_poker_hand_evaluator.cc",
        ],
    copts = [
        "-Isolver/include",
        ],
    tags = ["benchmark"],
    deps = [
        "solver",
        "@com_google_benchmark//:benchmark_main",
        "@com_google_absl//absl/strings",
        ]
    )

# tests
cc_test(
    name = "test_solver",
    srcs = ["tests/test_solver.cc"],
    copts = [
        "-Iexternal/gtest/include",
        "-Isolver/include",
        ],
    deps = [
        "@gtest//:main",
        "@com_google_absl//absl/strings",
        "solver"
        ]
    )

cc_test(
    name = "test_decision_finder",
    srcs = [
        "tests/test_decision_finder.cc",
        ],
    tags = ["testcard"],
    copts = [
        "-Isolver/include",
        "-Iexternal/gtest/include",
        ],
    deps = [
        "solver",
        "@gtest//:main",
        "@com_google_absl//absl/strings",
        ]
    )

cc_test(
    name = "test_card",
    srcs = [
        "tests/test_card.cc",
        ],
    tags = ["testcard"],
    copts = [
        "-Isolver/include",
        "-Iexternal/gtest/include",
        ],
    deps = [
        "@gtest//:main",
        "solver",
        "@com_google_absl//absl/strings",
        ]
    )

cc_test(
    name = "test_advanced_solver",
    srcs = [
        "tests/test_advanced_solver.cc",
        ],
    tags = ["test"],
    copts = [
        "-Isolver/include",
        "-Iexternal/gtest/include",
        ],
    deps = [
        "solver",
        "@gtest//:main",
        ]
    )
cc_test(
    name = "test_advanced_decision_finder",
    srcs = [
        "tests/test_advanced_decision_finder.cc",
        ],
    tags = ["test"],
    copts = [
        "-Isolver/include",
        "-Iexternal/gtest/include",
        ],
    deps = [
        "solver",
        "@gtest//:main",
        ]
    )

cc_test(
    name = "test_poker_hand_info",
    srcs = [
        "tests/test_poker_hand_info.cc",
        ],
    tags = ["test"],
    copts = [
        "-Isolver/include",
        "-Iexternal/gtest/include",
        ],
    deps = [
        "solver",
        "@gtest//:main",
        ]
    )

cc_test(
    name = "test_hand",
    srcs = [
        "tests/test_hand.cc",
        ],
    tags = ["test"],
    copts = [
        "-Isolver/include",
        "-Iexternal/gtest/include",
        ],
    deps = [
        "solver",
        "@gtest//:main",
        ]
    )

cc_test(
    name = "test_deck",
    srcs = [
        "tests/test_deck.cc",
        ],
    tags = ["test"],
    copts = [
        "-Isolver/include",
        "-Iexternal/gtest/include",
        ],
    deps = [
        "@gtest//:main",
        "solver",
        "@com_google_absl//absl/strings",
        ]
    )

cc_test(
    name = "test_fast_poker_hand_evaluator",
    srcs = [
        "tests/test_fast_poker_hand_evaluator.cc",
        ],
    copts = [
        "-Iexternal/gtest/include",
        "-Isolver/include",
        "-O3",
        ],
    deps = [
        "solver",
        "@gtest//:main",
        "@com_google_absl//absl/strings",
        ]
    )
