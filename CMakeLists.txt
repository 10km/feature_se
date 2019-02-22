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

# 设置外部提供人脸识别SDK类型,可选值CASSDK,CUSTOM,EUCLIDEAN
# 当设置为CUSTOM时，必须指定 CUSTOM_SDK_INCLUDE CUSTOM_SDK_LIBRARY
# 设置为EUCLIDEAN时，使用默认的点积比对函数
if(NOT EXT_SDK_TYPE)
	## 默认值CASSDK
	set(EXT_SDK_TYPE CASSDK)
	set(CASSDK_USED TRUE)
endif()

if(EXT_SDK_TYPE STREQUAL "CUSTOM")
	# CUSTOM_FEACOMP_HEADERS 比较函数头文件位置
	# CUSTOM_FEACOMP_LIBRARY 比较函数库文件位置
	# CUSTOM_FEACOMP_FUNNAME 比较函数名
	if(NOT CUSTOM_FEACOMP_INCLUDE OR NOT CUSTOM_FEACOMP_HEADERS OR NOT CUSTOM_FEACOMP_LIBRARY OR NOT CUSTOM_FEACOMP_FUNNAME)
		message(FATAL_ERROR "MUST DEFINE 
		CUSTOM_FEACOMP_INCLUDE(face sdk include folder)
		CUSTOM_FEACOMP_HEADERS(feature compare function's header file full path) 
		CUSTOM_FEACOMP_LIBRARY(feature compare function's library full path),
		CUSTOM_FEACOMP_FUNNAME(function name)")
	endif()	
endif()
if(EXT_SDK_TYPE STREQUAL "EUCLIDEAN")
	# EUCLIDEAN 使用系统内置的特征值比对函数,需要指定特征值数据类型和特征值长度
	# 特征值数据类型如果没指定，默认使用double
	# 特征值数据长度如果没指定，则报错退出
	if(NOT EUCLIDEAN_ELEM_TYPE)
		set(EUCLIDEAN_ELEM_TYPE double)
		message(STATUS "EUCLIDEAN_ELEM_TYPE use default:double,available value:(double|float)")
	endif()
	if(NOT EUCLIDEAN_ELEM_TYPE STREQUAL "double" OR NOT EUCLIDEAN_ELEM_TYPE STREQUAL "float")
		message(STATUS "invalid EUCLIDEAN_ELEM_TYPE: ${EUCLIDEAN_ELEM_TYPE},available value:(double|float)")
	endif()	
	# 默认特征值数组后面没有double类型的点积和
	if(NOT EUCLIDEAN_CODE_END_WITH_SUM)
		set(EUCLIDEAN_CODE_END_WITH_SUM OFF)
		message(STATUS "EUCLIDEAN_CODE_END_WITH_SUM: OFF")
	endif()	
	if(NOT EUCLIDEAN_ELEM_LEN)
		message(FATAL_ERROR "MUST DEFINE EUCLIDEAN_ELEM_LEN")
	endif()	
endif()
# 定义动态库名
if(NOT FSE_LIBNAME)
	set(FSE_LIBNAME fse_cas)
endif()
# 定义JNI动态库名
if(NOT JNI_FSE_LIBNAME)
	set(JNI_FSE_LIBNAME fse_cas_jni)
endif()
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
	if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5.0.0" )
		message(FATAL_ERROR "compiler required: gcc 5.0.0" )
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
