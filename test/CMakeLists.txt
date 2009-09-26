# Test suite of application
INCLUDE_DIRECTORIES(${CHARON_CORE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/PixelSelections)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/Incrementors)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/ObjectiveFunctions)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/SurfaceAnalysis)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/BrightnessModels)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/MotionModels)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/Derivatives)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/build/src)

IF(COMPILE_SOLVERS)
	INCLUDE_DIRECTORIES(BEFORE ${PETSC_INCLUDE_DIRS})
ENDIF(COMPILE_SOLVERS)

ADD_DEFINITIONS(-DPENGUINFILE="${CMAKE_CURRENT_SOURCE_DIR}/Penguin.cimg")
ADD_DEFINITIONS(-DPENGUINHEADFILE="${CMAKE_CURRENT_SOURCE_DIR}/sequence.cimg")
ADD_DEFINITIONS(-DTESTFILE="${CMAKE_CURRENT_SOURCE_DIR}/test.cimg")

IF(ENABLE_TEST)
	IF(COMPILE_BLOCKMATCHING)
		# tests for imgmanip classes
		ADD_EXECUTABLE(listedpixelselectiontest listedpixelselectiontest.cpp)
		ADD_EXECUTABLE(incrementorcountuptest incrementorcountuptest.cpp)
		ADD_EXECUTABLE(objectivefunctioncomparingtest objectivefunctioncomparingtest.cpp)
		ADD_EXECUTABLE(surfaceanalysisminchangetest surfaceanalysisminchnagetest.cpp)
		TARGET_LINK_LIBRARIES(listedpixelselectiontest charon-core roi filereader)
		TARGET_LINK_LIBRARIES(incrementorcountuptest 
		                      charon-core 
		                      incrementorparameter)
		TARGET_LINK_LIBRARIES(objectivefunctioncomparingtest 
		                      charon-core 
		                      roi 
		                      filereader 
		                      incrementorparameter 
		                      brightnessmodels_constant
		                      motionmodels_localconstant 
		                      listedpixelselection 
		                      sobel 
		                      interpolatorlinear)
		TARGET_LINK_LIBRARIES(surfaceanalysisminchangetest charon-core incrementorparameter)
		IF(UNIX)
			TARGET_LINK_LIBRARIES(listedpixelselectiontest pthread ${X11_LIBRARIES})
			TARGET_LINK_LIBRARIES(incrementorcountuptest pthread ${X11_LIBRARIES})
			TARGET_LINK_LIBRARIES(objectivefunctioncomparingtest pthread ${X11_LIBRARIES})
			TARGET_LINK_LIBRARIES(surfaceanalysisminchangetest pthread ${X11_LIBRARIES})
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
