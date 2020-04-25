/**
; RUN: cd %CURRENT_DIR
; RUN: %GOGOOTE_EXEC %s --
; RUN: %FILECHECK_EXEC %s --input-file=%s.gtest.cpp
; CHECK:class ComplexNumberTest : public ::testing::Test {
; CHECK:public:
; CHECK:  void SetUp() {
; CHECK:  }
; CHECK:  void TearDown() {
; CHECK:  }
**/

namespace CppUnit {
class TestFixture {};
}

class ComplexNumberTest : public CppUnit::TestFixture {
public:
  void setUp() {}
  void tearDown() {}
};
