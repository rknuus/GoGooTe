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
  // TODO(KNR): rule of six
};

// std::ostream& operator<<(std::ostream& os, const TestFixture& value);
// std::string to_string(const TestFixture &value);

}  // namespace model
}  // namespace gogoote
