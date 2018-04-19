#cmake file for library fse(feature search engine)
#author:guyadong
#created:2018/04/13
include (CMakePackageConfigHelpers)
# define RUNTIME_INSTALL_DIR LIBRARY_INSTALL_DIR INCLUDE_INSTALL_DIR CONFIG_INSTALL_DIR
set (RUNTIME_INSTALL_DIR bin)
set (LIBRARY_INSTALL_DIR lib)
set (INCLUDE_INSTALL_DIR include)
if(WIN32 AND NOT CYGWIN)
  set (CONFIG_INSTALL_DIR  cmake)
else()
  set (CONFIG_INSTALL_DIR  ${LIBRARY_INSTALL_DIR}/cmake)
endif()
find_package(Cassdk REQUIRED)
# 定义所有的源文件列表  
set(_SOURCE_FILES
    OverTableManager.cpp
    HashMapCl.cpp 
    ICodeManager.cpp
    ICodeManagerCPU.cpp
    CodeManagerFactory.cpp
    TopKCodeBean.cpp
    feature_se.cpp
    ${COMMONS_SOURCE_FOLDER}/RWLock.cpp
    ${COMMONS_SOURCE_FOLDER}/ThreadPool.cpp
    )
# 用于外部调用的公共头文件
set (_PUBLIC_HEADERS
    config.h 
    feature_data_types.h 
    cross_define.h 
    feature_se.h
    )
set (_FSE_TARGETS fse_cas_static fse_cas)
# CASSDK版本的CodeMgr实现
add_library(fse_cas_static STATIC ${_SOURCE_FILES})
add_library(fse_cas        SHARED ${_SOURCE_FILES})

use_static_library(fse_cas)

if(MSVC) 
	add_library(fse_cas_static_mt STATIC ${_SOURCE_FILES})
  with_mt_if_msvc(fse_cas_static_mt)
  list(APPEND _FSE_TARGETS fse_cas_static_mt)
endif(MSVC)

set_target_output_name("${_FSE_TARGETS}")
# 设置项目属性
function (define_fse_target target)
if(NOT TARGET ${target})
	return ()
endif()
get_target_property(_type ${target} TYPE)
if(${_type} STREQUAL "SHARED_LIBRARY")
  set(_is_shared TRUE)
  set(_is_shared_str "1")
else()
  set(_is_shared_str "0")
endif()
# 加入CASSDK 版本定义,编译库时必须加FSE_EXPORTS定义
target_compile_definitions (${target} 
	PRIVATE FSE_EXPORTS
	PUBLIC FSE_IS_A_DLL=${_is_shared_str}
	PUBLIC CASSDK
	)
set_target_properties (${target}  PROPERTIES 
	PUBLIC_HEADER "${_PUBLIC_HEADERS}"
	VERSION ${PROJECT_VERSION} 
	SOVERSION ${PROJECT_VERSION_MAJOR}
	)
set(_link_libraries cassdk_THFeature)
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	list(APPEND _link_libraries -pthread -ldl)
endif()
if(_is_shared)
	target_link_libraries(${target} PRIVATE ${_link_libraries})
else()
	set_target_properties (${target}  PROPERTIES 	INTERFACE_POSITION_INDEPENDENT_CODE ON)
	target_link_libraries(${target} PUBLIC ${_link_libraries})
endif()
target_include_directories (${target} INTERFACE "$<INSTALL_INTERFACE:include>")
endfunction()

define_fse_target(fse_cas_static)
define_fse_target(fse_cas_static_mt)
define_fse_target(fse_cas)


# 测试程序
add_executable(fctest EXCLUDE_FROM_ALL FeatureCompareSpeedTest.cpp)
add_executable(fctest2 EXCLUDE_FROM_ALL FeatureCompareVeracityTest.cpp)
# ----------------------------------------------------------------------------
set (fse_DEPENDENCY "find_dependency(Cassdk)")
configure_package_config_file (${PROJECT_SOURCE_DIR}/cmake/config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/fse-config.cmake
  INSTALL_DESTINATION cmake
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
