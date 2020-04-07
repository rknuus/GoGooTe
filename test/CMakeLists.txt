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
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_MakeAvailable(googletest)
endif()

add_executable(gogoote-tests ${GOGOOTE_UNIT_TEST_SOURCES})
target_link_libraries(gogoote-tests
  gogoote-static
  gmock
  gmock_main
  LLVMSupport
)
target_include_directories(gogoote-tests PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GOGOOTE_INCLUDE_DIRS}
)
target_include_directories(gogoote-tests SYSTEM PRIVATE
  ${gmock_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}/include
)

add_test(unittest gogoote-tests)