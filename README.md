# GoGooTe
CppUnit to GoogleTest converter.

## Purpose
Currently this project serves to explore technologies (e.g. libclang or later CI), techniques (e.g. large scale automated refactorings), and to have fun.

Currently the tool supports only the most basic CppUnit elements presented in the [cookbook](http://cppunit.sourceforge.net/doc/cvs/cppunit_cookbook.html): a `TestCase` implementing test method `runTest` containing just `ASSERT_TRUE` statements (i.e. no non-assertion code).

See section Outlook, below to learn what might come in the future.

## Build GoGooTe
```
git clone https://github.com/rknuus/GoGooTe.git --recursive
cd GoGooTe
mkdir build.dir
cd build.dir
cmake -DPATH_TO_LLVM=path/to/llvm -DPATH_TO_LIT=path/to/llvm-lit ..
make
```

## Compatibility
Initially, GoGooTe will only support a single version of libclang/LLVM, CppUnit, and GoogleTest. Later, the set of supported versions might be gradually expanded. See section Outlook for more information.

GoGooTe can be built with the following libclang/LLVM version:

| LLVM version  | supported       |
| ------------- |:---------------:|
| >10.x         | no (not tested) |
| 10.x          | yes             |
| <10.x         | no (not tested) |

GoGooTe can convert CppUnit versions to GoogleTest versions as follows:

| from CppUnit version \ to GoogleTest version | 1.10.x |
| -------------------------------------------- |:------:|
| 1.11.x                                       | yes    |

## Design considerations
Large scale refactorings are best done in many incremental steps. Reasons are explained in Titus Winter's CppCon 2019 talk [From Functions to Concepts: Refactoring Impact of Higher-Level Design Features](https://www.youtube.com/watch?v=v_yzLe-wnfk). Gradually introducing GoogleTest doesn't make sense, at least not automatically. But keeping the old tests until the new ones are confirmed to work does make sense.

That's why this tool scans the given compilation database for CppUnit tests and generates new CPP files with corresponding GoogleTests. The tool's approach resembles the one of [bloomberg's clangmetatool](https://bloomberg.github.io/clangmetatool/) (even though that tool is intended for small refactorings): The tool populates a model, which is then used to generate code in a separate step.

For the sake of simplicity this tool focuses on test code generation. It doesn't generate a test runner, adjust your build definitions, nor removes CppUnit files. So those steps must be done manually.

## Outlook
The plan is to gradually support:
- all CppUnit assertions
- non-assertion code in the test method
- `TestSuite` with `setUp` and `tearDown` methods and any number of test methods
- non-test-method code (e.g. member data and other member methods) in a `TestSuite`
- all remaining CppUnit features

Additional ideas, which might never make it into the tool:
- support GoogleTest to CppUnit conversion
- support conversion from/to Catch2, Catch1, ...
- support older versions of CppUnit
- support older versions of GoogleTest
- support building GoGooTe with older versions of libclang/LLVM

Some ideas for experiments are:
- try various patterns described in Martin Fowler's book "Domain Specific Languages" to populate the model, e.g.:
  - symbol table to collect all CppUnit items and look them up (e.g. using USR as key)
  - pure form of context variables
  - construction builder to first collect all data before creating an immutable element from those data
  - parser combinator: not sure
  - tree construction
  - embedded translation: not sure
  - state machine should work, but might be a bit over the top
- try a few patterns described in Martin Fowler's book "Domain Specific Languages" to generate output:
  - transformer generation
  - model aware generation

Contributions are highly appreciated!