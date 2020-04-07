#include "gogoote/Version.h"

#include <llvm/Support/raw_ostream.h>

namespace gogoote {

const char *gogooteVersionString() { return "@PROJECT_VERSION@"; }
const char *gogooteCommitString() { return "@GIT_COMMIT@"; }
const char *gogooteBuildDateString() { return "@BUILD_DATE@"; }
const char *gogooteDescriptionString() { return "@PROJECT_DESCRIPTION@"; }
const char *gogooteHomepageString() { return "@PROJECT_HOMEPAGE_URL@"; }

const char *llvmVersionString() { return "@LLVM_VERSION@"; }

void printVersionInformation() { printVersionInformationStream(llvm::errs()); }

void printVersionInformationStream(llvm::raw_ostream &out) {
  out << "GoGooTe: " << gogooteDescriptionString() << "\n";
  out << gogooteHomepageString() << "\n";
  out << "Version: " << gogooteVersionString() << "\n";
  out << "Commit: " << gogooteCommitString() << "\n";
  out << "Date: " << gogooteBuildDateString() << "\n";
  out << "LLVM: " << llvmVersionString() << "\n";
}

} // namespace gogoote
