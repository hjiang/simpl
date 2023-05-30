cc_library(name= 'config',
           hdrs = ['config.h'])

cc_binary(name = 'simpl',
          srcs = ['main.cc'],
          deps = ['simpl-imp'],
          visibility = ["//visibility:public"])

cc_library(name = 'simpl-imp',
           srcs = ['simpl.cc'],
           hdrs = ['simpl.h'],
           deps = [
               'error',
               'interpreter',
               'lexer',
               'parser',
               'token',
           ])

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

cc_library(name ="ast",
           srcs = [],
           hdrs = ['ast.h'],
           deps = ['config'])

cc_library(name = 'parser',
           srcs = ['parser.cc'],
           hdrs = ['parser.h'],
           deps = ['ast', 'token', 'error', 'config'])

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
               'built_in/arithmetic.cc',
               'built_in/assert.cc',
               'built_in/comparison.cc',
               'built_in/io.cc',
               'built_in/logic.cc',
               'callable.cc',
               'interpreter.cc',
               'interpreter_util.cc',
               'user_fn.cc',
           ],
           hdrs = [
               'built_in/arithmetic.h',
               'built_in/assert.h',
               'built_in/comparison.h',
               'built_in/io.h',
               'built_in/logic.h',
               'callable.h',
               'interpreter.h',
               'interpreter_util.h',
               'user_fn.h',
           ],
           deps = ['ast', 'error', 'config'])

cc_test(name = 'interpreter_test',
        srcs = ['interpreter_test.cc'],
        deps = ['interpreter', 'parser', 'token', 'lexer', '@gtest//:gtest_main'],
        size="small")

cc_library(name = 'error',
           srcs = ['error.cc'],
           hdrs = ['error.h'])

