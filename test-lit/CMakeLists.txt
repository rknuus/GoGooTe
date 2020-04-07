set(CLANG_EXEC ${PATH_TO_LLVM}/bin/clang++)
set(LIT_EXEC ${PATH_TO_LIT})
# TODO(KNR): currently only supporting UNIX-like systems, because don't have access to
# Windows system
set(FILE_EXISTS_EXEC ${CMAKE_CURRENT_SOURCE_DIR}/test-file-exists-unix.sh)
set(FILECHECK_EXEC filecheck)  # TODO(KNR): path?

# HACK(KNR): Copy entire test-lit directory from source to build directory, because we
# don't want to maintain a list of lit tests in CMake, GoGooTe creates files in-situ,
# and we don't want to mess up source directory.
#
# This is a hack, because the source and build directories are only partly kept in sync.
# New and changed files are copied automatically, but deleted files are not removed automatically!
#
# Copying directories at build time happens before running tests, so this is early enough.
add_custom_target(copy-test-lit-input ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/tests ${CMAKE_CURRENT_BINARY_DIR}/tests
)

# To avoid copying the entire test-lit directory copy the lit.cfg separately.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lit.cfg ${CMAKE_CURRENT_BINARY_DIR}/lit.cfg COPYONLY)

set(LIT_ENV "PATH_TO_LLVM=${PATH_TO_LLVM} CURRENT_DIR=${CMAKE_CURRENT_BINARY_DIR} GOGOOTE_EXEC=$<TARGET_FILE:gogoote> \
  CLANG_EXEC=${CLANG_EXEC} FILE_EXISTS_EXEC=${FILE_EXISTS_EXEC} FILECHECK_EXEC=${FILECHECK_EXEC}"
)
set(LIT_COMMAND "find ${CMAKE_CURRENT_BINARY_DIR}/tests -name '*.gtest.cpp' | xargs rm -rf; ${LIT_ENV} ${LIT_EXEC} -vv ${CMAKE_CURRENT_BINARY_DIR}/tests")

add_test(NAME test-lit
  COMMAND /bin/bash -c "${LIT_COMMAND}"
)
