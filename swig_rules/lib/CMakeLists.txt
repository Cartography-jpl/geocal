list(APPEND SWIG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND SWIG_EXTRA_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/serialize_function.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/swig_type_mapper_base.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/weak_ptr_serialize_support.cc
  )
list(APPEND SWIG_EXTRA_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/serialize_function.h
  ${CMAKE_CURRENT_SOURCE_DIR}/swig_to_python.h
  ${CMAKE_CURRENT_SOURCE_DIR}/swig_type_mapper_base.h
  ${CMAKE_CURRENT_SOURCE_DIR}/swig_type_mapper.h
  ${CMAKE_CURRENT_SOURCE_DIR}/weak_ptr_serialize_support.h
  )
  
set(SWIG_INCLUDE_DIR ${SWIG_INCLUDE_DIR} PARENT_SCOPE)
set(SWIG_EXTRA_SOURCE_FILES ${SWIG_EXTRA_SOURCE_FILES} PARENT_SCOPE)
set(SWIG_EXTRA_HEADER_FILES ${SWIG_EXTRA_HEADER_FILES} PARENT_SCOPE)
