include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/40412d85124f7c6f3d88454583c4633e5e10fc8c.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(
  lexer_test
  lexer_test.cc
)
set_default_properties(lexer_test)
target_link_libraries(
  lexer_test
  lexer
  GTest::gtest_main
)
gtest_discover_tests(lexer_test)

add_executable(
  parser_test
  parser_test.cc
)
set_default_properties(parser_test)
target_link_libraries(
  parser_test
  parser
  lexer
  GTest::gtest_main
)
gtest_discover_tests(parser_test)

add_executable(
  interpreter_test
  interpreter_test.cc
)
set_default_properties(interpreter_test)
target_link_libraries(
  interpreter_test
  parser
  token
  lexer
  interpreter
  GTest::gtest_main
)
gtest_discover_tests(interpreter_test)
