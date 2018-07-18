find_path(CONFIG++_INCLUDE_DIR libconfig.h++ /usr/include /usr/local/include)

find_library(CONFIG++_LIBRARY NAMES config++ PATH /usr/lib /usr/local/lib) 

if (CONFIG++_INCLUDE_DIR AND CONFIG++_LIBRARY)
    SET(CONFIG++_FOUND TRUE)
endif ( CONFIG++_INCLUDE_DIR AND CONFIG++_LIBRARY)

if (CONFIG++_FOUND)
    if (NOT CONFIG++_FIND_QUIETLY)
	message(STATUS "Found Config++: ${CONFIG++_LIBRARY}")
    endif (NOT  CONFIG++_FIND_QUIETLY)
else(NOT CONFIG++_FOUND)
	if (Config++_FIND_REQUIRED)
		if(NOT CONFIG++_INCLUDE_DIR)
			message(FATAL_ERROR "Could not find LibConfig++ header file!")
		endif(NOT CONFIG++_INCLUDE_DIR)
		if(NOT CONFIG++_LIBRARY)
			message(FATAL_ERROR "Could not find LibConfig++ library file!")
		endif(NOT CONFIG++_LIBRARY)
	endif (Config++_FIND_REQUIRED)
endif (CONFIG++_FOUND)