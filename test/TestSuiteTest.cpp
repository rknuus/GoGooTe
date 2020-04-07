#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "gogoote/model/TestSuite.h"

using ::testing::StrEq;

namespace {

TEST(TestSuite, EmptySuite_GenerateOutput) {
  gogoote::model::TestSuite suite{"Foo"};
  const auto actual{to_string(suite)};
  ASSERT_THAT(actual, StrEq("#include <gtest/gtest.h>\n\nTEST(Foo, Foo) {\n}\n"));
}

}  // namespace
