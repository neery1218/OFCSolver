cc_binary(
    name = "server",
    srcs = [
        "server/include/single_include/nlohmann/json.hpp",
        "server/server.cc"
        ],
    copts = [
        "-Isolver/include/",
        "-Iserver/include/",
        "-O3",
        "-DRESEARCH",
        "-std=c++17",
        ],
    # hdrs = ["server/include/single_include/nlohmann/json.hpp"],
    deps = [
        "//solver:solver",
        "@com_google_absl//absl/strings",
        ],
    )
