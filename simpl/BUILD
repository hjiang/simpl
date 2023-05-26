cc_library(name= 'config',
           hdrs = ['config.h'])

cc_binary(name = 'simpl',
          srcs = ['main.cc'],
          deps = ['simpl-imp'])

cc_library(name = 'simpl-imp',
           srcs = ['simpl.cc'],
           hdrs = ['simpl.h'],
           deps = ['error', 'token', 'lexer', 'interpreter'])

cc_library(name = 'token',
           srcs = ['token.cc'],
           hdrs = ['token.h'],
           deps = ['config'])

cc_library(name = 'lexer',
           srcs = ['lexer.cc'],
           hdrs = ['lexer.h'],
           deps = [ 'token', 'error', 'config' ])

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
           deps = ['token', 'error', 'config'])

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
           srcs = [
               'arithmetic.cc',
               'callable.cc',
               'comparison.cc',
               'interpreter.cc',
               'interpreter_util.cc',
               'io.cc',
               'logic.cc',
               'user_fn.cc',
           ],
           hdrs = [
               'arithmetic.h',
               'callable.h',
               'comparison.h',
               'interpreter.h',
               'interpreter_util.h',
               'io.h',
               'logic.h',
               'user_fn.h',
           ],
           deps = ['parser', 'error', 'config'])

cc_test(name = 'interpreter_test',
        srcs = ['interpreter_test.cc'],
        deps = ['interpreter', 'parser', 'token', 'lexer', '@gtest//:gtest_main'],
        size="small")

cc_library(name = 'error',
           srcs = ['error.cc'],
           hdrs = ['error.h'])

