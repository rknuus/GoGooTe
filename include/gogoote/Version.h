#pragma once

namespace llvm {
class raw_ostream;
}

namespace gogoote {

const char *gogooteVersionString();
const char *gogooteCommitString();
const char *gogooteDescriptionString();
const char *gogooteHomepageString();
const char *gogooteBuildDateString();

const char *llvmVersionString();

void printVersionInformation();
void printVersionInformationStream(llvm::raw_ostream &out);

} // namespace gogoote
