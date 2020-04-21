#include "gogoote/tool/Tool.h"

namespace gogoote {
namespace tool {

std::unique_ptr<clang::ASTConsumer> Tool::CreateASTConsumer(clang::CompilerInstance &CI,
                                                            llvm::StringRef file) {
  test_case_finder_.sign_up(finder_, &files_);

  return finder_.newASTConsumer();
}

void Tool::ExecuteAction() {
  ASTFrontendAction::ExecuteAction();
  files_.generate();
}

}  // namespace tool
}  // namespace gogoote
