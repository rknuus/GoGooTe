#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "gogoote/model/Assertion.h"
#include "gogoote/model/TestCase.h"
#include "gogoote/model/TestSuite.h"

using ::testing::HasSubstr;
using ::testing::StrEq;

namespace {

TEST(TestSuite, EmptySuite_GenerateInclude) {
  const gogoote::model::TestSuite suite{"Foo"};
  const auto actual{to_string(suite)};
  ASSERT_THAT(actual, HasSubstr("#include <gtest/gtest.h>"));
}

TEST(TestSuite, CppUnitTestCase_GenerateGTestTest) {
  gogoote::model::TestSuite suite{"Foo"};
  const gogoote::model::TestCase test_case{"runTest"};
  suite.addTestCase(test_case);
  const auto actual{to_string(suite)};
  ASSERT_THAT(actual, HasSubstr("TEST(Foo, runTest) {\n}\n"));
}

TEST(TestSuite, CppUnitBasicAssertion_GenerateGTestAssertion) {
  const gogoote::model::Assertion assertion{"true"};
  gogoote::model::TestCase test_case{"runTest"};
  test_case.addAssertion(assertion);
  gogoote::model::TestSuite suite{"Foo"};
  suite.addTestCase(test_case);

  const auto actual{to_string(suite)};
  ASSERT_THAT(actual, HasSubstr("TEST(Foo, runTest) {\nASSERT_TRUE(true);\n}\n"));
}

}  // namespace
