cmake_minimum_required(VERSION 3.20)

project(MyProject VERSION 0.1
                  DESCRIPTION "A LISP Dialect"
                  LANGUAGES CXX)

### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

function(SET_DEFAULT_PROPERTIES TARGET)
  target_include_directories(
    ${TARGET}
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  )
  target_compile_features(${TARGET} PUBLIC cxx_std_20)
  set_target_properties(${TARGET} PROPERTIES CXX_EXTENSIONS OFF)
endfunction()

add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
