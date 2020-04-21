#pragma once

#include <string>
#include "gogoote/model/TestSuite.h"

namespace gogoote {
namespace model {

class ITestApplication {
public:
  virtual ~ITestApplication() = default;  // TODO(KNR): rule of six
  virtual void add(const std::string& filename, const model::TestSuite& item) = 0;
  virtual model::TestSuite * const get(const std::string& filename) = 0;
};

}  // namespace model
}  // namespace gogoote
