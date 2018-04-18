## Utility CMake functions.

# ----------------------------------------------------------------------------
## Convert boolean value to 0 or 1
macro (bool_to_int VAR)
  if (${VAR})
    set (${VAR} 1)
  else ()
    set (${VAR} 0)
  endif ()
endmacro ()
## 对给定的library_name,在lib_dir中查找指定类型(library_type)的库(library_type不可包含空格)，
## 定义为{library_name}(大写)${library_type} 全局变量
function(defineLibraryVariable library_name library_type lib_dir )
  ##计算${library_name}的静态库名
  string(TOUPPER ${library_name} var_prefix)
  ##查找${library_name}的库
  string(REPLACE _ " " doc_str "${library_name} ${library_type}")
  string(TOLOWER ${doc_str} doc_str)
  set (lib_var_name ${var_prefix}${library_type})
  unset(${lib_var_name} CACHE)
  find_library( ${lib_var_name}
    NAMES ${library_name}
    PATHS ${lib_dir}
    DOC ${doc_str}
    NO_DEFAULT_PATH
  )
  message(STATUS "${lib_var_name}=${${lib_var_name}}" )
endfunction()
# ----------------------------------------------------------------------------
## Extract version numbers from version string
function (version_numbers version major minor patch)
  if (version MATCHES "([0-9]+)(\\.[0-9]+)?(\\.[0-9]+)?(rc[1-9][0-9]*|[a-z]+)?")
    if (CMAKE_MATCH_1)
      set (_major ${CMAKE_MATCH_1})
    else ()
      set (_major 0)
    endif ()
    if (CMAKE_MATCH_2)
      set (_minor ${CMAKE_MATCH_2})
      string (REGEX REPLACE "^\\." "" _minor "${_minor}")
    else ()
      set (_minor 0)
    endif ()
    if (CMAKE_MATCH_3)
      set (_patch ${CMAKE_MATCH_3})
      string (REGEX REPLACE "^\\." "" _patch "${_patch}")
    else ()
      set (_patch 0)
    endif ()
  else ()
    set (_major 0)
    set (_minor 0)
    set (_patch 0)
  endif ()
  set ("${major}" "${_major}" PARENT_SCOPE)
  set ("${minor}" "${_minor}" PARENT_SCOPE)
  set ("${patch}" "${_patch}" PARENT_SCOPE)
endfunction ()

# ----------------------------------------------------------------------------
## Configure public header files
function (configure_headers out)
  set (tmp)
  foreach (src IN LISTS ARGN)
    if (IS_ABSOLUTE "${src}")
      list (APPEND tmp "${src}")
    elseif (EXISTS "${PROJECT_SOURCE_DIR}/src/${src}.in")
      configure_file ("${PROJECT_SOURCE_DIR}/src/${src}.in" "${PROJECT_BINARY_DIR}/include/${GFLAGS_INCLUDE_DIR}/${src}" @ONLY)
      list (APPEND tmp "${PROJECT_BINARY_DIR}/include/${GFLAGS_INCLUDE_DIR}/${src}")
    else ()
      configure_file ("${PROJECT_SOURCE_DIR}/src/${src}" "${PROJECT_BINARY_DIR}/include/${GFLAGS_INCLUDE_DIR}/${src}" COPYONLY)
      list (APPEND tmp "${PROJECT_BINARY_DIR}/include/${GFLAGS_INCLUDE_DIR}/${src}")
    endif ()
  endforeach ()
  set (${out} "${tmp}" PARENT_SCOPE)
endfunction ()

# ----------------------------------------------------------------------------
## Configure source files with .in suffix
function (configure_sources out)
  set (tmp)
  foreach (src IN LISTS ARGN)
    if (src MATCHES ".h$" AND EXISTS "${PROJECT_SOURCE_DIR}/src/${src}.in")
      configure_file ("${PROJECT_SOURCE_DIR}/src/${src}.in" "${PROJECT_BINARY_DIR}/include/${GFLAGS_INCLUDE_DIR}/${src}" @ONLY)
      list (APPEND tmp "${PROJECT_BINARY_DIR}/include/${GFLAGS_INCLUDE_DIR}/${src}")
    else ()
      list (APPEND tmp "${PROJECT_SOURCE_DIR}/src/${src}")
    endif ()
  endforeach ()
  set (${out} "${tmp}" PARENT_SCOPE)
endfunction ()

function(fatal_if_not_target target)
if(NOT TARGET ${target})
  message(FATAL_ERROR "INVALID TARGET :${target}")
endif()
endfunction()

function (link_static_libstdcxx_if_linux target )
############linux下静态链接c++库###################
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  fatal_if_not_target(${target})
  get_target_property(_type ${target} TYPE)
  if(_type STREQUAL "STATIC_LIBRARY")
    message(STATUS "WARNING:${target} is STATIC_LIBRARY,skip")    
  else()
    get_target_property(_options ${target} LINK_FLAGS)
    if(_options)
      set_target_properties(${target} PROPERTIES LINK_FLAGS "-static-libstdc++ ${_options}" )
    else()
      set_target_properties(${target} PROPERTIES LINK_FLAGS "-static-libstdc++" )
    endif()
    message(STATUS "target ${target} add link option:-static-libstdc++")
  endif()
  unset(_type)
endif()
endfunction ()

# Use the static C library for all build types to a target
function (with_mt_if_msvc target )
fatal_if_not_target(${target})
if(MSVC) 
  set(_mt "/MT$<$<CONFIG:Debug>:d>")
  get_target_property(_options ${target} COMPILE_OPTIONS)
  if(_options)
    # message(STATUS "${target} COMPILE_OPTIONS=${_options}")
    if(${_options} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" _options "${_options}")
    else()
      set(_options "${_options} ${_mt}")
    endif()    
  else()
    set(_options "${_mt}")
  endif()
  get_target_property(_type ${target} TYPE)
  if(_type STREQUAL "STATIC_LIBRARY")
    # 静态库将/MT选项加入INTERFACE_COMPILE_OPTIONS
    target_compile_options( ${target} PUBLIC "${_options}")
  else()
    target_compile_options( ${target} PRIVATE "${_options}")
  endif()
  # Cleanup temporary variables.
  unset(_mt)
  unset(_options)
  unset(_type)
  message(STATUS "target ${target} use static runtime /MT")
endif(MSVC)
endfunction()
# Use the static C library for all build types to all target
function(with_mt_if_msvc_for_all)  
  foreach(var 
      CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
    )
    if(${var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}")
    endif()
  endforeach()  
endfunction()

# 对executeable和shared library target有效
function (use_static_library target )
fatal_if_not_target(${target})
get_target_property(_type ${target} TYPE)
if(_type STREQUAL "STATIC_LIBRARY")
  message(STATUS "WARNING:${target} is STATIC_LIBRARY,skip")
else()
  link_static_libstdcxx_if_linux(${target})
  with_mt_if_msvc(${target})
endif()
unset(_type)
endfunction()

function(cxx11_support)
#判断编译器类型,如果是gcc编译器,则在编译选项中加入c++11支持
if(CMAKE_COMPILER_IS_GNUCXX)     
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}" PARENT_SCOPE)
  message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)
endfunction()

# debug模式下对输出文件名加_d后缀
function(set_target_output_name targets)
foreach(target ${targets})
  fatal_if_not_target(${target})
  get_target_property(_output_name ${target} OUTPUT_NAME_DEBUG)  
  if(_output_name)
    set_target_properties(${target} PROPERTIES OUTPUT_NAME_DEBUG "${_output_name}_d" )
  else()
    set_target_properties(${target} PROPERTIES OUTPUT_NAME_DEBUG "${target}_d" )
  endif()  
  get_target_property(_output_name ${target} OUTPUT_NAME_DEBUG)
  message(STATUS "TARGET:${target} OUTPUT_NAME_DEBUG=${_output_name}")
  unset(_output_name)
endforeach()
endfunction()