# Test suite of application
INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/PixelSelections)
IF(COMPILE_SOLVERS)
	INCLUDE_DIRECTORIES(BEFORE ${PETSC_INCLUDE_DIRS})
ENDIF(COMPILE_SOLVERS)

ADD_DEFINITIONS(-DPENGUINFILE="${CMAKE_CURRENT_SOURCE_DIR}/Penguin.cimg")
ADD_DEFINITIONS(-DPENGUINHEADFILE="${CMAKE_CURRENT_SOURCE_DIR}/sequence.cimg")

IF(ENABLE_TEST)
	IF(COMPILE_BLOCKMATCHING)
		# tests for imgmanip classes
		ADD_EXECUTABLE(listedpixelselectiontest listedpixelselectiontest.cpp)
		TARGET_LINK_LIBRARIES(listedpixelselectiontest charon-core roi filereader)
		IF(UNIX)
			TARGET_LINK_LIBRARIES(listedpixelselectiontest pthread ${X11_LIBRARIES})
		ENDIF(UNIX)
	ENDIF(COMPILE_BLOCKMATCHING)
	IF(COMPILE_SOLVERS)
		# tests for solver classes
		ADD_EXECUTABLE(petscsolvertest PetscSolverTest.cpp)
		TARGET_LINK_LIBRARIES(petscsolvertest
		                      charon-core
		                      roi
		                      gbcce l2norm
		                      brightnessmodels_constant
		                      motionmodels_localconstant
		                      bin2
		                      channelconverter
		                      filereader
		                      filewriter)
		IF(UNIX)
			TARGET_LINK_LIBRARIES(petscsolvertest pthread ${X11_LIBRARIES})
		ENDIF(UNIX)
	ENDIF(COMPILE_SOLVERS)
ENDIF(ENABLE_TEST)