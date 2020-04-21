#pragma once

#include <fstream>
#include <string>

namespace gogoote {
namespace tool {

class FileWriter
{
public:
  explicit FileWriter(const std::string& path);
  void append(const std::string& content);

private:
  std::ofstream file_;
};

}  // namespace tool
}  // namespace gogoote
