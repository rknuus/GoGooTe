/**
; RUN: cd %CURRENT_DIR
; RUN: %GOGOOTE_EXEC %s --
; RUN: %FILECHECK_EXEC %s --input-file=%s.gtest.cpp
; CHECK:TEST(ComplexNumberTest, runTest) {
**/

// as alternative to FileCheck:
// # Arguments are:
// #  2. CMAKE_CURRENT_SOURCE_DIR
// #  3. CMAKE_CURRENT_BINARY_DIR
// #  4. the name of the test case
// diff -u $2/$4.c.expected $3/$4.c


namespace CppUnit {
class TestCase {};
}

class ComplexNumberTest : public CppUnit::TestCase {
public:
  void runTest() {
  }
};
