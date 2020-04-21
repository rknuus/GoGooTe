#include "gogoote/cppunit/TestCaseFinder.h"
#include <assert.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>
#include <memory>
#include <regex>
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


void TestCaseFinder::sign_up(clang::ast_matchers::MatchFinder& finder, tool::Files* files) {
  assert(files != nullptr);
  assert(files_ == nullptr);
  files_ = files;

  const auto test_case = cxxRecordDecl(isDerivedFrom(hasName("TestCase")), isExpansionInMainFile(), hasDefinition())
                           .bind("TestCase");
  finder.addMatcher(test_case, this);

  const auto test_method = cxxMethodDecl(ofClass(test_case)).bind("TestMethod");
  finder.addMatcher(test_method, this);

  const auto assertion_method = cxxMethodDecl(ofClass(cxxRecordDecl(hasName("Asserter"))), hasName("failIf"));
  const auto assertion_call = callExpr(callee(assertion_method), hasArgument(0, unaryOperator().bind("Condition")));
  finder.addMatcher(assertion_call, this);
}

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

std::string get_condition_code(const UnaryOperator* condition_op_node, SourceManager &sm) {
  assert(condition_op_node != nullptr);

  const auto range = expandRange(condition_op_node->getSourceRange(), sm);
  const auto text = clang::Lexer::getSourceText(range, sm, clang::LangOptions()).str();
  std::regex cppunit_assertion{"^CPPUNIT_ASSERT\\s*[(](.*)[)]$"};

  // return text;
  return std::regex_replace(text, cppunit_assertion, "$1");
}

void TestCaseFinder::run(const MatchFinder::MatchResult &Result) {
  const CXXRecordDecl *test_case_node = Result.Nodes.getNodeAs<CXXRecordDecl>("TestCase");
  const CXXMethodDecl *test_method_node = Result.Nodes.getNodeAs<CXXMethodDecl>("TestMethod");
  const UnaryOperator *condition_op_node = Result.Nodes.getNodeAs<UnaryOperator>("Condition");

  if (test_case_node && !test_method_node) {
    // TODO(KNR): this is borked, look up builder pattern in loud-mouth's book
    if (current_test_suite_ && test_case_node->getName() == current_test_suite_->get_name()) {
      return;
    }
    files_->add(get_source_file(Result) + ".gtest.cpp", model::TestSuite{test_case_node->getName()});
    current_test_suite_ = files_->get(test_case_node->getName());
  } else if (test_method_node && !condition_op_node) {
    assert(current_test_suite_ != nullptr);
    current_test_suite_->add_test_case(model::TestCase{test_method_node->getName()});
  } else if (condition_op_node) {
    assert(current_test_suite_ != nullptr);
    current_test_suite_->get_test_case()->add_assertion(model::Assertion{get_condition_code(condition_op_node, *Result.SourceManager)});
  }
}

}  // namespace cppunit
}  // namespace gogoote
