#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include "gogoote/Tool.h"


using namespace clang::tooling;
using namespace llvm;


// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static cl::OptionCategory GoGooTeCategory("GoGooTe options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\n%%TODO(KNR)%%...\n");


int main(int argc, const char **argv) {
  CommonOptionsParser option_parser(argc, argv, GoGooTeCategory);
  ClangTool tool(option_parser.getCompilations(), option_parser.getSourcePathList());
  return tool.run(newFrontendActionFactory<gogoote::Tool>().get());
}
