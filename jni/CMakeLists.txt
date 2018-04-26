cmake_minimum_required (VERSION 2.6)
find_package(JNI REQUIRED)
# 加入jni支持
include_directories(${JAVA_INCLUDE_PATH})
include_directories(${JAVA_INCLUDE_PATH2})
include_directories("${PROJECT_SOURCE_DIR}/feature_se")
#设置变量，表示所有的源文件列表  
set(SOURCE_FILES
    ${DEPENDENT_SOURCE_DIR}/jni_utilits.cpp
     net_gdface_sdk_fse_FseJniBridge.cpp
     jni_mgr.cpp
     BeanUtilits.cpp
     )
# CASSDK版本的JNI接口
add_library(fse_cas_jni SHARED ${SOURCE_FILES})
link_static_libstdcxx_if_linux(fse_cas_jni)
# 加入依赖库
if(CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(fse_cas_jni fse_cas_static)  
else()
  target_link_libraries(fse_cas_jni fse_cas_static_mt)
endif()
target_include_directories (fse_cas_jni	PRIVATE ${DEPENDENT_SOURCE_DIR})
#############设置动态库版本号###################
set_target_properties(fse_cas_jni PROPERTIES  VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR} DEBUG_POSTFIX _d)
message(STATUS "feature_se jni VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR}")

#######Windows 下DLL 文件名前缀为空 #######
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	set_property(TARGET fse_cas_jni PROPERTY  PREFIX "")
endif(CMAKE_SYSTEM_NAME MATCHES "Windows")
########安装脚本##########
install(TARGETS fse_cas_jni 
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        )
