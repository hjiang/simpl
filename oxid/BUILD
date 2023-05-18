cc_binary(name = 'oxid',
          srcs = ['main.cc'],
          deps = ['oxid-imp'])

cc_library(name = 'oxid-imp',
           srcs = ['oxid.cc'],
           hdrs = ['oxid.h'],
           deps = ['error', 'token', 'lexer', 'interpreter'])

cc_library(name = 'token',
           srcs = ['token.cc'],
           hdrs = ['token.h'])

cc_library(name = 'lexer',
           srcs = ['lexer.cc'],
           hdrs = ['lexer.h'],
           deps = [ 'token', 'error' ])

cc_test(
    name = "lexer_test",
    srcs = ["lexer_test.cc"],
    deps = [
        "lexer",
        "@gtest//:gtest_main"
    ],
    size = "small")

cc_library(name = 'parser',
           srcs = ['parser.cc'],
           hdrs = ['parser.h'],
           deps = ['token', 'error'])

cc_test(
    name = "parser_test",
    srcs = ["parser_test.cc"],
    deps = [
        "lexer",
        "parser",
        "token",
        "@gtest//:gtest_main"
    ],
    size = "small")

cc_library(name = 'interpreter',
           srcs = ['interpreter.cc'],
           hdrs = ['interpreter.h'],
           deps = ['parser', 'error'])

cc_test(name = 'interpreter_test',
        srcs = ['interpreter_test.cc'],
        deps = ['interpreter', 'parser', 'token', 'lexer', '@gtest//:gtest_main'])

cc_library(name = 'error',
           srcs = ['error.cc'],
           hdrs = ['error.h'])

