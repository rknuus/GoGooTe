#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "gogoote/model/TestSuite.h"

using ::testing::HasSubstr;
using ::testing::StrEq;

namespace {

TEST(TestSuite, EmptySuite_GenerateInclude) {
  gogoote::model::TestSuite suite{"Foo"};
  const auto actual{to_string(suite)};
  ASSERT_THAT(actual, HasSubstr("#include <gtest/gtest.h>"));
}

TEST(TestSuite, CppUnitTestCase_GenerateGTestTest) {
  gogoote::model::TestSuite suite{"Foo"};
  suite.add_test_case("runTest");  // TODO(KNR): replace string by proper object
  const auto actual{to_string(suite)};
  ASSERT_THAT(actual, HasSubstr("TEST(Foo, runTest) {\n}\n"));
}

}  // namespace
