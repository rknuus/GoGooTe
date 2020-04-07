execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND date "+%d %b %Y"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE BUILD_DATE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file (
  ${CMAKE_SOURCE_DIR}/src/Version.cpp
  ${CMAKE_BINARY_DIR}/src/Version.cpp
  @ONLY
)

add_library(GogooteVersion ${CMAKE_BINARY_DIR}/src/Version.cpp)
target_include_directories(GogooteVersion PRIVATE
  ${GOGOOTE_INCLUDE_DIRS}
)
target_include_directories(GogooteVersion SYSTEM PRIVATE
  ${THIRD_PARTY_INCLUDE_DIRS}
)
set_target_properties(GogooteVersion PROPERTIES
  LINK_FLAGS ${GOGOOTE_LINK_FLAGS}
  COMPILE_FLAGS ${GOGOOTE_CXX_FLAGS}
)
