#include "gogoote/tool/TestApplication.h"
#include <algorithm>
#include <string>
#include <utility>
#include "gogoote/tool/FileWriter.h"

namespace gogoote {
namespace tool {

void TestApplication::add(const std::string& filename, std::unique_ptr<model::TestSuite> item) {
  items_[filename] = std::move(item);
}

void TestApplication::generate() const {
  for (const auto& it : items_) {
    FileWriter f{it.first};
    f.append(it.second->toString());
  }
}

}  // namespace tool
}  // namespace gogoote
