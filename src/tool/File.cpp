#include "gogoote/tool/File.h"

namespace gogoote {
namespace tool {

File::File(const std::string& path) : file_(path, std::ios::ate) {}

void File::append(const std::string& content) {
  file_ << content;
}

}  // namespace tool
}  // namespace gogoote
