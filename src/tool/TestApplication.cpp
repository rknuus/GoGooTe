#include "gogoote/tool/TestApplication.h"
#include <algorithm>
#include <string>
#include <utility>
#include "gogoote/tool/FileWriter.h"

namespace gogoote {
namespace tool {

void TestApplication::add(const std::string& filename, const model::TestSuite& item) {
  items_.push_back(std::make_pair(filename, item));
}

model::TestSuite * const TestApplication::get(const std::string& name) {
  const auto match_name = [name](const std::pair<std::string, model::TestSuite>& item) {
    return item.second.getName() == name;
  };
  auto item = std::find_if(std::begin(items_), std::end(items_), match_name);
  if (item == std::end(items_)) {
    return nullptr;
  }
  return &item->second;
}

void TestApplication::generate() const {
  const auto gen = [](const std::pair<std::string, model::TestSuite>& p) {
    FileWriter f{p.first}; f.append(model::to_string(p.second));
  };
  std::for_each(std::cbegin(items_), std::cend(items_), gen);
}

}  // namespace tool
}  // namespace gogoote
