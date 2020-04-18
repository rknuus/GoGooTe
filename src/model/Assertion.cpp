#include "gogoote/model/Assertion.h"
#include <sstream>


namespace gogoote {
namespace model {

Assertion::Assertion(const std::string& condition) : condition_(condition) {}


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
