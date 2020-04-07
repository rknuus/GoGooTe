#include "gogoote/File.h"

namespace gogoote {

File::File(const std::string& path) : file_(path, std::ios::ate) {}

void File::append(const std::string& content) {
  file_ << content;
}

}  // namespace gogoote
