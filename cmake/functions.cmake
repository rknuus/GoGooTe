macro(add_gogoote_executable)  # TODO(KNR): try function
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

  INSTALL(TARGETS ${local_NAME}
    RUNTIME DESTINATION bin
  )
endmacro()
