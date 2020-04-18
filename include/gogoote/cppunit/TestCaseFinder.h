#pragma once

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include "gogoote/Files.h"


namespace gogoote {

namespace model {
class TestSuite;
}  // namespace model

namespace cppunit {

class TestCaseFinder : public clang::ast_matchers::MatchFinder::MatchCallback
{
public :
  void sign_up(clang::ast_matchers::MatchFinder& finder, Files* files);  // TODO(KNR): factor out interface
  void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  Files* files_{nullptr};
  model::TestSuite* current_test_suite_{nullptr};
};

}  // namespace cppunit
}  // namespace gogoote
