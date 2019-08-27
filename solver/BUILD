cc_library(
    name = "solver",
    srcs = glob(["src/*.cc"]),
    hdrs = glob(["include/*.h"]),
    copts = [
        "-Isolver/include/", 
        "-O3",
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
    name = "benchmark_solver",
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
