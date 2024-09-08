cmake_minimum_required(VERSION 3.20.0)  # in sync with LLVM from 07-Sept-2024

find_package(GTest REQUIRED)

set(GOGOOTE_UNIT_TEST_SOURCES
  # AssertionTest.cpp
  # TestSuiteTest.cpp
  Dummy.cpp  # TEMPORARILY(RAKN): until we get real tests
)

add_executable(gogoote-tests ${GOGOOTE_UNIT_TEST_SOURCES})
target_link_libraries(gogoote-tests
  gogoote-static
  ${GTEST_BOTH_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  LLVMSupport
)

add_test(unittest gogoote-tests)