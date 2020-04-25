#include "gogoote/model/TestFixture.h"
#include <sstream>


namespace gogoote {
namespace model {

TestFixture::TestFixture(const std::string& name) : TestSuite(name) {}


std::ostream& operator<<(std::ostream& os, const TestFixture& value) {
  os << "#include <gtest/gtest.h>\n\n";
  os << "class " << value.getName() << " : public ::testing::Test {\n";
  os << "};\n";
  return os;
}

std::string to_string(const TestFixture &value) {
  std::ostringstream ss;
  ss << value;
  return ss.str();
}

}  // namespace model
}  // namespace gogoote
