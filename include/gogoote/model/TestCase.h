#pragma once

#include <ostream>
#include <string>
#include <vector>
#include "gogoote/model/Assertion.h"


namespace gogoote {
namespace model {

class TestCase {
public:
  explicit TestCase(const std::string& name);
  // TODO(KNR): rule of six

  void addAssertion(const Assertion& assertion);

private:
  std::string name_;
  std::vector<Assertion> assertions_;

  friend std::ostream& operator<<(std::ostream& os, const TestCase& value);
};

std::ostream& operator<<(std::ostream& os, const TestCase& value);
std::string to_string(const TestCase &value);

}  // namespace model
}  // namespace gogoote
