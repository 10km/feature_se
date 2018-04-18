# - Try to find CASSDK
#
# The following variables are optionally searched for defaults
#  FSE_ROOT_DIR:            Base directory where all FEATURE_SE components are found
#
# The following are set after configuration is done:
#定义CASSDK位置
set(CASSDK_ROOT_DIR "" CACHE PATH "Folder contains CASSDK")
if(WIN32 AND NOT CYGWIN)
  set (_CONFIG_INSTALL_DIR  cmake)
else()
  set (_CONFIG_INSTALL_DIR  lib/cmake)
endif()
if(CASSDK_ROOT_DIR)
  get_filename_component(cassdk_DIR "${CASSDK_ROOT_DIR}/${_CONFIG_INSTALL_DIR}" ABSOLUTE)
elseif(DEFINED ENV{CASSDK_HOME})
  # 使用环境变量CASSDK_HOME指定的位置
  get_filename_component(cassdk_DIR "$ENV{CASSDK_HOME}/${_CONFIG_INSTALL_DIR}" ABSOLUTE)
endif()
if(cassdk_DIR)
  message(STATUS "cassdk_DIR = ${cassdk_DIR}")
endif()
unset(_CONFIG_INSTALL_DIR)

find_package(cassdk CONFIG)