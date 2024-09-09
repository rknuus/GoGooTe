function(find_brew_prefix brew_prefix_out_var)
  execute_process(COMMAND brew --version OUTPUT_QUIET ERROR_QUIET RESULT_VARIABLE result)
  if (result EQUAL 0)
    execute_process(COMMAND brew --prefix OUTPUT_VARIABLE PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(${brew_prefix_out_var} "${PREFIX}" PARENT_SCOPE)
  else()
    set(${brew_prefix_out_var} "" PARENT_SCOPE)
  endif()
endfunction()

function(set_llvm_target_search_paths target search_paths_out_var)
  if (DEFINED PATH_TO_LLVM)
    message(STATUS "Using user-provided path to LLVM: ${PATH_TO_LLVM}")
    set(search_paths "${PATH_TO_LLVM}/lib/cmake/${target}")
  elseif (WIN32)
    set(search_paths
      "C:/Program Files/LLVM/lib/cmake/${target}/"
      "C:/Program Files (x86)/LLVM/lib/cmake/${target}/"
    )
  elseif (UNIX OR APPLE)
    set(search_paths
      "/usr/lib/llvm-${SUPPORTED_LLVM_VERSION}/lib/cmake/${target}/"
      "/usr/share/llvm-${SUPPORTED_LLVM_VERSION}/cmake/${target}/"
      "/usr/lib/cmake/${target}/"
    )
    find_brew_prefix(BREW_PREFIX)
    if (NOT BREW_PREFIX STREQUAL "")
      list(PREPEND search_paths "${BREW_PREFIX}/opt/llvm/lib/cmake/${target}/")
    endif()
  else()
    message(FATAL_ERROR "Your platform is not supported. You need a Windows, Unix/Linux or MacOS system.")
  endif()
  set(${search_paths_out_var} ${search_paths} PARENT_SCOPE)
endfunction()

function(find_llvm_cmake_directories SUPPORTED_LLVM_VERSION llvm_cmake_dir_out_var clang_cmake_directory_out_var)
  set_llvm_target_search_paths("llvm" llvm_search_paths)
  find_path(LLVM_CMAKE_DIR LLVMConfig.cmake PATHS ${llvm_search_paths} NO_DEFAULT_PATH)
  if (NOT LLVM_CMAKE_DIR)
    message(FATAL_ERROR "Could not find LLVM CMake directory in the search paths. Please ensure LLVM is installed or provide its location with PATH_TO_LLVM as explained in the README.md.")
  endif()

  set_llvm_target_search_paths("clang" clang_search_paths)
  find_path(CLANG_CMAKE_DIR ClangConfig.cmake PATHS ${clang_search_paths} NO_DEFAULT_PATH)
  if (NOT CLANG_CMAKE_DIR)
    message(FATAL_ERROR "Could not find Clang CMake directory in the search paths. Please ensure Clang is installed or provide its location with PATH_TO_LLVM as explained in the README.md.")
  endif()

  message(STATUS "Found LLVM CMake directory: ${LLVM_CMAKE_DIR}")
  set(${llvm_cmake_dir_out_var} "${LLVM_CMAKE_DIR}" PARENT_SCOPE)

  message(STATUS "Found Clang CMake directory: ${CLANG_CMAKE_DIR}")
  set(${clang_cmake_directory_out_var} "${CLANG_CMAKE_DIR}" PARENT_SCOPE)
endfunction()

function(set_lit_search_paths search_paths_out_var)
  if (DEFINED LIT_PATH)
    message(STATUS "Using user-provided path to lit: ${LIT_PATH}")
    set(search_paths "${LIT_PATH}")
  elseif (WIN32)
    set(search_paths
      "$ENV{APPDATA}/Python/Scripts"
      "$ENV{LOCALAPPDATA}/Programs/Python/Scripts"
    )
  elseif (UNIX OR APPLE)
    set(search_paths
      "$ENV{HOME}/.local/bin"
      "/usr/local/bin"
      "/usr/bin"
    )
    find_brew_prefix(BREW_PREFIX)
    if (NOT BREW_PREFIX STREQUAL "")
      list(PREPEND search_paths "${BREW_PREFIX}/bin/lit")
    endif()
  else()
    message(FATAL_ERROR "Your platform is not supported. You need a Windows, Unix/Linux or MacOS system.")
  endif()

  set(${search_paths_out_var} ${search_paths} PARENT_SCOPE)
endfunction()

function(find_lit_program lit_path_out_var)
  set_lit_search_paths(lit_search_paths)
  find_program(lit_executable NAMES lit PATHS ${lit_search_paths})

  if (NOT lit_executable)
    message(FATAL_ERROR "Could not find 'lit' program. Please install lit using 'brew' or 'pip', or provide the LIT_PATH manually.")
  endif()

  message(STATUS "Using lit program: ${lit_executable}")
  set(${lit_path_out_var} "${lit_executable}" PARENT_SCOPE)
endfunction()

function(setup_precompiled_llvm compiler_location_out_var)
  if (NOT TARGET clang)
    message(FATAL_ERROR "Target 'clang' does not exist. No clue why and what to do about it.")
  endif()

  get_target_property(clang_location clang LOCATION)
  set(${compiler_location_out_var} ${clang_location} PARENT_SCOPE)
endfunction()

function(add_gogoote_executable)
  set (prefix local)
  set (optionArguments )
  set (singleValueArguments NAME MAIN)
  set (multipleValueArguments SOURCES LINK_WITH)

  cmake_parse_arguments(${prefix}
    "${optionArguments}"
    "${singleValueArguments}"
    "${multipleValueArguments}"
    ${ARGN}
  )

  add_library(${local_NAME}-static STATIC ${local_SOURCES})

  add_executable(${local_NAME} ${local_MAIN})
  target_link_libraries(${local_NAME} ${local_NAME}-static ${local_LINK_WITH} GogooteVersion)

  set(TARGETS "${local_NAME}-static" "${local_NAME}")
  foreach(TARGET ${TARGETS})
    set_target_properties(${TARGET} PROPERTIES
      LINK_FLAGS ${GOGOOTE_LINK_FLAGS}
      COMPILE_FLAGS ${GOGOOTE_CXX_FLAGS}
    )
    target_include_directories(${TARGET} PUBLIC
      ${GOGOOTE_INCLUDE_DIRS}
    )
    target_include_directories(${TARGET} SYSTEM PRIVATE
      ${THIRD_PARTY_INCLUDE_DIRS}
    )
  endforeach()

  install(TARGETS ${local_NAME}
    RUNTIME DESTINATION bin
  )
endfunction()
