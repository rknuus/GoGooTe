cmake_minimum_required(VERSION 3.20.0)  # in sync with LLVM from 07-Sept-2024

if(NOT GOGOOTE_VERSION)
  set(GOGOOTE_VERSION 0.0.1)
endif()

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

if (APPLE)
  find_brew_prefix(BREW_PREFIX)
  if (NOT BREW_PREFIX STREQUAL "")
    list(APPEND CMAKE_PREFIX_PATH "${BREW_PREFIX}/opt/llvm/lib/cmake")
  endif()
endif()

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)
if (NOT LLVM_VERSION_MAJOR GREATER ${SUPPORTED_LLVM_MAJOR_VERSION} AND
    NOT LLVM_VERSION_MAJOR EQUAL ${SUPPORTED_LLVM_MAJOR_VERSION})
  message(FATAL_ERROR "LLVM version must be at least ${SUPPORTED_LLVM_MAJOR_VERSION}.0.0. Found ${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}.${LLVM_VERSION_PATCH}")
endif()

find_program(LIT_EXECUTABLE NAMES lit)
if (NOT LIT_EXECUTABLE)
  message(FATAL_ERROR "Could not find 'lit' program. Please install lit using 'brew' or 'pip', or provide the LIT_PATH manually.")
endif()
message(STATUS "Using lit program: ${LIT_EXECUTABLE}")

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
configure_gogoote_version(
  ${GOGOOTE_INCLUDE_DIRS}
  ${THIRD_PARTY_INCLUDE_DIRS}
  ${GOGOOTE_LINK_FLAGS}
  ${GOGOOTE_CXX_FLAGS}
)

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

link_directories(/usr/local/lib)  # Needed by FreeBSD

# # https://cmake.org/Wiki/CMake_RPATH_handling#Always_full_RPATH
# SET(CMAKE_SKIP_BUILD_RPATH FALSE)
# SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
# SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" "${LLVM_LIB_PATH}")
# SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# FIXME(RAKN): check whether clangmetatool supports Windows. If not: drop support in GoGooTe for Windows, because that's pointless.
# FIXME(RAKN): clangmetatool was probably not yet migrated to version 18 of LLVM and generates compiler warnings like:
# build/_deps/clangmetatool-src/src/tool_application_support.cpp:121:41: warning: 'makeArrayRef<const char *>' is deprecated: Use deduction guide instead [-Wdeprecated-declarations]
#          driver.BuildCompilation(llvm::makeArrayRef(argsArray)));
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
  clangmetatool
  GIT_REPOSITORY https://github.com/bloomberg/clangmetatool.git
  GIT_TAG        0.21.7
)
FetchContent_MakeAvailable(clangmetatool)

# TODO(RAKN): fix upstream
# FIXME(RAKN): patch CMakeLists.txt of clangmetatool/t/ to use the _CURRENT_ variant of CMAKE_SOURCE_DIR and CMAKE_BINARY_DIR

set (SOURCES
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