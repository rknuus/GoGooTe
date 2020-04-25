set (SOURCES
  cppunit/TestCaseFinder.cpp
  model/Assertion.cpp
  model/TestCase.cpp
  model/TestFixture.cpp
  model/TestSuite.cpp
  tool/FileWriter.cpp
  tool/TestApplication.cpp
  tool/Tool.cpp
)

if ("LLVM" IN_LIST LLVM_AVAILABLE_LIBS)
  set (GOGOOTE_CXX_LLVM_LIBRARIES
    LLVM
  )
else()
  set (GOGOOTE_CXX_LLVM_LIBRARIES
    LLVMObject
  )
endif()

add_gogoote_executable(
  NAME gogoote
  MAIN main.cpp
  SOURCES ${SOURCES}
  LINK_WITH
    clangTooling
    clangBasic
    clangASTMatchers
    ${GOGOOTE_CXX_LLVM_LIBRARIES}
)