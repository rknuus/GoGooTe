#include "gogoote/model/TestSuite.h"
#include <sstream>


namespace gogoote {
namespace model {

TestSuite::TestSuite(const std::string& name) : name_(name) {}

void TestSuite::add_test_case(const TestCase& test_case) {
  // test_cases_.push_back(test_case);
  test_case_ = test_case;
}

bool TestSuite::is_called(const std::string& name) const {
  return name_ == name;
}

std::string TestSuite::get_name() const {
  return name_;
}

TestCase *const TestSuite::get_test_case() {
  return &test_case_;
}


std::ostream& operator<<(std::ostream& os, const TestSuite& value) {
  os << "#include <gtest/gtest.h>\n\n";
  // for (const auto& test_case : value.test_cases_) {
  //   os << "TEST(" << value.name_ << ", " << test_case;
  // }
  os << "TEST(" << value.name_ << ", " << value.test_case_;
  return os;
}

std::string to_string(const TestSuite &value) {
  std::ostringstream ss;
  ss << value;
  return ss.str();
}

}  // namespace model
}  // namespace gogoote
