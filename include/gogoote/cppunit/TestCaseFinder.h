#pragma once

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include "gogoote/model/ITestApplication.h"


namespace gogoote {

namespace model {
class TestSuite;
}  // namespace model

namespace cppunit {

class TestCaseFinder : public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
  void addMatchers(clang::ast_matchers::MatchFinder& finder, model::ITestApplication* files);
  void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  model::ITestApplication* files_{nullptr};  // TODO(KNR): rename to test_application_ or translation_units_?
  model::TestSuite* current_test_suite_{nullptr};
};

}  // namespace cppunit
}  // namespace gogoote
