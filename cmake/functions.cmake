function(find_brew_prefix brew_prefix_out_var)
  execute_process(
    COMMAND brew --version
    OUTPUT_QUIET
    ERROR_QUIET
    RESULT_VARIABLE brew_check_result
  )

  set(${brew_prefix_out_var} "" PARENT_SCOPE)
  if (brew_check_result EQUAL 0)
    execute_process(
      COMMAND brew --prefix
      OUTPUT_VARIABLE BREW_PREFIX
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(${brew_prefix_out_var} "${BREW_PREFIX}" PARENT_SCOPE)
  endif()
endfunction()

# FIXME(RAKN): generate concise usage messages for all optional variables and extend the variables by a prefix
# decide whether to use CMake options or not
# FIXME(RAKN): test the whole stuff on Linux and Windows
# FIXME(RAKN): if the directories were not found explain how to install LLVM in supported ways per platform
function(find_llvm_cmake_directories SUPPORTED_LLVM_VERSION llvm_cmake_dir_out_var clang_cmake_directory_out_var)
  function(set_llvm_target_search_paths target search_paths_out_var)
    if (DEFINED PATH_TO_LLVM)
      set(search_paths "${PATH_TO_LLVM}/lib/cmake/${target}")
    elseif (WIN32)
      set(search_paths
        "C:/Program Files/LLVM/lib/cmake/${target}/"
        "C:/Program Files (x86)/LLVM/lib/cmake/${target}/"
      )
    else()
      set(search_paths
        "/usr/lib/llvm-${SUPPORTED_LLVM_VERSION}/lib/cmake/${target}/"
        "/usr/share/llvm-${SUPPORTED_LLVM_VERSION}/cmake/${target}/"
        "/usr/lib/cmake/${target}/"
      )

      find_brew_prefix(HOMEBREW_PREFIX)
      if (NOT HOMEBREW_PREFIX STREQUAL "")
        list(APPEND search_paths "${HOMEBREW_PREFIX}/opt/llvm/lib/cmake/${target}/")
      endif()
    endif()

    set(${search_paths_out_var} ${search_paths} PARENT_SCOPE)
  endfunction()

  set_llvm_target_search_paths("llvm" llvm_search_paths)
  find_path(LLVM_CMAKE_DIR LLVMConfig.cmake PATHS ${llvm_search_paths} NO_DEFAULT_PATH)
  if (NOT LLVM_CMAKE_DIR)
    message(FATAL_ERROR "Could not find LLVM CMake directory in the search paths ${llvm_search_paths}. Please ensure LLVM is installed, and provide its location manually with PATH_TO_LLVM if necessary.")
  endif()

  set_llvm_target_search_paths("clang" clang_search_paths)
  find_path(CLANG_CMAKE_DIR ClangConfig.cmake PATHS ${clang_search_paths} NO_DEFAULT_PATH)
  if (NOT CLANG_CONFIG_CMAKE)
    message(FATAL_ERROR "Could not find Clang CMake directory in the search paths ${clang_search_paths}. Please ensure LLVM is installed, and provide its location manually with PATH_TO_LLVM if necessary.")
  endif()

  message(STATUS "Found LLVM CMake directory: ${LLVM_CMAKE_DIR}")
  set(${llvm_cmake_dir_out_var} "${LLVM_CMAKE_DIR}" PARENT_SCOPE)

  message(STATUS "Found Clang CMake directory: ${CLANG_CMAKE_DIR}")
  set(${clang_cmake_directory_out_var} "${CLANG_CMAKE_DIR}" PARENT_SCOPE)
endfunction()

function(check_homebrew_package_installed package_name result_out_var)
  # Check if brew is installed
  execute_process(
    COMMAND brew --version
    OUTPUT_QUIET
    ERROR_QUIET
    RESULT_VARIABLE brew_check_result
  )

  if (NOT brew_check_result EQUAL 0)
    set(${result_out_var} FALSE PARENT_SCOPE)
    return()
  endif()

  # Check if the package is installed
  execute_process(
    COMMAND brew list --formula ${package_name}
    OUTPUT_QUIET
    ERROR_QUIET
    RESULT_VARIABLE package_check_result
  )

  set(${result_out_var} $<BOOL:${package_check_result} EQUAL 0> PARENT_SCOPE)
endfunction()

function(find_homebrew_prefix package_name brew_prefix_out_var)
  execute_process(
    COMMAND brew --prefix ${package_name}
    OUTPUT_VARIABLE BREW_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET  # errors are handled below
  )

  if (NOT BREW_PREFIX)
    message(FATAL_ERROR "Could not find ${package_name} using Homebrew. Please ensure ${package_name} is installed via `brew install ${package_name}`.")
  endif()

  message(STATUS "Homebrew prefix for ${package_name}: ${BREW_PREFIX}")
  set(${brew_prefix_out_var} "${BREW_PREFIX}" PARENT_SCOPE)
endfunction()

function(find_lit_program lit_path_out_var)
  if (DEFINED LIT_PATH)
    message(STATUS "Using user-provided path to lit: ${LIT_PATH}")
    set(lit_executable ${LIT_PATH})
  else()
    check_homebrew_package_installed(lit HOMEBREW_PACKAGE_LIT_INSTALLED)
    if (HOMEBREW_PACKAGE_LIT_INSTALLED)
      find_homebrew_prefix(lit LIT_PREFIX)
      set(lit_executable "${LIT_PREFIX}/bin/lit")
    elseif (UNIX OR APPLE)  # Unix* platforms using pip to install lit
      find_program(lit_executable lit HINTS $ENV{HOME}/.local/bin /usr/local/bin /usr/bin)
    elseif (WIN32)  # Windows platforms using pip to install lit
      find_program(lit_executable lit HINTS $ENV{APPDATA}/Python/Scripts $ENV{LOCALAPPDATA}/Programs/Python/Scripts)
    else()
      message(FATAL_ERROR "Unsupported platform")
    endif()
  endif()

  if (NOT lit_executable)
    message(FATAL_ERROR "Could not find lit program. Please install lit using `brew` or `pip`, or provide the LIT_PATH manually.")
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
