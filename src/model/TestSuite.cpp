#include "gogoote/model/TestSuite.h"
#include <sstream>


namespace gogoote {
namespace model {

TestSuite::TestSuite(const std::string& name) : name_(name) {}

void TestSuite::addTestCase(const TestCase& test_case) {
  // test_cases_.push_back(test_case);
  test_case_ = test_case;
}

std::string TestSuite::getName() const {
  return name_;
}

TestCase *const TestSuite::getTestCase() {
  return &test_case_;
}

std::string TestSuite::toString() const {
  std::ostringstream ss;
  ss << *this;
  return ss.str();
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
