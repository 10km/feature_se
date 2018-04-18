#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gdface::fse_cas_static" for configuration "Debug"
set_property(TARGET gdface::fse_cas_static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(gdface::fse_cas_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/fse_cas_static_d.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS gdface::fse_cas_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_gdface::fse_cas_static "${_IMPORT_PREFIX}/lib/fse_cas_static_d.lib" )

# Import target "gdface::fse_cas" for configuration "Debug"
set_property(TARGET gdface::fse_cas APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(gdface::fse_cas PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/fse_cas_d.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/fse_cas_d.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS gdface::fse_cas )
list(APPEND _IMPORT_CHECK_FILES_FOR_gdface::fse_cas "${_IMPORT_PREFIX}/lib/fse_cas_d.lib" "${_IMPORT_PREFIX}/bin/fse_cas_d.dll" )

# Import target "gdface::fse_cas_static_mt" for configuration "Debug"
set_property(TARGET gdface::fse_cas_static_mt APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(gdface::fse_cas_static_mt PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/fse_cas_static_mt_d.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS gdface::fse_cas_static_mt )
list(APPEND _IMPORT_CHECK_FILES_FOR_gdface::fse_cas_static_mt "${_IMPORT_PREFIX}/lib/fse_cas_static_mt_d.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
