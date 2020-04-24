#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "gogoote/model/Assertion.h"


using ::gogoote::model::Assertion;
using ::testing::Eq;


namespace {

TEST(Assertion, CPPUNIT_ASSERT) {
  const gogoote::model::Assertion assertion{"CPPUNIT_ASSERT(true)"};
  const auto actual{to_string(assertion)};
  ASSERT_THAT(actual, Eq("ASSERT_TRUE(true);"));
}

TEST(Assertion, CPPUNIT_ASSERT_MESSAGE) {
  const gogoote::model::Assertion assertion{"CPPUNIT_ASSERT_MESSAGE(true, \"M\")"};
  const auto actual{to_string(assertion)};
  ASSERT_THAT(actual, Eq("ASSERT_TRUE(true) << \"M\";"));
}

TEST(Assertion, CPPUNIT_ASSERT_DOUBLES_EQUAL) {
  const Assertion assertion{"CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, 2.0, 3.0)"};
  const auto actual{to_string(assertion)};
  ASSERT_THAT(actual, Eq("ASSERT_NEAR(1.0, 2.0, 3.0);"));
}

TEST(Assertion, CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE) {
  const Assertion assertion{"CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(1.0, 2.0, 3.0, \"Foo\")"};
  const auto actual{to_string(assertion)};
  ASSERT_THAT(actual, Eq("ASSERT_NEAR(1.0, 2.0, 3.0) << \"Foo\";"));
}

TEST(Assertion, CPPUNIT_ASSERT_EQUAL) {
  const Assertion assertion{"CPPUNIT_ASSERT_EQUAL(1, 2)"};
  const auto actual{to_string(assertion)};
  ASSERT_THAT(actual, Eq("ASSERT_EQ(1, 2);"));
}

TEST(Assertion, CPPUNIT_FAIL) {
  const Assertion assertion{"CPPUNIT_FAIL(\"bar\")"};
  const auto actual{to_string(assertion)};
  ASSERT_THAT(actual, Eq("FAIL() << \"bar\";"));
}

// Test List:
// ----------
// - CPPUNIT_ASSERT_NO_THROW
// - CPPUNIT_ASSERT_NO_THROW_MESSAGE
// - CPPUNIT_ASSERT_THROW
// - CPPUNIT_ASSERT_THROW_MESSAGE
// - CPPUNIT_ASSERT_ASSERTION_FAIL
// - CPPUNIT_ASSERT_ASSERTION_FAIL_MESSAGE
// - CPPUNIT_ASSERT_ASSERTION_PASS
// - CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE

}  // namespace
