#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gdface::fse_cas_static" for configuration "RelWithDebInfo"
set_property(TARGET gdface::fse_cas_static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(gdface::fse_cas_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/fse_cas_static.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS gdface::fse_cas_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_gdface::fse_cas_static "${_IMPORT_PREFIX}/lib/fse_cas_static.lib" )

# Import target "gdface::fse_cas" for configuration "RelWithDebInfo"
set_property(TARGET gdface::fse_cas APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(gdface::fse_cas PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/fse_cas.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/fse_cas.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS gdface::fse_cas )
list(APPEND _IMPORT_CHECK_FILES_FOR_gdface::fse_cas "${_IMPORT_PREFIX}/lib/fse_cas.lib" "${_IMPORT_PREFIX}/bin/fse_cas.dll" )

# Import target "gdface::fse_cas_static_mt" for configuration "RelWithDebInfo"
set_property(TARGET gdface::fse_cas_static_mt APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(gdface::fse_cas_static_mt PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/fse_cas_static_mt.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS gdface::fse_cas_static_mt )
list(APPEND _IMPORT_CHECK_FILES_FOR_gdface::fse_cas_static_mt "${_IMPORT_PREFIX}/lib/fse_cas_static_mt.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
