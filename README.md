# GoGooTe
CppUnit to GoogleTest converter

## Build GoGooTe
```
git clone https://github.com/rknuus/GoGooTe.git --recursive
cd GoGooTe
mkdir build.dir
cd build.dir
cmake -DPATH_TO_LLVM=path/to/llvm -DPATH_TO_LIT=path/to/llvm-lit ..
make
```

## Design considerations
Large scale refactorings are best done in many incremental steps. Reasons are explained in Titus Winter's CppCon 2019 talk [From Functions to Concepts: Refactoring Impact of Higher-Level Design Features](https://www.youtube.com/watch?v=v_yzLe-wnfk). Gradually introducing GoogleTest doesn't make sense, at least not automatically. But keeping the old tests until the new ones are confirmed to work does make sense.

That's why this tool scans the given compilation database for CppUnit tests and generates new CPP files with corresponding GoogleTests. The tool's approach resembles the one of [bloomberg's clangmetatool](https://bloomberg.github.io/clangmetatool/) (even though that tool is intended for small refactorings): The tool populates a model, which is then used to generate code in a separate step.

For the sake of simplicity this tool focuses on test code generation. It doesn't generate a test runner, adjust your build definitions, nor removes CppUnit files. So those steps must be done manually.