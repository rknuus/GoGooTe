#pragma once

#include <memory>
#include <string>
#include "gogoote/model/TestSuite.h"

namespace gogoote {
namespace model {

class ITestApplication {
public:
  virtual ~ITestApplication() = default;
  ITestApplication(const ITestApplication&) = default;
  ITestApplication& operator=(const ITestApplication&) = default;
  ITestApplication(ITestApplication&&) = default;
  ITestApplication& operator=(ITestApplication&&) = default;

  virtual void add(const std::string& filename, std::unique_ptr<model::TestSuite> item) = 0;

protected:
  ITestApplication() = default;
};

}  // namespace model
}  // namespace gogoote
