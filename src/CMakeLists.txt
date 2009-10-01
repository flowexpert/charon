INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(PixelSelections)
INCLUDE_DIRECTORIES(Incrementors)
INCLUDE_DIRECTORIES(ObjectiveFunctions)
INCLUDE_DIRECTORIES(SurfaceAnalysis)

IF(COMPILE_SOLVERS AND PETSC_USED)
	INCLUDE_DIRECTORIES(BEFORE ${PETSC_INCLUDE_DIRS})
	IF(!PETSC_USES_MPI)
		INCLUDE_DIRECTORIES(BEFORE ${PETSC_ROOT_DIR}/include/mpiuni/)		
	ENDIF(!PETSC_USES_MPI)
ENDIF(COMPILE_SOLVERS AND PETSC_USED)

ADD_DEFINITIONS(/DHANDLE_DLL)

#Create shared libraries
ADD_LIBRARY(channelconverter SHARED ChannelConverter.cpp)

IF (COMPILE_DERIVATIVES)
	ADD_LIBRARY(bin2 SHARED Derivatives/bin2.cpp)
	ADD_LIBRARY(sobel SHARED Derivatives/Sobel.cpp)
ENDIF (COMPILE_DERIVATIVES)
IF (COMPILE_STENCILS)
	ADD_LIBRARY(gbcce SHARED Stencils/Gbcce.cpp)
	ADD_LIBRARY(l2norm SHARED Stencils/L2Norm.cpp)
ENDIF (COMPILE_STENCILS)
IF (COMPILE_SOLVERS)
	IF (PETSC_USED)
		ADD_LIBRARY(petscsolver SHARED Solvers/PetscSolver.cpp)
		#add blas/lapack of petsc
		IF (PETSC_USES_FORTRAN)
			TARGET_LINK_LIBRARIES(petscsolver	${PETSC_ROOT_DIR}/externalpackages/fblaslapack/${PETSC_ARCH}/libfblas.${LIBFILE_EXT}
								${PETSC_ROOT_DIR}/externalpackages/fblaslapack/${PETSC_ARCH}/libflapack.${LIBFILE_EXT})
		ELSE (PETSC_USES_FORTRAN)
			TARGET_LINK_LIBRARIES(petscsolver	${PETSC_ROOT_DIR}/externalpackages/f2cblaslapack/${PETSC_ARCH}/libf2cblas.${LIBFILE_EXT}
								${PETSC_ROOT_DIR}/externalpackages/f2cblaslapack/${PETSC_ARCH}/libf2clapack.${LIBFILE_EXT})
		ENDIF (PETSC_USES_FORTRAN)

		#add MPI support
		IF (PETSC_USES_MPI)
			TARGET_LINK_LIBRARIES(petscsolver	${MPI_DIR}/lib/mpi.${LIBFILE_EXT}) #TODO: correct this
		ELSE (PETSC_USES_MPI)
			TARGET_LINK_LIBRARIES(petscsolver	${PETSC_ROOT_DIR}/lib/${PETSC_ARCH}/libmpiuni.${LIBFILE_EXT})
		ENDIF (PETSC_USES_MPI)

		#add all libraries needed by petsc
		TARGET_LINK_LIBRARIES(petscsolver	charon-core
							${PETSC_ROOT_DIR}/lib/${PETSC_ARCH}/libpetsc.${LIBFILE_EXT}
							# ${PETSC_ROOT_DIR}/lib/${PETSC_ARCH}/libpetsccontrib.${LIBFILE_EXT}
							${PETSC_ROOT_DIR}/lib/${PETSC_ARCH}/libpetscdm.${LIBFILE_EXT}
							${PETSC_ROOT_DIR}/lib/${PETSC_ARCH}/libpetscksp.${LIBFILE_EXT}
							${PETSC_ROOT_DIR}/lib/${PETSC_ARCH}/libpetscmat.${LIBFILE_EXT}
							# ${PETSC_ROOT_DIR}/lib/${PETSC_ARCH}/libpetscsnes.${LIBFILE_EXT}
							# ${PETSC_ROOT_DIR}/lib/${PETSC_ARCH}/libpetscts.${LIBFILE_EXT}
							${PETSC_ROOT_DIR}/lib/${PETSC_ARCH}/libpetscvec.${LIBFILE_EXT})
	ENDIF (PETSC_USED)
ENDIF (COMPILE_SOLVERS)
IF (COMPILE_BLOCKMATCHING)
	ADD_LIBRARY(incrementorparameter SHARED IncrementorParameter.cpp)
	ADD_LIBRARY(incrementorcountup SHARED Incrementors/IncrementorCountUp.cpp)
	TARGET_LINK_LIBRARIES(incrementorcountup incrementorparameter)
	ADD_LIBRARY(objectivefunctioncomparing SHARED ObjectiveFunctions/ObjectiveFunctionComparing.cpp)
	ADD_LIBRARY(listedpixelselection SHARED PixelSelections/ListedPixelSelection.cpp)
	TARGET_LINK_LIBRARIES(listedpixelselection roi)
	ADD_LIBRARY(surfaceanalysisminchange SHARED SurfaceAnalysis/SurfaceAnalysisMinChange.cpp)
	TARGET_LINK_LIBRARIES(surfaceanalysisminchange incrementorparameter)
	ADD_LIBRARY(blockmatchingliacs SHARED BlockMatching/BlockMatchingLIACS.cpp)
	TARGET_LINK_LIBRARIES(blockmatchingliacs incrementorcountup)
ENDIF (COMPILE_BLOCKMATCHING)
IF (COMPILE_BRIGHTNESS)
	ADD_LIBRARY(brightnessmodels_constant SHARED BrightnessModels/Constant.cpp)
#	ADD_LIBRARY(brightnessmodels_diffusion SHARED BrightnessModels/Diffusion.cpp)
#	ADD_LIBRARY(brightnessmodels_exponential SHARED BrightnessModels/Exponential.cpp)
#	ADD_LIBRARY(brightnessmodels_movingillumination SHARED BrightnessModels/MovingIllumination.cpp)
ENDIF (COMPILE_BRIGHTNESS)
IF (COMPILE_MOTION)
#	ADD_LIBRARY(motionmodels_localaffiness SHARED MotionModels/LocalAffiness.cpp FlowFunctor.cpp)
	ADD_LIBRARY(motionmodels_localconstant SHARED MotionModels/LocalConstant.cpp FlowFunctor.cpp)
#	ADD_LIBRARY(motionmodels_localplanarity SHARED MotionModels/LocalPlanarity.cpp FlowFunctor.cpp)
#	ADD_LIBRARY(motionmodels_localrotation SHARED MotionModels/LocalRotation.cpp FlowFunctor.cpp)
#	ADD_LIBRARY(motionmodels_localstretch SHARED MotionModels/LocalStretch.cpp FlowFunctor.cpp)
ENDIF (COMPILE_MOTION)

SET(Plugins
	channelconverter
)

IF (COMPILE_DERIVATIVES)
	LIST(APPEND Plugins
		bin2
		sobel
	)
ENDIF (COMPILE_DERIVATIVES)

IF (COMPILE_STENCILS)
	LIST(APPEND Plugins
		gbcce
		l2norm
	)
ENDIF (COMPILE_STENCILS)

IF (COMPILE_SOLVERS AND PETSC_USED)
	LIST(APPEND Plugins
		petscsolver
	)
ENDIF (COMPILE_SOLVERS AND PETSC_USED)

IF (COMPILE_BLOCKMATCHING)
	LIST(APPEND Plugins
		incrementorparameter	
		incrementorcountup
		objectivefunctioncomparing
		listedpixelselection
		surfaceanalysisminchange
		blockmatchingliacs
	)
ENDIF (COMPILE_BLOCKMATCHING)

IF (COMPILE_BRIGHTNESS)
	LIST(APPEND Plugins
		brightnessmodels_constant
#		brightnessmodels_diffusion
#		brightnessmodels_exponential
#		brightnessmodels_movingillumination
	)
ENDIF (COMPILE_BRIGHTNESS)

IF (COMPILE_MOTION)
	LIST(APPEND Plugins
#		motionmodels_localaffiness
		motionmodels_localconstant
#		motionmodels_localplanarity 
#		motionmodels_localrotation
#		motionmodels_localstretch
	)
ENDIF (COMPILE_MOTION)

FOREACH(X ${Plugins})
	TARGET_LINK_LIBRARIES(${X} charon-core)
	IF(UNIX)
		TARGET_LINK_LIBRARIES(${X} pthread ${X11_LIBRARIES})
	ENDIF(UNIX)
	IF(WIN32)
		TARGET_LINK_LIBRARIES(${X} gdi32)
	ENDIF(WIN32)
ENDFOREACH(X)

# install libraries/plugins
IF(NOT ${CMAKE_INSTALL_PREFIX} STREQUAL ${CHARON_UTILS_ROOT_DIR})
	MESSAGE(STATUS	"Warning: Plugins will NOT be installed in standart "
					"plugin directory!")
	MESSAGE(STATUS	"Set CMAKE_INSTALL_PREFIX to ${CHARON_UTILS_ROOT_DIR} "
					"to install into global plugin path.")
ENDIF(NOT ${CMAKE_INSTALL_PREFIX} STREQUAL ${CHARON_UTILS_ROOT_DIR})
SET_TARGET_PROPERTIES(${Plugins}
	PROPERTIES INSTALL_RPATH
	${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/charon-plugins:${CHARON_CORE_ROOT_DIR}/lib${LIB_SUFFIX}
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
FILE(GLOB_RECURSE HEADER_FILES
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
