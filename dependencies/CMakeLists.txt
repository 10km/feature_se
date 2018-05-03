#cmake file for dependencies 
#author:guyadong
#created:2018/05/03
cmake_minimum_required( VERSION 2.6 )
# 依赖代码位置
#set(DEPENDENT_SOURCE_DIR "${PROJECT_BINARY_DIR}/dependent_sources" CACHE STRINGS "dependent source folder" FORCE)
##定义公共代码位置   
#set( COMMONS_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../common_source_cpp)
#if(NOT EXISTS ${COMMONS_SOURCE_DIR}/CMakeLists.txt )
#	message( FATAL_ERROR "Not exists or Empty folder: ${COMMONS_SOURCE_DIR},the submodule must be init and pull" )	
#endif()

## WITH_STATIC_CRT 定义参见 ../cmake/compiler_flag_overrides.cmake
##############根据编译条件定义gflags位置##############
define_project_folder_name(gflags "${WITH_STATIC_CRT}")
if( IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${gflags_folder}" )
	unset(GFLAGS_DIR CACHE)
	set( GFLAGS_DIR "${CMAKE_CURRENT_LIST_DIR}/${gflags_folder}" CACHE INTERNAL "glfags folder")
	message(STATUS "GFLAGS_DIR=${GFLAGS_DIR}" )
	list(APPEND CMAKE_PREFIX_PATH ${GFLAGS_DIR})
elseif(MSVC)
	message(FATAL_ERROR "not found fitted gflags directory:${gflags_folder},please see./README.md") 
endif()

##############根据编译条件定义glog位置##############
define_project_folder_name(glog "${WITH_STATIC_CRT}")
if( IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${glog_folder}" )
	unset(GLOG_DIR CACHE)
	set( GLOG_DIR "${CMAKE_CURRENT_LIST_DIR}/${glog_folder}" CACHE INTERNAL "glog folder")
	message(STATUS "GLOG_DIR=${GLOG_DIR}")
	list(APPEND CMAKE_PREFIX_PATH ${GLOG_DIR})
elseif(MSVC)
	message(FATAL_ERROR "not found fitted glog directory:${glog_folder},please see./README.md") 
endif()
# 将值传递到上一级
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" PARENT_SCOPE)
message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}" )

#foreach( subdir common_source_cpp )
#	if( IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${subdir}" )
#		MESSAGE( STATUS "Found sub project ${subdir}, adding it" )
#		add_subdirectory( ${subdir} )
#	endif()
#endforeach()
