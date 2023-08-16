cc_library(name= 'config',
           hdrs = ['config.h'])

cc_binary(name = 'simpl',
          srcs = ['main.cc'],
          deps = ['simplib'],
          visibility = ['//visibility:public'])

cc_library(name = 'simplib',
           srcs = ['simpl.cc'],
           hdrs = ['simpl.h'],
           deps = [
               'error',
               'interpreter',
               'lexer',
               'parser',
               'token',
           ])

cc_test(name = 'simpl_test',
        srcs = ['simpl_test.cc'],
        deps = [
            '@gtest//:gtest_main',
            'ast',
            'config',
            'interpreter',
            'simplib',
        ],
        size = 'small')

cc_library(name = 'concepts',
           hdrs = ['concepts.hh'])

cc_library(name = 'token',
           srcs = ['token.cc'],
           hdrs = ['token.h'],
           deps = ['config'])

cc_library(name = 'lexer',
           srcs = ['lexer.cc'],
           hdrs = ['lexer.h'],
           deps = [ 'token', 'error', 'config' ])

cc_test(
    name = 'lexer_test',
    srcs = ['lexer_test.cc'],
    deps = [
        'lexer',
        '@gtest//:gtest_main'
    ],
    size = 'small')

cc_library(name ='ast',
           srcs = ['ast.cc', 'callable.cc'],
           hdrs = ['ast.h', 'callable.h', 'overload.h'],
           deps = ['config'])

cc_test(name='ast_test',
        srcs=['ast_test.cc'],
        deps=[
            '@gtest//:gtest_main',
            'ast',
            'lexer',
            'parser',
        ],
        size='small')

cc_library(name = 'parser',
           srcs = ['parser.cc'],
           hdrs = ['parser.h'],
           deps = ['ast', 'token', 'error', 'config'])

cc_test(
    name = 'parser_test',
    srcs = ['parser_test.cc'],
    deps = [
        'ast',
        'lexer',
        'parser',
        'token',
        '@gtest//:gtest_main'
    ],
    size = 'small')

cc_library(name='built_in_util',
           hdrs = ['built_in/util.h'])

cc_library(name = 'interpreter',
           srcs = [
               'built_in/arithmetic.cc',
               'built_in/assert.cc',
               'built_in/cdt_ops.cc',
               'built_in/comparison.cc',
               'built_in/control_flow.cc',
               'built_in/def.cc',
               'built_in/fn.cc',
               'built_in/io.cc',
               'built_in/logic.cc',
               'built_in/sequence.cc',
               'function.cc',
               'interpreter.cc',
               'interpreter_util.cc',
               'user_fn.cc',
           ],
           hdrs = [
               'built_in/arithmetic.h',
               'built_in/assert.h',
               'built_in/cdt_ops.h',
               'built_in/comparison.h',
               'built_in/control_flow.h',
               'built_in/def.h',
               'built_in/fn.h',
               'built_in/io.h',
               'built_in/logic.h',
               'built_in/sequence.h',
               'function.h',
               'interpreter.h',
               'interpreter_util.h',
               'user_fn.h',
           ],
           deps = ['ast', 'built_in_util', 'error', 'config', 'concepts'])

cc_test(name = 'interpreter_test',
        srcs = ['interpreter_test.cc'],
        deps = ['interpreter', 'parser', 'token', 'lexer', '@gtest//:gtest_main'],
        size='small')

cc_library(name = 'error',
           srcs = ['error.cc'],
           hdrs = ['error.h'])

