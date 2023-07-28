include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
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
