#include "gogoote/model/TestSuite.h"
#include <sstream>

namespace gogoote {
namespace model {

TestSuite::TestSuite(const std::string& name) : name_(name) {}

void TestSuite::add_test_case(const std::string& test_case_name) {
  test_cases_.push_back(test_case_name);
}

bool TestSuite::is_called(const std::string& name) const {
  return name_ == name;
}


std::ostream& operator<<(std::ostream& os, const TestSuite& value) {
  os << "#include <gtest/gtest.h>\n\n";
  for (const auto& test_case : value.test_cases_) {
    os << "TEST(" << value.name_ << ", " << test_case << ") {\n" << "}\n";
  }
  return os;
}

std::string to_string(const TestSuite &value) {
  std::ostringstream ss;
  ss << value;
  return ss.str();
}

}  // namespace model
}  // namespace gogoote
