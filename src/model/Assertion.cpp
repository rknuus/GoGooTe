#include "gogoote/model/Assertion.h"
#include <assert.h>
#include <regex>
#include <sstream>


namespace gogoote {
namespace model {


const std::regex cppunit_assertion_pattern{"^(CPPUNIT_ASSERT\\w*)\\s*[(](.*)[)]$"};


Assertion::Assertion(const std::string& assertion) {
  std::smatch match;
  const bool ok{std::regex_search(assertion, match, cppunit_assertion_pattern)};
  assert(ok && match.size() == 3);
  assertion_ = match[1];
  condition_ = match[2];
}


std::ostream& operator<<(std::ostream& os, const Assertion& value) {
  os << "ASSERT_TRUE(" << value.condition_ << ");";
  return os;
}

std::string to_string(const Assertion &value) {
  std::ostringstream ss;
  ss << value;
  return ss.str();
}

}  // namespace model
}  // namespace gogoote
