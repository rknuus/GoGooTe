#pragma once

#include <memory>
#include <string>
#include <vector>
#include "gogoote/model/TestSuite.h"

namespace gogoote {

class Files {
public:
  void add(const std::string& filename, const model::TestSuite& item);
  void generate() const;

private:
  std::vector<std::pair<std::string, model::TestSuite>> items_;
};

}  // namespace gogoote
