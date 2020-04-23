#pragma once

#include <ostream>
#include <string>


namespace gogoote {
namespace model {

class Assertion {
public:
  explicit Assertion(const std::string& assertion);
  // TODO(KNR): rule of six

private:
  std::string assertion_;
  std::string condition_;

  friend std::ostream& operator<<(std::ostream& os, const Assertion& value);
};

std::ostream& operator<<(std::ostream& os, const Assertion& value);
std::string to_string(const Assertion &value);

}  // namespace model
}  // namespace gogoote
