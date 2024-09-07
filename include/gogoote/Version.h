#pragma once

namespace llvm {
class raw_ostream;
}

namespace gogoote {

void printVersionInformationStream(llvm::raw_ostream &out);

} // namespace gogoote
