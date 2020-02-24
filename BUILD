cc_binary(
    name = "server",
    srcs = ["server/server.cc"],
    copts = [
        "-Isolver/include/",
        "-O3",
        "-DSTUDENTCS",
        ],
    deps = [
        "//solver:solver",
        "@com_google_absl//absl/strings",
        ],
    )
