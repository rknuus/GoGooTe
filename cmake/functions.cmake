function(resolve_absolute_path var base_dir)
  if (NOT IS_ABSOLUTE ${${var}})
    get_filename_component(${var}
      "${${var}}" REALPATH BASE_DIR "${base_dir}")
    set(${var} "${${var}}" PARENT_SCOPE)
  endif()
endfunction()

function(find_llvm_cmake_dir llvm_cmake_directory_out_var)
  if (DEFINED PATH_TO_LLVM)
    set(search_paths ${PATH_TO_LLVM})
  else()
    if (APPLE)
      if (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        # ARM-based Macs (M1/M2) using Homebrew
        set(search_paths /opt/homebrew/opt/llvm/lib/cmake/llvm/)
      else()
        # Intel-based Macs using Homebrew
        set(search_paths /usr/local/opt/llvm/lib/cmake/llvm/)
      endif()
    elseif (UNIX)
      # Linux platforms
      # FIXME(RAKN): generalize to match llvm-18 til future versions
      set(search_paths
        /usr/lib/llvm-10/lib/cmake/llvm/      # Example for LLVM 10 on Ubuntu/Debian
        /usr/lib/llvm-12/lib/cmake/llvm/      # Example for LLVM 12 on Fedora/RHEL
        /usr/share/llvm/cmake/                # General shared cmake folder
        /usr/lib/cmake/llvm/                  # General cmake folder
      )
    elseif (WIN32)
      # Windows platforms using LLVM installer
      set(search_paths "C:/Program Files/LLVM/lib/cmake/llvm/")
    else()
      message(FATAL_ERROR "Unsupported platform")
    endif()
  endif()

  find_file(LLVM_CONFIG_CMAKE "LLVM-Config.cmake" PATHS ${search_paths} NO_DEFAULT_PATH)
  if (NOT LLVM_CONFIG_CMAKE)
    message(FATAL_ERROR "Could not find LLVM-Config.cmake in the specified paths")
  endif()
  get_filename_component(llvm_cmake_dir "${LLVM_CONFIG_CMAKE}" DIRECTORY)
  set(${llvm_cmake_directory_out_var} "${llvm_cmake_dir}" PARENT_SCOPE)
endfunction()

function(find_clang_cmake_dir clang_cmake_directory_out_var)
  if (DEFINED PATH_TO_LLVM)
    # FIXME(RAKN): this does not work for all Linux locations
    set(search_paths ${PATH_TO_LLVM}/lib/cmake/clang)
  else()
    if (APPLE)
      if (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        # ARM-based Macs (M1/M2) using Homebrew
        set(search_paths /opt/homebrew/opt/llvm/lib/cmake/clang/)
      else()
        # Intel-based Macs using Homebrew
        set(search_paths /usr/local/opt/llvm/lib/cmake/clang/)
      endif()
    elseif (UNIX)
      # Linux platforms (check common APT/YUM paths for Clang)
      # FIXME(RAKN): generalize to match llvm-18 til future versions
      set(search_paths
        /usr/lib/llvm-10/lib/cmake/clang/    # Example for Clang 10 on Ubuntu/Debian
        /usr/lib/llvm-12/lib/cmake/clang/    # Example for Clang 12 on Fedora/RHEL
        /usr/share/clang/cmake/              # General shared CMake folder for Clang
        /usr/lib/cmake/clang/                # General cmake folder for Clang
      )
    elseif (WIN32)
      # Windows platforms using LLVM/Clang installer
      set(search_paths "C:/Program Files/LLVM/lib/cmake/clang/")
    else()
      message(FATAL_ERROR "Unsupported platform")
    endif()
  endif()

  find_file(CLANG_CONFIG_CMAKE "ClangConfig.cmake" PATHS ${search_paths} NO_DEFAULT_PATH)
  if (NOT CLANG_CONFIG_CMAKE)
    message(FATAL_ERROR "Could not find ClangConfig.cmake in the specified paths")
  endif()
  get_filename_component(clang_cmake_dir "${CLANG_CONFIG_CMAKE}" DIRECTORY)
  message(STATUS "Found ClangConfig.cmake in: ${clang_cmake_dir}")
  set(${clang_cmake_directory_out_var} "${clang_cmake_dir}" PARENT_SCOPE)
endfunction()

# FIXME(RAKN): delete
function(setup_llvm_cmake_search_paths search_paths_out_var)
  # TODO(RAKN): support other platforms than MacOS X on M*
  set(path_to_llvm "/opt/homebrew/opt/llvm/lib/cmake/llvm/")
  set(${search_paths_out_var}
    ${path_to_llvm}
    ${path_to_llvm}/lib/cmake
    ${path_to_llvm}/lib/cmake/llvm
    ${path_to_llvm}/lib/cmake/clang
    ${path_to_llvm}/share/clang/cmake/
    ${path_to_llvm}/share/llvm/cmake/
    PARENT_SCOPE
  )
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
