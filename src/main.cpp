#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clangmetatool/meta_tool_factory.h>
#include <clangmetatool/meta_tool.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>
#include "gogoote/tool/Tool.h"
#include "gogoote/Version.h"


using namespace clang::tooling;
using namespace llvm;


static cl::OptionCategory gogoote_category("GoGooTe options");


int main(int argc, const char **argv) {
  cl::SetVersionPrinter(gogoote::printVersionInformationStream);
  Expected<CommonOptionsParser> option_parser = CommonOptionsParser::create(argc, argv, gogoote_category);
  if (not option_parser) {
    handleAllErrors(option_parser.takeError(), [&](const llvm::ErrorInfoBase &error_info) {
        errs() << "Error: " << error_info.message() << "\n";
    });
    cl::PrintHelpMessage();
    return 1;
  }

  clang::tooling::RefactoringTool tool(option_parser->getCompilations(),
                                       option_parser->getSourcePathList());
  clangmetatool::MetaToolFactory< clangmetatool::MetaTool<gogoote::tool::Tool> >
    factory(tool.getReplacements());
  return tool.runAndSave(&factory);
}
