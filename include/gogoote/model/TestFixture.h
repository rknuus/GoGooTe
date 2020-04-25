#pragma once

#include <ostream>
#include <string>
#include <vector>
#include "gogoote/model/Assertion.h"
#include "gogoote/model/TestSuite.h"


namespace gogoote {
namespace model {

class TestFixture : public TestSuite {
public:
  explicit TestFixture(const std::string& name);
  virtual ~TestFixture() = default;
  TestFixture(const TestFixture&) = default;
  TestFixture& operator=(const TestFixture&) = default;
  TestFixture(TestFixture&&) = default;
  TestFixture& operator=(TestFixture&&) = default;

  std::string toString() const override;
};

std::ostream& operator<<(std::ostream& os, const TestFixture& value);
std::string to_string(const TestFixture &value);

}  // namespace model
}  // namespace gogoote
