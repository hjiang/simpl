add_library(ast ast.cc callable.cc)
set_default_properties(ast)

add_library(token token.cc)
set_default_properties(token)

add_library(lexer lexer.cc)
set_default_properties(lexer)
target_link_libraries(lexer token error)

add_library(error error.cc)
set_default_properties(error)

add_library(parser parser.cc)
set_default_properties(parser)
target_link_libraries(parser ast error)

add_library(interpreter interpreter.cc function.cc interpreter_util.cc user_fn.cc)
set_default_properties(interpreter)
target_link_libraries(interpreter ast error)
