#cmake file for library fse(feature search engine)
#author:guyadong
#created:2018/04/13
include (CMakePackageConfigHelpers)
if(WIN32 AND NOT CYGWIN)
  set (CONFIG_INSTALL_DIR  cmake)
else()
  set (CONFIG_INSTALL_DIR  ${LIBRARY_INSTALL_DIR}/cmake/fse)
endif()
if(EXT_SDK_TYPE STREQUAL "CASSDK")
	find_package(Cassdk REQUIRED)
elseif(EXT_SDK_TYPE STREQUAL "EUCLIDEAN")
elseif(EXT_SDK_TYPE STREQUAL "CUSTOM")
	if(NOT IS_DIRECTORY ${CUSTOM_FEACOMP_INCLUDE})
		message(FATAL_ERROR "NOT FOUND include directory ${CUSTOM_FEACOMP_INCLUDE}")
	endif()
	set(CUSTOM_INCLUDE_STATEMENTS )
	foreach(_name ${CUSTOM_SYS_HEADERS})
		set(CUSTOM_INCLUDE_STATEMENTS "${CUSTOM_INCLUDE_STATEMENTS}
#include <${_name}>")
	endforeach()
	message(STATUS CUSTOM_FEACOMP_HEADERS=${CUSTOM_FEACOMP_HEADERS})
	foreach(_name ${CUSTOM_FEACOMP_HEADERS})
		set(CUSTOM_INCLUDE_STATEMENTS "${CUSTOM_INCLUDE_STATEMENTS}
#include \"${_name}\"
")
	endforeach()
	# 生成比较函数代码
	configure_file(${PROJECT_SOURCE_DIR}/cmake/custom_feature_compare.h.in
  		${PROJECT_BINARY_DIR}/generated/custom_feature_compare.h @ONLY )
else()
	message(STATUS "unsupported operation")
endif()
# 定义所有的源文件列表  
set(_SOURCE_FILES
    OverTableManager.cpp
    HashMapCl.cpp 
    ICodeManager.cpp
    ICodeManagerCPU.cpp
    CodeManagerFactory.cpp
    TopKCodeBean.cpp
    feature_se.cpp
    ${DEPENDENT_SOURCE_DIR}/RWLock.cpp
    ${DEPENDENT_SOURCE_DIR}/ThreadPool.cpp
    )
# 用于外部调用的公共头文件
set (_PUBLIC_HEADERS
    config.h 
    feature_data_types.h 
    cross_define.h 
    feature_se.h
    )
set (_FSE_TARGETS ${FSE_LIBNAME}_static ${FSE_LIBNAME})
# CASSDK版本的CodeMgr实现
add_library(${FSE_LIBNAME}_static STATIC ${_SOURCE_FILES})
add_library(${FSE_LIBNAME}        SHARED ${_SOURCE_FILES})

use_static_library(${FSE_LIBNAME})

if(MSVC) 
	add_library(${FSE_LIBNAME}_static_mt STATIC ${_SOURCE_FILES})
  with_mt_if_msvc(${FSE_LIBNAME}_static_mt)
  list(APPEND _FSE_TARGETS ${FSE_LIBNAME}_static_mt)
endif(MSVC)

#########################3
# 设置项目属性
function (define_fse_target target)
if(NOT TARGET ${target})
	return ()
endif()
# MSVC Debug 编译时生成包含用于调试器的完整符号调试信息的 .obj 文件
# see also https://msdn.microsoft.com/zh-cn/library/958x11bc.aspx
target_compile_options(${target} PRIVATE $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Z7>)

get_target_property(_type ${target} TYPE)
if(${_type} STREQUAL "SHARED_LIBRARY")
  set(_is_shared TRUE)
  set(_is_shared_str "1")
else()
	set(_is_shared FALSE)
  set(_is_shared_str "0")
endif()
target_include_directories (${target} 
	PRIVATE ${CMAKE_CURRENT_LIST_DIR}
	INTERFACE "$<INSTALL_INTERFACE:include>"
	)
# 编译库时必须加FSE_EXPORTS定义
target_compile_definitions (${target} 
	PRIVATE FSE_EXPORTS
	PUBLIC FSE_IS_A_DLL=${_is_shared_str}
	_SL_USE_BYTE_STREAM
	)
set_target_properties (${target}  PROPERTIES 
	PUBLIC_HEADER "${_PUBLIC_HEADERS}"
	VERSION ${PROJECT_VERSION} 
	SOVERSION ${PROJECT_VERSION_MAJOR}
	DEBUG_POSTFIX _d
	)
set(_link_libraries )
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	list(APPEND _link_libraries -pthread -ldl)
endif()
####################
if(CASSDK_USED)
	# 加入CASSDK 版本定义
	target_compile_definitions (${target} 
		PUBLIC CASSDK=1
		)
	# 加入CASSDK　import target
	list(APPEND _link_libraries cassdk_THFeature)
endif(CASSDK_USED)
if(EXT_SDK_TYPE STREQUAL "EUCLIDEAN")
	# 加入特征长度和类型定义
	target_compile_definitions (${target} 
		PUBLIC EUCLIDEAN_FEACOMP=1
		PUBLIC CODE_ELEM_TYPE=${EUCLIDEAN_ELEM_TYPE}
		PUBLIC CODE_ELEM_LEN=${EUCLIDEAN_ELEM_LEN}
		)	
	if(EUCLIDEAN_CODE_END_WITH_SUM)
		# 指定特征值数组后以double类型的点积和结尾
		target_compile_definitions (${target} PUBLIC CODE_END_WITH_SUM=1)
	endif()
endif()
if(EXT_SDK_TYPE STREQUAL "CUSTOM")
	# 加入自定义的包含特征比较函数的算法库
	target_compile_definitions(${target} 
		PUBLIC CUSTOM_FEACOMP=1 
		PUBLIC CODE_BYTE_LENGTH=${CUSTOM_CODE_BYTE_LEN})
	target_link_libraries(${target} PRIVATE ${CUSTOM_FEACOMP_LIBRARY})
	target_include_directories(${target} PRIVATE "${PROJECT_BINARY_DIR}/generated" ${CUSTOM_FEACOMP_INCLUDE})
endif()
target_link_libraries(${target} PRIVATE $<BUILD_INTERFACE:common_source>)
if(_is_shared)
	target_link_libraries(${target} PRIVATE ${_link_libraries})
else()
	target_link_libraries(${target} PUBLIC ${_link_libraries})
	set_target_properties (${target}  PROPERTIES INTERFACE_POSITION_INDEPENDENT_CODE ON
																							           POSITION_INDEPENDENT_CODE ON)
endif()
endfunction()
#####################

define_fse_target(${FSE_LIBNAME}_static)
define_fse_target(${FSE_LIBNAME}_static_mt)
define_fse_target(${FSE_LIBNAME})

# 测试程序
add_executable(fctest  EXCLUDE_FROM_ALL FeatureCompareSpeedTest.cpp)
add_executable(fctest2 EXCLUDE_FROM_ALL FeatureCompareVeracityTest.cpp)

target_link_libraries(fctest common_source)
target_link_libraries(fctest2 common_source $<$<CXX_COMPILER_ID:GNU>:-lpthread>)
target_compile_options(fctest2 PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)
# ----------------------------------------------------------------------------
set (fse_DEPENDENCY "find_dependency(Cassdk)")
configure_package_config_file (${PROJECT_SOURCE_DIR}/cmake/config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/fse-config.cmake
  INSTALL_DESTINATION ${CONFIG_INSTALL_DIR}
  NO_CHECK_REQUIRED_COMPONENTS_MACRO)
write_basic_package_version_file (fse-config-version.cmake VERSION
  ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)
########安装脚本##########
install(TARGETS ${_FSE_TARGETS} EXPORT fse-targets
  RUNTIME DESTINATION ${RUNTIME_INSTALL_DIR}
  LIBRARY DESTINATION ${LIBRARY_INSTALL_DIR}
  ARCHIVE DESTINATION ${LIBRARY_INSTALL_DIR}
  PUBLIC_HEADER DESTINATION ${INCLUDE_INSTALL_DIR}/feature_se
  )

if(MSVC)
	foreach(_target ${_FSE_TARGETS})
		# 如果是动态库则安装对应的pdb文件
		install(FILES $<$<STREQUAL:SHARED_LIBRARY,$<TARGET_PROPERTY:${_target},TYPE>>:$<TARGET_PDB_FILE:${_target}>> DESTINATION ${RUNTIME_INSTALL_DIR} OPTIONAL)
	endforeach(_target)
endif(MSVC)

install (FILES
  ${CMAKE_CURRENT_BINARY_DIR}/fse-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/fse-config-version.cmake
  DESTINATION ${CONFIG_INSTALL_DIR}
  )
install(EXPORT fse-targets NAMESPACE gdface:: DESTINATION ${CONFIG_INSTALL_DIR})

# Cleanup temporary variables.
set(_PUBLIC_HEADERS)
set(_SOURCE_FILES)
set(_FSE_TARGETS)
