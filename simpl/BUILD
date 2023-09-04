cc_library(name= 'config',
           hdrs = ['config.hh'])

cc_binary(name = 'simpl',
          srcs = ['main.cc'],
          deps = ['simplib'],
          visibility = ['//visibility:public'])

cc_library(name = 'simplib',
           srcs = ['simpl.cc',
                   '//simpl_lib:arrows.hh'],
           hdrs = ['simpl.hh'],
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

cc_library(name = 'util',
           hdrs = ['util.hh'])

cc_library(name = 'token',
           srcs = ['token.cc'],
           hdrs = ['token.hh'],
           deps = ['config'])

cc_library(name = 'lexer',
           srcs = ['lexer.cc'],
           hdrs = ['lexer.hh'],
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
           hdrs = ['ast.hh', 'callable.hh', 'overload.hh'],
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
           hdrs = ['parser.hh'],
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
           hdrs = ['built_in/util.hh'])

cc_library(name = 'interpreter',
           srcs = [
               'built_in/arithmetic.cc',
               'built_in/assert.cc',
               'built_in/cdt_ops.cc',
               'built_in/comparison.cc',
               'built_in/control_flow.cc',
               'built_in/def.cc',
               'built_in/eval.cc',
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
               'built_in/arithmetic.hh',
               'built_in/assert.hh',
               'built_in/cdt_ops.hh',
               'built_in/comparison.hh',
               'built_in/control_flow.hh',
               'built_in/def.hh',
               'built_in/eval.hh',
               'built_in/fn.hh',
               'built_in/io.hh',
               'built_in/logic.hh',
               'built_in/sequence.hh',
               'function.hh',
               'interpreter.hh',
               'interpreter_util.hh',
               'user_fn.hh',
           ],
           deps = ['ast', 'built_in_util', 'error', 'config', 'util'])

cc_test(name = 'interpreter_test',
        srcs = ['interpreter_test.cc'],
        deps = ['interpreter', 'parser', 'token', 'lexer', '@gtest//:gtest_main'],
        size='small')

cc_library(name = 'error',
           srcs = ['error.cc'],
           hdrs = ['error.hh'])

