INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

ADD_DEFINITIONS(/DHANDLE_DLL)

#Create shared libraries
ADD_LIBRARY(gbcce SHARED Stencils/GBCCE.cpp)
ADD_LIBRARY(l2norm SHARED Stencils/L2Norm.cpp)
# ADD_LIBRARY(petscsolver SHARED Solvers/PetscSolver.cpp)

SET(Plugins
	gbcce
	l2norm
#	petscsolver
)

FOREACH(X ${Plugins})
	TARGET_LINK_LIBRARIES(${X} charon-core)
	IF(UNIX)
		TARGET_LINK_LIBRARIES(${X} pthread ${X11_LIBRARIES})
	ENDIF(UNIX)
	IF(WIN32)
		TARGET_LINK_LIBRARIES(${X} gdi32)
	ENDIF(WIN32)
ENDFOREACH(X)

# install library
SET_TARGET_PROPERTIES(${Plugins}
	PROPERTIES INSTALL_RPATH
	${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins
)
INSTALL(
    TARGETS      	${Plugins}
    EXPORT      	${PROJECT_NAME}
    RUNTIME      	DESTINATION bin
    LIBRARY      	DESTINATION lib${LIB_SUFFIX}/charon-plugins
    ARCHIVE      	DESTINATION lib${LIB_SUFFIX}/charon-plugins
    COMPONENT    	libraries
)

# install header files to include directory
FILE(GLOB HEADER_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.h *.hxx
)
INSTALL(
    FILES       ${HEADER_FILES}
    DESTINATION include/${PROJECT_NAME}
    COMPONENT   headers
)


INSTALL(
    EXPORT       ${PROJECT_NAME}
    DESTINATION  include/${PROJECT_NAME}
    COMPONENT    libraries
)
