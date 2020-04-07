#pragma once

#include <fstream>
#include <string>

namespace gogoote {

class File
{
public:
  explicit File(const std::string& path);
  void append(const std::string& content);

private:
  std::ofstream file_;
};

}  // namespace gogoote
