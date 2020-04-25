#include "gogoote/model/TestFixture.h"
#include <sstream>


namespace gogoote {
namespace model {

TestFixture::TestFixture(const std::string& name) : TestSuite(name) {}


// std::ostream& operator<<(std::ostream& os, const TestFixture& value) {
//   os << "#include <gtest/gtest.h>\n\n";
//   // for (const auto& test_case : value.test_cases_) {
//   //   os << "TEST(" << value.name_ << ", " << test_case;
//   // }
//   os << "TEST(" << value.name_ << ", " << value.test_case_;
//   return os;
// }

// std::string to_string(const TestFixture &value) {
//   std::ostringstream ss;
//   ss << value;
//   return ss.str();
// }

}  // namespace model
}  // namespace gogoote
