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
#判断编译类型和版本是否满足编译要求
if(MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 19)
	message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}") 
	message(STATUS "CMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}" )
elseif(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5)
	message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}") 
	message(STATUS "CMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}" )
else()
	message(FATAL_ERROR "compiler required: Visual Studio 2015 OR gcc 5.2.0" )
endif()
# includes utils.cmake module
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")
include (utils)
#定义公共代码位置   
set( COMMONS_SOURCE_FOLDER ${CMAKE_SOURCE_DIR}/common_source_cpp CACHE STRING "common source folder" FORCE)
if( NOT IS_DIRECTORY "${COMMONS_SOURCE_FOLDER}" )
	message( FATAL_ERROR "Found common source folder: ${COMMONS_SOURCE_FOLDER}, please locate it" )	
endif()
message( STATUS "COMMONS_SOURCE_FOLDER= ${COMMONS_SOURCE_FOLDER}" )
include_directories(${COMMONS_SOURCE_FOLDER})
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
set( SUBDIRECTORIES jni feature_se)
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