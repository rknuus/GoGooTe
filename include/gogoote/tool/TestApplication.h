#pragma once

#include <memory>
#include <string>
#include <vector>
#include "gogoote/model/ITestApplication.h"
#include "gogoote/model/TestSuite.h"

namespace gogoote {
namespace tool {

class TestApplication : public model::ITestApplication {
public:
  void add(const std::string& filename, const model::TestSuite& item) override;
  model::TestSuite * const get(const std::string& filename) override;
  void generate() const;

private:
  std::vector<std::pair<std::string, model::TestSuite>> items_;
};

}  // namespace tool
}  // namespace gogoote
