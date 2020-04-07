#pragma once

#include <string>
#include <ostream>
#include "gogoote/model/ModelItem.h"


namespace gogoote {
namespace model {

// NOTE(KNR): Following nomenclature of International Software Testing Qualifications Board (ISTQB)
// in alignment with GTest's future direction (see [1]) we define a TestSuite as a set of
// TestCases, even though this doesn't match CppUnit's naming.
//
// CppUnit     | Model       | GTest
// ---------------------------------------------------------------------------------------------------------
// TestSuite   | TestSuite   | TestSuite (TestCase is deprecated, see [1])
// TestFixture | TestFixture | TestFixture (TEST_F)
// TestCase    | TestCase    | Test (TEST)
//
// [1]: https://github.com/google/googletest/blob/master/googletest/docs/primer.md#beware-of-the-nomenclature
class TestSuite : public ModelItem
{
public:
  explicit TestSuite(const std::string& name);
  // TODO(KNR): rule of six

private:
  std::string name_;

  friend std::ostream& operator<<(std::ostream& os, const TestSuite& value);
};

std::ostream& operator<<(std::ostream& os, const TestSuite& value);
std::string to_string(const TestSuite &value);

}  // namespace model
}  // namespace gogoote
