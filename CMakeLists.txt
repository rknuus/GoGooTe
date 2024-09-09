cmake_minimum_required(VERSION 3.20.0)  # in sync with LLVM from 07-Sept-2024

if(NOT GOGOOTE_VERSION)
  set(GOGOOTE_VERSION 0.0.1)
endif()

set(PATH_TO_LLVM "" CACHE PATH "Path to custom LLVM installation (optional).")
set(PATH_TO_LIT "" CACHE PATH "Path to the 'lit' executable (optional).")

project(GoGooTe
  LANGUAGES C CXX
)

set(PROJECT_VERSION ${GOGOOTE_VERSION})
set(PROJECT_DESCRIPTION "CppUnit to GoogleTest converter")
set(PROJECT_HOMEPAGE_URL "https://github.com/rknuus/GoGooTe")

set(SUPPORTED_LLVM_MAJOR_VERSION 18)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

include(${CMAKE_CURRENT_LIST_DIR}/cmake/functions.cmake)

# Originally we also supported the llvm-project root directory as PATH_TO_LLVM, but clangmetatool
# can only cope with preinstalled LLVMs.
find_llvm_cmake_directories(SUPPORTED_LLVM_MAJOR_VERSION llvm_search_paths clang_search_paths)
find_package(LLVM REQUIRED CONFIG PATHS ${llvm_search_paths} NO_DEFAULT_PATH)
find_package(Clang REQUIRED CONFIG PATHS ${clang_search_paths} NO_DEFAULT_PATH)
# As LLVM and Clang are searched in the same directory, checking the version for LLVM is enough
if (NOT LLVM_VERSION_MAJOR GREATER ${SUPPORTED_LLVM_MAJOR_VERSION} AND
    NOT LLVM_VERSION_MAJOR EQUAL ${SUPPORTED_LLVM_MAJOR_VERSION})
  message(FATAL_ERROR "LLVM version must be at least ${SUPPORTED_LLVM_MAJOR_VERSION}.0.0. Found ${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}.${LLVM_VERSION_PATCH}")
endif()

setup_precompiled_llvm(GOGOOTE_CC)
message(STATUS "Building against precompiled LLVM using compiler ${GOGOOTE_CC}")

set(GOGOOTE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# TODO(KNR): figure out compiler flags
set (GOGOOTE_CXX_FLAGS "-fPIC -fvisibility-inlines-hidden -fno-exceptions -Wunused -Werror")
if (NOT LLVM_ENABLE_RTTI)
  set(GOGOOTE_CXX_FLAGS "${GOGOOTE_CXX_FLAGS} -fno-rtti")
endif()

set (GOGOOTE_LINK_FLAGS "-rdynamic")

set (GOGOOTE_DEFINITIONS ${LLVM_DEFINITIONS})
set (THIRD_PARTY_INCLUDE_DIRS
  ${LLVM_INCLUDE_DIRS}
)
set (GOGOOTE_INCLUDE_DIRS
  ${GOGOOTE_SOURCE_DIR}/include
)
# FIXME(RAKN): can only include version.cmake *after* GOGOOTE_INCLUDE_DIRS was defined
include(${CMAKE_CURRENT_LIST_DIR}/cmake/version.cmake)

if ("LLVM" IN_LIST LLVM_AVAILABLE_LIBS)
  set (GOGOOTE_LLVM_COMPATIBILITY_LIBRARIES
    LLVM
  )
else()
  set (GOGOOTE_LLVM_COMPATIBILITY_LIBRARIES
    LLVMSupport
    LLVMCore
  )
endif()

# Needed by FreeBSD
link_directories(/usr/local/lib)

# # https://cmake.org/Wiki/CMake_RPATH_handling#Always_full_RPATH
# SET(CMAKE_SKIP_BUILD_RPATH FALSE)
# SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
# SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" "${LLVM_LIB_PATH}")
# SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# FIXME(RAKN): print messages when downloading the repo
# FIXME(RAKN): clangmetatool was probably not yet migrated to version 18 of LLVM and generates compiler warnings like:
# build/_deps/clangmetatool-src/src/tool_application_support.cpp:121:41: warning: 'makeArrayRef<const char *>' is deprecated: Use deduction guide instead [-Wdeprecated-declarations]
#          driver.BuildCompilation(llvm::makeArrayRef(argsArray)));
include(FetchContent)
FetchContent_Declare(
  clangmetatool
  GIT_REPOSITORY https://github.com/bloomberg/clangmetatool.git
  GIT_TAG        0.21.7
)
FetchContent_MakeAvailable(clangmetatool)
# TODO(RAKN): fix upstream
# FIXME(RAKN): patch CMakeLists.txt of clangmetatool/t/ to use the _CURRENT_ variant of CMAKE_SOURCE_DIR and CMAKE_BINARY_DIR

set (SOURCES
  # src/cppunit/TestCaseFinder.cpp
  # src/model/Assertion.cpp
  # src/model/TestCase.cpp
  # src/model/TestSuite.cpp
  # src/tool/FileWriter.cpp
  # src/tool/TestApplication.cpp
  src/tool/Tool.cpp
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
  MAIN src/main.cpp
  SOURCES ${SOURCES}
  LINK_WITH
    clangmetatool
    clangTooling
    ${GOGOOTE_CXX_LLVM_LIBRARIES}
)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(test)
  add_subdirectory(test-lit)
endif() 