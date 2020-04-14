#include "gogoote/cppunit/TestCaseFinder.h"
#include <assert.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <memory>
#include <string>
#include "gogoote/model/TestSuite.h"


using namespace clang;
using namespace clang::ast_matchers;


namespace gogoote {
namespace cppunit {

std::string get_source_file(const MatchFinder::MatchResult &Result) {
  auto sm{Result.SourceManager};
  assert(sm != nullptr);
  auto file_id = sm->getMainFileID();
  const auto *file_entry = sm->getFileEntryForID(file_id);
  assert(file_entry != nullptr);  // TODO(KNR): might have to handle this case gracefully
  return file_entry->getName().str();
}


void TestCaseFinder::sign_up(clang::ast_matchers::MatchFinder& finder, Files* files) {
  assert(files != nullptr);
  assert(files_ == nullptr);
  const auto test_case = cxxRecordDecl(isDerivedFrom(hasName("TestCase")), isExpansionInMainFile(), hasDefinition())
                           .bind("TestCase");
  finder.addMatcher(test_case, this);
  const auto test_method = cxxMethodDecl(ofClass(cxxRecordDecl(isDerivedFrom(hasName("TestCase"))).bind("TestCase")))
                             .bind("TestMethod");
  finder.addMatcher(test_method, this);
  files_ = files;
}

void TestCaseFinder::run(const MatchFinder::MatchResult &Result) {
  const CXXRecordDecl *test_case = Result.Nodes.getNodeAs<CXXRecordDecl>("TestCase");
  const CXXMethodDecl *test_method = Result.Nodes.getNodeAs<CXXMethodDecl>("TestMethod");

  if (test_method && test_case) {
    // TODO(KNR): this is borked, look up builder pattern in loud-mouth's book
    auto * test_suite = files_->get(test_case->getName());
    if (test_suite == nullptr) {
      files_->add(get_source_file(Result) + ".gtest.cpp", model::TestSuite{test_case->getName()});
    }
    test_suite = files_->get(test_case->getName());
    assert(test_suite != nullptr);
    test_suite->add_test_case(test_method->getName());
  } else if (test_case) {
    auto * const test_suite = files_->get(test_case->getName());
    if (!test_suite) {
      files_->add(get_source_file(Result) + ".gtest.cpp", model::TestSuite{test_case->getName()});
    }
  }
}

}  // namespace cppunit
}  // namespace gogoote
