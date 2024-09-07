#pragma once

#include <clang/AST/ASTConsumer.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/Refactoring.h>
#include <llvm/ADT/StringRef.h>
#include "gogoote/cppunit/TestCaseFinder.h"
#include "gogoote/tool/TestApplication.h"

// test fixture collector
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <string>
#include <vector>

// tool
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <iostream>
#include <map>


namespace {

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
      // We want to exclude CppUnit framework classes driving from a text fixture. Using namespace
      // CppUnit only works if CPPUNIT_NO_NAMESPACE is not set. That's why we check the file path
      // instead.
      // TODO(RAKN): check for the include directory from the compilation database to ensure false
      // positives.
      if (not locatedInPath(*decl, "/include/cppunit/", *result.SourceManager)) {
        testFixtures.push_back(decl->getNameAsString());
      }
    }
  }

  const std::vector<std::string>& getTestFixtures() const {
    return testFixtures;
  }

private:
  template <typename TDecl>
  bool locatedInPath(const TDecl &declaration, const std::string &partialPath, const clang::SourceManager &sourceManager) {
    using namespace clang;
    const SourceLocation loc = declaration.getLocation();
    const PresumedLoc presumedLoc = sourceManager.getPresumedLoc(loc);
    const std::string fileName = presumedLoc.getFilename();
    return fileName.find(partialPath) != std::string::npos;
  }

  std::vector<std::string> testFixtures;
};

}  // namespace


namespace gogoote::tool {

class Tool {
public:
  Tool(clang::CompilerInstance* ci, clang::ast_matchers::MatchFinder* finder)
    : collector(finder) {}

  void postProcessing(std::map<std::string, clang::tooling::Replacements>&) {
    const auto& testFixtures = collector.getTestFixtures();
    for (const auto& fixture : testFixtures) {
      std::cout << "Found test fixture class: " << fixture << std::endl;
    }
  }

private:
  TestFixtureCollector collector;
};

}  // namespace gogoote::tool
