cmake_minimum_required(VERSION 3.14.0)

set(GOGOOTE_UNIT_TEST_SOURCES
  TestSuiteTest.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)
FetchContent_MakeAvailable(googletest)

add_executable(gogoote-tests ${GOGOOTE_UNIT_TEST_SOURCES})
target_link_libraries(gogoote-tests
  gogoote-static
  gmock
  gmock_main
  LLVMSupport
)

add_test(unittest gogoote-tests)