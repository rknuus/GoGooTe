#include "gogoote/model/TestCase.h"
#include <sstream>


namespace gogoote {
namespace model {

TestCase::TestCase(const std::string& name) : name_(name) {}

void TestCase::add_assertion(const Assertion& assertion) {
  assertions_.push_back(assertion);
}


std::ostream& operator<<(std::ostream& os, const TestCase& value) {
  os << value.name_ << ") {\n";
  for (const auto& assertion : value.assertions_) {
    os << assertion << "\n";
  }
  os << "}\n";
  return os;
}

std::string to_string(const TestCase &value) {
  std::ostringstream ss;
  ss << value;
  return ss.str();
}

}  // namespace model
}  // namespace gogoote
