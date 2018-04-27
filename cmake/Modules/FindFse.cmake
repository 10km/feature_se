# - Try to find FEATURE_SE
#
# The following variables are optionally searched for defaults
#  FSE_ROOT_DIR:            Base directory where all FEATURE_SE components are found
#
# The following are set after configuration is done:
set(FSE_ROOT_DIR "" CACHE PATH "Folder contains feature_se")
if(FSE_ROOT_DIR)
	if(WIN32 AND NOT CYGWIN)
	  set (_CONFIG_INSTALL_DIR  cmake)
	else()
	  set (_CONFIG_INSTALL_DIR  lib/cmake/fse)
	endif()
  get_filename_component(Fse_DIR "${FSE_ROOT_DIR}/${_CONFIG_INSTALL_DIR}" ABSOLUTE)
  unset(_CONFIG_INSTALL_DIR)
endif(FSE_ROOT_DIR)
if(Fse_DIR)
  message(STATUS "Fse_DIR = ${Fse_DIR}")
endif()
find_package(Fse CONFIG)
