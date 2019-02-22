cmake_minimum_required (VERSION 2.6)
# We are only interested in finding jni.h: we do not care about extended JVM
# functionality or the AWT library.
set(JAVA_AWT_LIBRARY NotNeeded)
set(JAVA_JVM_LIBRARY NotNeeded)
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
	# android 平台没有jni_md.h
	set(JAVA_INCLUDE_PATH2 NotNeeded)
endif()

set(JAVA_AWT_INCLUDE_PATH NotNeeded)
find_package(JNI REQUIRED)
message(STATUS JAVA_INCLUDE_PATH=${JAVA_INCLUDE_PATH})

# 加入jni支持
include_directories(${JAVA_INCLUDE_PATH})
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Android")
	include_directories(${JAVA_INCLUDE_PATH2})
endif()

include_directories("${PROJECT_SOURCE_DIR}/feature_se")
#设置变量，表示所有的源文件列表  
set(SOURCE_FILES
    ${DEPENDENT_SOURCE_DIR}/jni_utilits.cpp
     net_gdface_sdk_fse_FseJniBridge.cpp
     jni_mgr.cpp
     BeanUtilits.cpp
     )
# CASSDK版本的JNI接口
add_library(${JNI_FSE_LIBNAME} SHARED ${SOURCE_FILES})
link_static_libstdcxx_if_linux(${JNI_FSE_LIBNAME})
# 加入依赖库 MSVC编译时使用/MT静态库
target_link_libraries(${JNI_FSE_LIBNAME} fse_cas_static$<$<CXX_COMPILER_ID:MSVC>:_mt>)

target_include_directories (${JNI_FSE_LIBNAME}	PRIVATE ${DEPENDENT_SOURCE_DIR})
#############设置动态库版本号###################
set_target_properties(${JNI_FSE_LIBNAME} PROPERTIES  VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR} DEBUG_POSTFIX _d)
message(STATUS "feature_se jni VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR}")

#######Windows 下DLL 文件名前缀为空 #######
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set_property(TARGET ${JNI_FSE_LIBNAME} PROPERTY  PREFIX "")
endif(CMAKE_SYSTEM_NAME MATCHES "Windows")
########安装脚本##########
install(TARGETS ${JNI_FSE_LIBNAME} 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        )
if(MSVC)
	# 安装pdb文件
	install(FILES $<TARGET_PDB_FILE:${JNI_FSE_LIBNAME}> DESTINATION bin OPTIONAL)
endif(MSVC)
