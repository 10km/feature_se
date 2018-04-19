#cmake file for test fse
#author:guyadong
#created:2018/04/16
cmake_minimum_required( VERSION 3.0 )

cmake_policy(SET CMP0048 NEW)
project( testfse VERSION 1.0.0 LANGUAGES C CXX)
# ---[ Using cmake scripts and modules
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(Fse REQUIRED)
include (utils)
cxx11_support()
##############设置目标文件生成位置#####################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
if(MSVC)     
	#关闭C4819警告
	add_definitions("/wd4819")		
	message(STATUS "optional:/wd4819")
endif(MSVC)
# 定义所有的源文件列表  
set(_CPP_SOURCE_FILES
    test.cpp 
    )
set(_C_SOURCE_FILES
    test_c.c
    )
# C++调用测试
add_executable(test_fse ${_CPP_SOURCE_FILES})
add_executable(test_fse_static ${_CPP_SOURCE_FILES})
target_link_libraries(test_fse gdface::fse_cas)
target_link_libraries(test_fse_static gdface::fse_cas_static)
if(MSVC) 
	add_executable(test_fse_static_mt ${_CPP_SOURCE_FILES})
  target_link_libraries(test_fse_static_mt gdface::fse_cas_static_mt)
endif()

# 标准C调用测试
add_executable(test_fse_c ${_C_SOURCE_FILES})
add_executable(test_fse_c_static ${_C_SOURCE_FILES})

target_link_libraries(test_fse_c gdface::fse_cas)
target_link_libraries(test_fse_c_static gdface::fse_cas_static)
if(MSVC)
  add_executable(test_fse_c_static_mt ${_C_SOURCE_FILES})
  target_link_libraries(test_fse_c_static_mt gdface::fse_cas_static_mt)
endif()

# Cleanup temporary variables.
set(_CPP_SOURCE_FILES)
set(_C_SOURCE_FILES)
