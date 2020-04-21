#pragma once

#include <clang/AST/ASTConsumer.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <llvm/ADT/StringRef.h>
#include "gogoote/cppunit/TestCaseFinder.h"
#include "gogoote/tool/TestApplication.h"


namespace gogoote {
namespace tool {

class Tool : public clang::ASTFrontendAction {
public:
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI,
                                                        llvm::StringRef file) override;
  void ExecuteAction() override;

private:
  TestApplication files_;
  clang::ast_matchers::MatchFinder finder_;
  cppunit::TestCaseFinder test_case_finder_;
};

}  // namespace tool
}  // namespace gogoote
