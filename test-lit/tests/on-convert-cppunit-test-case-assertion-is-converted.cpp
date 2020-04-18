/**
; RUN: cd %CURRENT_DIR
; RUN: %GOGOOTE_EXEC %s --
; RUN: %FILECHECK_EXEC %s --input-file=%s.gtest.cpp
; CHECK:TEST(ComplexNumberTest, runTest) {
; CHECK:  ASSERT_TRUE( Complex (10, 1) == Complex (10, 1) );
; CHECK:  ASSERT_TRUE( !(Complex (1, 1) == Complex (2, 2)) );
**/

namespace CppUnit {
class TestCase {};

class Message {};

class SourceLine {};

struct Asserter
{
  static void failIf( bool shouldFail, 
                                  const Message &message, 
                                  const SourceLine &sourceLine = SourceLine() );
};
}

#define CPPUNIT_ASSERT(condition)                                            \
  ( CppUnit::Asserter::failIf( !(condition), CppUnit::Message(), CppUnit::SourceLine() ) )


class Complex {
public:
  Complex(const int re, const int im) {};
};

bool operator==(const Complex& lhs, const Complex& rhs) { return true; }

class ComplexNumberTest : public CppUnit::TestCase {
public:
  void runTest() {
    CPPUNIT_ASSERT( Complex (10, 1) == Complex (10, 1) );
    CPPUNIT_ASSERT( !(Complex (1, 1) == Complex (2, 2)) );
  }
};
