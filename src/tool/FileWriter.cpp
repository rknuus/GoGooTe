#include "gogoote/tool/FileWriter.h"

namespace gogoote {
namespace tool {

FileWriter::FileWriter(const std::string& path) : file_(path, std::ios::ate) {}

void FileWriter::append(const std::string& content) {
  file_ << content;
}

}  // namespace tool
}  // namespace gogoote
