cc_binary(name = 'oxid-cli',
          srcs = ['main.cc'],
          deps = ['oxid'])

cc_library(name = 'oxid',
           srcs = ['oxid.cc'],
           hdrs = ['oxid.h'],
           deps = ['error', 'token', 'lexer'])

cc_library(name = 'token',
           srcs = ['token.cc'],
           hdrs = ['token.h'])

cc_library(name = 'lexer',
           srcs = ['lexer.cc'],
           hdrs = ['lexer.h'],
           deps = [ 'token', 'error' ])

cc_library(name = 'error',
           srcs = ['error.cc'],
           hdrs = ['error.h'])

cc_test(
    name = "lexer_test",
    srcs = ["lexer_test.cc"],
    deps = [
        "lexer",
        "@gtest//:gtest_main"
    ],
    size = "small"
)
