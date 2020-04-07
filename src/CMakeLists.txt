set (SOURCES
  cppunit/TestCaseFinder.cpp
  model/TestSuite.cpp
  File.cpp
  Files.cpp
  Tool.cpp
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
  MAIN ${CMAKE_CURRENT_LIST_DIR}/main.cpp
  SOURCES ${SOURCES}
  LINK_WITH
    clangTooling
    clangBasic
    clangASTMatchers
    ${GOGOOTE_CXX_LLVM_LIBRARIES}
)