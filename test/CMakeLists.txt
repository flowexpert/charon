# Test suite of application
IF(BUILD_TESTING)
	INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)
	INCLUDE_DIRECTORIES(${charon-core_INCLUDE_DIRS})
	INCLUDE_DIRECTORIES(${charon-utils_INCLUDE_DIRS})

	ADD_DEFINITIONS(-DTESTDIR="${CMAKE_CURRENT_SOURCE_DIR}")
	GET_TARGET_PROPERTY(Roi_LOC roi LOCATION)
	GET_FILENAME_COMPONENT(GLOBAL_PLUGIN_DIR ${Roi_LOC} PATH)
	GET_TARGET_PROPERTY(Diff2d_LOC diff2d LOCATION)
	GET_FILENAME_COMPONENT(LOCAL_PLUGIN_DIR ${Diff2d_LOC} PATH)
	ADD_DEFINITIONS(-DGLOBAL_PLUGIN_DIR="${GLOBAL_PLUGIN_DIR}")
	ADD_DEFINITIONS(-DLOCAL_PLUGIN_DIR="${LOCAL_PLUGIN_DIR}")

	INCLUDE_DIRECTORIES(BEFORE ${PETSC_INCLUDE_DIRS})
	# test petsc initialization
	ADD_EXECUTABLE(petscinittest PetscInitializationTest.cpp)
	IF(MSVC)
		SET_TARGET_PROPERTIES(petscinittest
			PROPERTIES LINK_FLAGS
				/NODEFAULTLIB:"LIBCMT"
		)
	ENDIF(MSVC)
	TARGET_LINK_LIBRARIES(petscinittest ${PETSC_LIBRARIES})
	ADD_TEST(
		NAME PetscInitialization
		COMMAND petscinittest
	)

	# checking creation of petsc plugins
	ADD_EXECUTABLE(petscplugintest PetscPluginTest.cpp)
	IF(MSVC)
		SET_TARGET_PROPERTIES(petscplugintest
			PROPERTIES LINK_FLAGS
				/NODEFAULTLIB:"LIBCMT"
		)
	ENDIF(MSVC)
	TARGET_LINK_LIBRARIES(petscplugintest
		charon-core
		${CIMG_LINK_LIBRARIES}
		roi
		gbcce
		l2norm
		brightnessmodels_constant
		motionmodels_localconstant
		channelconverter
		filereader
		filewriter
		mask1d
		linearfilter
		petscsolver
	)
	ADD_TEST(
		NAME PetscPlugin
		COMMAND petscplugintest
	)

	IF(UNIX)
		# check loading and unloading petsc shared library
		# this is known to fail e.g. using the ubuntu package
		# petsc 3.0.0.dfsg-6ubuntu1
		# use a more recent version in this case,
		# e.g. from the charon-suite ppa (see
		# https://launchpad.net/~jmgottfried/+archive/charon)
		ADD_LIBRARY(petsctestdll SHARED PetscTestDLL.cpp)
		TARGET_LINK_LIBRARIES(petsctestdll
			${PETSC_LIBRARIES}
		)
		ADD_EXECUTABLE(petsctestdllload PetscTestDLLLoad.cpp)
		TARGET_LINK_LIBRARIES(petsctestdllload
			dl
		)
		ADD_TEST(
			NAME Pets_Load_SharedLibrary
			COMMAND petsctestdllload
		)
	ENDIF(UNIX)

	# check loading and unloading petsc plugin
	ADD_EXECUTABLE(petscsolverdlltest PetscSolverDLLTest.cpp)
	TARGET_LINK_LIBRARIES(petscsolverdlltest
		charon-core
		charon-plugins
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME PetscSolver_DLL
		COMMAND petscsolverdlltest
	)

	# executing test workflow
	ADD_EXECUTABLE(petscsolvertest PetscSolverTest.cpp)
	TARGET_LINK_LIBRARIES(petscsolvertest
		charon-core
		charon-plugins
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME Petsc_HornSchunck_Sinus
		COMMAND petscsolvertest
	)

	# testing Horn&Schunck Method on Yosemite Sequence
	ADD_EXECUTABLE(hornschunckyosemite HornSchunckYosemite.cpp)
	TARGET_LINK_LIBRARIES(hornschunckyosemite
		flowcomparator
		charon-plugins
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME Petsc_HornSchunck_Yosemite
		COMMAND hornschunckyosemite
	)

	# testing Horn&Schunk without plugin usage to simplify profiling
	ADD_EXECUTABLE(hornschuncknoplugins
		HornSchunckYosemiteNoPlugins.cpp
		../src/Diff2D.cpp
	)
	TARGET_LINK_LIBRARIES(hornschuncknoplugins
		charon-core
		${PETSC_LIBRARIES}
		${CIMG_LINK_LIBRARIES}
	)
	ADD_TEST(
		NAME Petsc_HornSchunck_Yosemite_NoPlugins
		COMMAND hornschuncknoplugins
	)

	IF(WIN32)
		# copy needed dll for tests and application run before installing
		GET_TARGET_PROPERTY(CLOC charon-core LOCATION)
		GET_TARGET_PROPERTY(Roi_LOC roi LOCATION)
		GET_TARGET_PROPERTY(Filereader_LOC filereader LOCATION)
		GET_TARGET_PROPERTY(Filewriter_LOC filewriter LOCATION)
		GET_TARGET_PROPERTY(Mask1d_LOC mask1d LOCATION)
		GET_TARGET_PROPERTY(LinearFilter_LOC linearfilter LOCATION)
		GET_TARGET_PROPERTY(ChannelConverter_LOC channelconverter LOCATION)
		GET_TARGET_PROPERTY(InterpolatorLinear_LOC interpolatorlinear LOCATION)
		GET_TARGET_PROPERTY(Crop_LOC crop LOCATION)
		ADD_CUSTOM_COMMAND(TARGET POST_BUILD hornschunckyosemite
			COMMAND ${CMAKE_COMMAND} -E copy "${CLOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${Roi_LOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${Filereader_LOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${Filewriter_LOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${LinearFilter_LOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${ChannelConverter_LOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${Mask1d_LOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${Crop_LOC}" "${LOCAL_PLUGIN_DIR}"
			COMMAND ${CMAKE_COMMAND} -E copy "${InterpolatorLinear_LOC}" "${LOCAL_PLUGIN_DIR}"
		)
	ENDIF(WIN32)
ENDIF(BUILD_TESTING)
