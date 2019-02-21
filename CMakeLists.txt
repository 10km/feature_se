#cmake file for project codemgr
#author:guyadong
#created:2015/10/05  
############################################################################
cmake_minimum_required( VERSION 3.0 )
cmake_policy(SET CMP0048 NEW)
# 3.0以上版本才允许使用VERSION option
project( feature_se VERSION 1.0.0 LANGUAGES CXX)
message(STATUS "Project: ${PROJECT_NAME} ${PROJECT_VERSION}") 
message(STATUS "Project Directory: ${PROJECT_SOURCE_DIR}")
message(STATUS "Project Binary Directory: ${PROJECT_BINARY_DIR}")
message(STATUS "CMAKE_COMPILER=${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}")

# 设置外部提供人脸识别SDK类型,可选值CASSDK,CUSTOM,DEFAULT
# 当设置为CUSTOM时，必须指定 CUSTOM_SDK_INCLUDE CUSTOM_SDK_LIBRARY
# 设置为DEFAULT时，使用默认的点积比对函数
if(NOT EXT_SDK_TYPE)
	## 默认值CASSDK
	set(EXT_SDK_TYPE CASSDK)
	set(CASSDK_USED TRUE)
endif()

if(EXT_SDK_TYPE STREQUAL "CUSTOM")
	if(NOT CUSTOM_SDK_INCLUDE OR NOT CUSTOM_SDK_LIBRARY)
		message(FATAL_ERROR "MUST DEFINE CUSTOM_SDK_INCLUDE CUSTOM_SDK_LIBRARY")
	endif()	
endif()

#set(ANDROID_TOOLCHAIN_NAME arm-linux-androideabi-4.9)
#判断编译类型和版本是否满足编译要求
if(MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 19)
	message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}") 
	message(STATUS "CMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}" )
elseif(CMAKE_COMPILER_IS_GNUCXX)
	include(CheckCXXCompilerFlag)
	## 检测编译器是否支持C++11
	check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
	if(COMPILER_SUPPORTS_CXX11)
		message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}") 
		message(STATUS "CMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}" )
	else()
		message(FATAL_ERROR "compiler required: Visual Studio 2015 OR surrpoted -std=c++11" )
	endif()	
endif()
# includes utils.cmake module
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")
include (utils)
# 依赖代码位置
set(DEPENDENT_SOURCE_DIR "${PROJECT_BINARY_DIR}/dependent_sources" CACHE STRINGS "dependent source folder" FORCE)
#定义公共代码位置   
set( COMMONS_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/common_source_cpp)
if(NOT EXISTS ${COMMONS_SOURCE_DIR}/CMakeLists.txt )
	message( FATAL_ERROR "Not exists or Empty folder: ${COMMONS_SOURCE_DIR},the submodule must be init and pull" )	
endif()
cxx11_support()
if(MSVC)     
	#关闭C4819警告
	add_definitions("/wd4819")		
	message(STATUS "optional:/wd4819")
	#关闭CRT_SECURE警告
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	message(STATUS "optional:-D_CRT_SECURE_NO_WARNINGS")	
endif(MSVC)
##############设置目标文件生成位置#####################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set( SUBDIRECTORIES common_source_cpp jni feature_se)
foreach( subdir ${SUBDIRECTORIES} )
	if( IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${subdir}" )
		MESSAGE( STATUS "Found sub project ${subdir}, adding it" )
		add_subdirectory( ${subdir} )
	endif()
endforeach( subdir )
# 复制安装脚本
if(WIN32)	
	install(FILES ${PROJECT_SOURCE_DIR}/INSTALL.bat.in DESTINATION bin RENAME INSTALL.bat)
	install(FILES ${PROJECT_SOURCE_DIR}/UNINSTALL.bat.in DESTINATION bin RENAME UNINSTALL.bat)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
	install(PROGRAMS ${PROJECT_SOURCE_DIR}/INSTALL.sh.in DESTINATION bin 
		PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
		RENAME INSTALL.sh
		)
	install(PROGRAMS ${PROJECT_SOURCE_DIR}/UNINSTALL.sh.in DESTINATION bin 
		PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
		RENAME UNINSTALL.sh
		)
endif()
