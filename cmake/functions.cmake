macro(add_gogoote_executable)
  set (prefix local)
  set (optionArguments )
  set (singleValueArguments NAME)
  set (multipleValueArguments SOURCES LINK_WITH)

  cmake_parse_arguments(${prefix}
    "${optionArguments}"
    "${singleValueArguments}"
    "${multipleValueArguments}"
    ${ARGN}
  )

  add_executable(${local_NAME} ${local_SOURCES})
  target_link_libraries(${local_NAME} ${local_LINK_WITH} GogooteVersion)

  set_target_properties(${local_NAME} PROPERTIES
    LINK_FLAGS ${GOGOOTE_LINK_FLAGS}
    COMPILE_FLAGS ${GOGOOTE_CXX_FLAGS}
  )
  target_include_directories(${local_NAME} PUBLIC
    ${GOGOOTE_INCLUDE_DIRS}
  )
  target_include_directories(${local_NAME} SYSTEM PRIVATE
    ${THIRD_PARTY_INCLUDE_DIRS}
  )

  INSTALL(TARGETS ${local_NAME}
    RUNTIME DESTINATION bin
  )
endmacro()
