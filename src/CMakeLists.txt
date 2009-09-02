# here are the include files of the charon-utils project
#INCLUDE_DIRECTORIES(${CHARON_UTILS_INCLUDE_DIRS})

#files for Charon library
SET(CHARON_FILES
    Derivatives/bin2.cpp
    Derivatives/Sobel.cpp
    DataAssemblers/nrange.cpp
    MotionModels/LocalPlanarity.cpp
    Fitters/LeastSquare.cpp
    MotionModels/LocalAffiness.cpp
    MotionModels/LocalRotation.cpp
    MotionModels/LocalStretch.cpp
    MotionModels/LocalConstant.cpp
    MotionEstimation.cpp
    BrightnessModels/Exponential.cpp
    BrightnessModels/Constant.cpp
    BrightnessModels/Diffusion.cpp
    BrightnessModels/MovingIllumination.cpp
    SequenceGeneration/SequenceGeneration.cpp
    functions.cpp
    Generate3d.cpp
    FlowFunctor.cpp
)

# create libraries
ADD_LIBRARY(${PROJECT_NAME} STATIC ${CHARON_FILES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} X11)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} charon-utils)



# install library
 INSTALL(
     TARGETS     ${PROJECT_NAME}
     EXPORT      ${PROJECT_NAME}
     RUNTIME     DESTINATION bin
     LIBRARY     DESTINATION lib${LIB_SUFFIX}
     ARCHIVE     DESTINATION lib${LIB_SUFFIX}
     COMPONENT   libraries
 )

 INSTALL(
     EXPORT       ${PROJECT_NAME}
     DESTINATION  include/${PROJECT_NAME}
     COMPONENT    libraries
 )
 
# install header files to include directory
 INSTALL(
     DIRECTORY   ${CMAKE_CURRENT_SOURCE_DIR}/
     DESTINATION include/${PROJECT_NAME}
     COMPONENT   headers
     FILES_MATCHING REGEX ".(h|hxx)$"
 )


