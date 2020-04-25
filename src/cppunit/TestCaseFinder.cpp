#include "gogoote/cppunit/TestCaseFinder.h"
#include <assert.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>
#include <memory>
#include <string>
#include "gogoote/model/TestFixture.h"
#include "gogoote/model/TestSuite.h"


using namespace clang;
using namespace clang::ast_matchers;


namespace gogoote {
namespace cppunit {

std::string getSourceFileName(const MatchFinder::MatchResult &Result) {
  auto sm{Result.SourceManager};
  assert(sm != nullptr);
  auto file_id = sm->getMainFileID();
  const auto *file_entry = sm->getFileEntryForID(file_id);
  assert(file_entry != nullptr);  // TODO(KNR): might have to handle this case gracefully
  return file_entry->getName().str();
}

// shamelessly copied from clangmetatool (file src/source_util.cpp)
clang::CharSourceRange expandRange(const clang::SourceRange &range, const clang::SourceManager &sourceManager) {
  // Get the start location, resolving from macro definition to macro call
  // location. Special handling is needed for a statement in a macro body, we
  // want to resolve to the entire macro call.

  clang::SourceLocation begin = range.getBegin();
  if (sourceManager.isMacroBodyExpansion(begin)) {
    begin = sourceManager.getExpansionRange(begin).getBegin();
  } else {
    begin = sourceManager.getFileLoc(begin);
  }

  // Get the end location, resolving from macro definition to macro call
  // location. The end location of a statement points to the beginning of the
  // last token in it, so we must use the lexer to traverse the token too.

  clang::SourceLocation end = range.getEnd();
  if (sourceManager.isMacroBodyExpansion(end)) {
    end = sourceManager.getExpansionRange(end).getEnd();
  } else {
    end = sourceManager.getFileLoc(end);
  }
  end = clang::Lexer::getLocForEndOfToken(end, 0, sourceManager,
                                          clang::LangOptions());

  return clang::CharSourceRange::getCharRange(begin, end);
}

std::string extractConditionText(const UnaryOperator* condition_op_node, SourceManager &sm) {
  assert(condition_op_node != nullptr);

  const auto range = expandRange(condition_op_node->getSourceRange(), sm);
  const auto text = clang::Lexer::getSourceText(range, sm, clang::LangOptions()).str();
  return text;
}


void TestCaseFinder::addMatchers(clang::ast_matchers::MatchFinder& finder, model::ITestApplication* files) {
  assert(files != nullptr);
  assert(files_ == nullptr);
  files_ = files;

  // TODO(KNR): prevent matching similar classes of other namespaces (accept namespaces global and CppUnit)
  const auto test_fixture = cxxRecordDecl(isDerivedFrom(hasName("TestFixture")),
                                          isExpansionInMainFile(), hasDefinition()).bind("TestFixture");
  finder.addMatcher(test_fixture, this);

  const auto test_case = cxxRecordDecl(isDerivedFrom(hasName("TestCase")), isExpansionInMainFile(), hasDefinition())
                           .bind("TestCase");
  finder.addMatcher(test_case, this);

  const auto test_method = cxxMethodDecl(ofClass(test_case)).bind("TestMethod");
  finder.addMatcher(test_method, this);

  const auto assertion_method = cxxMethodDecl(ofClass(cxxRecordDecl(hasName("Asserter"))), hasName("failIf"));
  const auto assertion_call = callExpr(callee(assertion_method), hasArgument(0, unaryOperator().bind("Condition")));
  finder.addMatcher(assertion_call, this);
}

void TestCaseFinder::run(const MatchFinder::MatchResult &Result) {
  const CXXRecordDecl *test_fixture_node = Result.Nodes.getNodeAs<CXXRecordDecl>("TestFixture");
  const CXXRecordDecl *test_case_node = Result.Nodes.getNodeAs<CXXRecordDecl>("TestCase");
  const CXXMethodDecl *test_method_node = Result.Nodes.getNodeAs<CXXMethodDecl>("TestMethod");
  const UnaryOperator *condition_op_node = Result.Nodes.getNodeAs<UnaryOperator>("Condition");

  if (test_fixture_node && !test_method_node) {
    files_->add(getSourceFileName(Result) + ".gtest.cpp", model::TestFixture{test_fixture_node->getName()});
    // this is a form of context variable as described in Martin Fowler's book "Domain Specific Languages"
    current_test_suite_ = files_->get(test_fixture_node->getName());
  } else if (test_case_node && !test_method_node) {
    files_->add(getSourceFileName(Result) + ".gtest.cpp", model::TestSuite{test_case_node->getName()});
    // this is a form of context variable as described in Martin Fowler's book "Domain Specific Languages"
    current_test_suite_ = files_->get(test_case_node->getName());
  } else if (test_method_node && !condition_op_node) {
    assert(current_test_suite_ != nullptr);
    current_test_suite_->addTestCase(model::TestCase{test_method_node->getName()});
  } else if (condition_op_node) {
    assert(current_test_suite_ != nullptr);
    current_test_suite_->getTestCase()->addAssertion(model::Assertion{extractConditionText(condition_op_node, *Result.SourceManager)});
  }
}

}  // namespace cppunit
}  // namespace gogoote
