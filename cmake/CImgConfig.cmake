SET(CIMG_LINK_LIBRARIES)
SET(CIMG_INCLUDE_DIRS)

# disable graphical output
ADD_DEFINITIONS(-Dcimg_display=0)
ADD_DEFINITIONS(-Dcimg_verbosity=0)

# Lapack support
OPTION(USE_LAPACK "enable lapack support for CImg" ${UNIX})
IF(USE_LAPACK)
	FIND_PACKAGE(LAPACK)
	IF(LAPACK_FOUND)
		ADD_DEFINITIONS(-Dcimg_use_lapack)
		LIST(APPEND CIMG_LINK_LIBRARIES ${LAPACK_LIBRARIES})
	ELSE(LAPACK_FOUND)
		MESSAGE(SEND_ERROR "Lapack cannot be used, please disable USE_LAPACK")
	ENDIF(LAPACK_FOUND)
ENDIF(USE_LAPACK)

# OpenMP support
FIND_PACKAGE(OpenMP QUIET)
IF(OPENMP_FOUND)
	# deactivate openmp in windows by default as it is not
	# supported in Visual Studio Express and will drop you
	# into dll hell
	IF(MSVC)
		OPTION(USE_OPENMP "use OpenMP" OFF)
	ELSE(MSVC)
		OPTION(USE_OPENMP "use OpenMP" ON)
	ENDIF(MSVC)
ENDIF(OPENMP_FOUND)
IF(USE_OPENMP)
	ADD_DEFINITIONS(-Dcimg_use_openmp ${OpenMP_CXX_FLAGS})
	IF(CMAKE_COMPILER_IS_GNUCXX)
		LIST(APPEND CIMG_LINK_LIBRARIES gomp)
	ENDIF(CMAKE_COMPILER_IS_GNUCXX)
ENDIF(USE_OPENMP)

# PNG support
FIND_PACKAGE(PNG)
IF(PNG_FOUND)
	OPTION(USE_PNG "Use png libraries" ON)
	IF(USE_PNG)
		ADD_DEFINITIONS(-Dcimg_use_png)
		LIST(APPEND CIMG_LINK_LIBRARIES ${PNG_LIBRARIES})
		LIST(APPEND CIMG_INCLUDE_DIRS ${PNG_INCLUDE_DIR})
	ENDIF(USE_PNG)
ENDIF(PNG_FOUND)

# support of further libraries not needed in charon-flow
# (PNG is only needed for testig only)

MACRO(CimgLibInfo)
	MESSAGE(STATUS "    Lapack       : ${USE_LAPACK}")
	MESSAGE(STATUS "    OpenMP       : ${USE_OPENMP}")
	MESSAGE(STATUS "    PNG          : ${USE_PNG}")
ENDMACRO()
