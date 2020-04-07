#include "gogoote/Files.h"
#include <algorithm>
#include <string>
#include <utility>
#include "gogoote/File.h"

namespace gogoote {

void Files::add(const std::string& filename, const model::TestSuite& item) {
  items_.push_back(std::make_pair(filename, item));
}

void Files::generate() const {
  const auto gen = [](const std::pair<std::string, model::TestSuite>& p) {
    File f{p.first}; f.append(model::to_string(p.second));
  };
  std::for_each(std::cbegin(items_), std::cend(items_), gen);
}

}  // namespace gogoote
