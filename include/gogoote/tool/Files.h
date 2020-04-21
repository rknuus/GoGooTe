#pragma once

#include <memory>
#include <string>
#include <vector>
#include "gogoote/model/TestSuite.h"

namespace gogoote {
namespace tool {

class Files {
public:
  void add(const std::string& filename, const model::TestSuite& item);
  model::TestSuite * const get(const std::string& filename);
  void generate() const;

private:
  std::vector<std::pair<std::string, model::TestSuite>> items_;
};

}  // namespace tool
}  // namespace gogoote
