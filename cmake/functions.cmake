function(find_brew_prefix brew_prefix_out_var)
  execute_process(COMMAND brew --version OUTPUT_QUIET ERROR_QUIET RESULT_VARIABLE result)
  if (result EQUAL 0)
    execute_process(COMMAND brew --prefix OUTPUT_VARIABLE PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(${brew_prefix_out_var} "${PREFIX}" PARENT_SCOPE)
  else()
    set(${brew_prefix_out_var} "" PARENT_SCOPE)
  endif()
endfunction()

function(setup_precompiled_llvm compiler_location_out_var)
  if (NOT TARGET clang)
    message(FATAL_ERROR "Target 'clang' does not exist. No clue why and what to do about it.")
  endif()

  get_target_property(clang_location clang LOCATION)
  set(${compiler_location_out_var} ${clang_location} PARENT_SCOPE)
endfunction()

function(configure_gogoote_version
  GOGOOTE_INCLUDE_DIRS
  THIRD_PARTY_INCLUDE_DIRS
  GOGOOTE_LINK_FLAGS
  GOGOOTE_CXX_FLAGS
)
  execute_process(COMMAND git log -1 --format=%h OUTPUT_VARIABLE GIT_COMMIT OUTPUT_STRIP_TRAILING_WHITESPACE)

  if (WIN32)
    execute_process(COMMAND powershell -Command "Get-Date -Format 'dd MMM yyyy'"
                    OUTPUT_VARIABLE BUILD_DATE
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
  elseif (UNIX OR APPLE)
    execute_process(COMMAND date "+%d %b %Y" OUTPUT_VARIABLE BUILD_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
  else()
    message(FATAL_ERROR "Your platform is not supported. You need a Windows, Unix/Linux or MacOS system.")
  endif()

  configure_file(${CMAKE_SOURCE_DIR}/src/Version.cpp ${CMAKE_BINARY_DIR}/src/Version.cpp @ONLY)

  add_library(GogooteVersion ${CMAKE_BINARY_DIR}/src/Version.cpp)

  target_include_directories(GogooteVersion PRIVATE ${GOGOOTE_INCLUDE_DIRS})
  target_include_directories(GogooteVersion SYSTEM PRIVATE ${THIRD_PARTY_INCLUDE_DIRS})

  set_target_properties(GogooteVersion PROPERTIES
    LINK_FLAGS ${GOGOOTE_LINK_FLAGS}
    COMPILE_FLAGS ${GOGOOTE_CXX_FLAGS}
  )
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
