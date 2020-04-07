#include "gogoote/model/TestSuite.h"
#include <sstream>

namespace gogoote {
namespace model {

TestSuite::TestSuite(const std::string& name) : name_(name) {}

std::ostream& operator<<(std::ostream& os, const TestSuite& value) {
  os << "#include <gtest/gtest.h>\n\n";
  os << "TEST(" << value.name_ << ", Foo) {\n" << "}\n";
  return os;
}

std::string to_string(const TestSuite &value) {
  std::ostringstream ss;
  ss << value;
  return ss.str();
}

}  // namespace model
}  // namespace gogoote
