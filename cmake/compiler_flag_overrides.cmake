if(MSVC)     
   option(WITH_STATIC_CRT
    "Link all libraries and executables with the static C run-time library (libcmt*.lib)instead of the C run-time DLL (msvcr*.dll)"
    FALSE)
  if(WITH_STATIC_CRT)
	  # Use the static C library for all build types
	  message(STATUS "Link to static C and C++ runtime lirbary(/MT /MTd)")

	  foreach(var 
			CMAKE_C_FLAGS_DEBUG_INIT 
			CMAKE_C_FLAGS_RELEASE_INIT
			CMAKE_C_FLAGS_MINSIZEREL_INIT 
			CMAKE_C_FLAGS_RELWITHDEBINFO_INIT
			CMAKE_CXX_FLAGS_DEBUG_INIT 
			CMAKE_CXX_FLAGS_RELEASE_INIT
			CMAKE_CXX_FLAGS_MINSIZEREL_INIT 
			CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT
	    )
	    set( has_replaced off)
	    if(${var} MATCHES "/MD")
	      string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}")
	      set( has_replaced on)
	    endif()
	    if( has_replaced ) 
	    	MESSAGE(STATUS  "${var}:${${var}}")
			endif( has_replaced )
			unset( has_replaced )
	  endforeach()
  endif(WITH_STATIC_CRT)
  # for detail,see https://msdn.microsoft.com/zh-cn/library/958x11bc.aspx
  option(WITH_DEBUG_Z7 
  	"use /Z7 option to produces an .obj file containing full symbolic debugging information for use with the debugger" 
  	FALSE)
  if(WITH_DEBUG_Z7)
	  string(REGEX REPLACE "/Z[iI]" "/Z7" CMAKE_C_FLAGS_DEBUG_INIT "${CMAKE_C_FLAGS_DEBUG_INIT}")
	  string(REGEX REPLACE "/Z[iI]" "/Z7" CMAKE_CXX_FLAGS_DEBUG_INIT "${CMAKE_CXX_FLAGS_DEBUG_INIT}")
	  MESSAGE(STATUS "CMAKE_C_FLAGS_DEBUG_INIT=${CMAKE_C_FLAGS_DEBUG_INIT}")
	  MESSAGE(STATUS "CMAKE_CXX_FLAGS_DEBUG_INIT=${CMAKE_CXX_FLAGS_DEBUG_INIT}")
	endif()
endif(MSVC)
