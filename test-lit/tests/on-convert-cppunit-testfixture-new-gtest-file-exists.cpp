/**
; RUN: cd %CURRENT_DIR
; RUN: %GOGOOTE_EXEC %s --
; RUN: %FILE_EXISTS_EXEC %s.gtest.cpp
**/

namespace CppUnit {
class TestFixture {};
}

class ComplexNumberTest : public CppUnit::TestFixture {
};
