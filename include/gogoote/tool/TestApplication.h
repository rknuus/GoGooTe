#pragma once

#include <map>
#include <memory>
#include <string>
#include "gogoote/model/ITestApplication.h"
#include "gogoote/model/TestSuite.h"

namespace gogoote {
namespace tool {

class TestApplication : public model::ITestApplication {
public:
  void add(const std::string& filename, std::unique_ptr<model::TestSuite> item) override;
  void generate() const;

private:
  std::map<std::string, std::unique_ptr<model::TestSuite>> items_;
};

}  // namespace tool
}  // namespace gogoote
