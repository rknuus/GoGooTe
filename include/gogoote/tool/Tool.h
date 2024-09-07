#pragma once

#include <clang/AST/ASTConsumer.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Refactoring.h>
// #include <clangmetatool/collectors/references.h>
#include <llvm/ADT/StringRef.h>
#include "gogoote/cppunit/TestCaseFinder.h"
#include "gogoote/tool/TestApplication.h"

// test fixture collector
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <string>
#include <vector>

// tool
// #include <clangmetatool/collectors/include_graph.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
// #include "TestFixtureCollector.h"
#include <iostream>
#include <map>


class TestFixtureCollector : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
  TestFixtureCollector(clang::ast_matchers::MatchFinder* finder) {
    finder->addMatcher(
      clang::ast_matchers::cxxRecordDecl(
        clang::ast_matchers::isDerivedFrom("CppUnit::TestFixture")
      ).bind("testFixture"),
      this
    );
  }

  // Called when a match is found
  void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override {
    if (const auto* decl = result.Nodes.getNodeAs<clang::CXXRecordDecl>("testFixture")) {
      testFixtures.push_back(decl->getNameAsString());
    }
  }

  const std::vector<std::string>& getTestFixtures() const {
    return testFixtures;
  }

private:
  std::vector<std::string> testFixtures;
};


namespace gogoote {
namespace tool {

class Tool {
public:
  Tool(clang::CompilerInstance* ci, clang::ast_matchers::MatchFinder* finder)
    : collector(finder) {}

  void postProcessing(std::map<std::string, clang::tooling::Replacements>&) {
    const auto& testFixtures = collector.getTestFixtures();
    // FIXME(RAKN): limit finds to the file passed as argument
    for (const auto& fixture : testFixtures) {
      std::cout << "Found test fixture class: " << fixture << std::endl;
    }
  }

private:
  TestFixtureCollector collector;

// public:
//   std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI,
//                                                         llvm::StringRef file) override;
//   void ExecuteAction() override;

// private:
//   TestApplication files_;
//   // clang::ast_matchers::MatchFinder finder_;
//   cppunit::TestCaseFinder test_case_finder_;
};

}  // namespace tool
}  // namespace gogoote
