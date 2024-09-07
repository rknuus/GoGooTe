#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clangmetatool/meta_tool_factory.h>
#include <clangmetatool/meta_tool.h>
#include "gogoote/tool/Tool.h"


using namespace clang::tooling;
using namespace llvm;


static cl::OptionCategory GoGooTeCategory("GoGooTe options");
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);


int main(int argc, const char **argv) {
  Expected<CommonOptionsParser> option_parser = CommonOptionsParser::create(argc, argv, GoGooTeCategory);
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
    raf(tool.getReplacements());
  return tool.runAndSave(&raf);
}
