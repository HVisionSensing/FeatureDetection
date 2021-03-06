SET( SUBPROJECT_NAME Classification ) # my own variable, not cmake's
PROJECT( ${SUBPROJECT_NAME} )
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET( ${SUBPROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${SUBPROJECT_NAME}_VERSION_MINOR 1 )

MESSAGE(STATUS "=== Configuring ${SUBPROJECT_NAME} ===")

# find dependencies
FIND_PACKAGE(Boost 1.48.0 REQUIRED)
FIND_PACKAGE(OpenCV 2.4.3 REQUIRED core)
MESSAGE(STATUS "OpenCV include dir found at ${OpenCV_INCLUDE_DIRS}")
MESSAGE(STATUS "OpenCV lib dir found at ${OpenCV_LIB_DIR}")
#Matlab:
SET( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )
FIND_PACKAGE( MATLAB REQUIRED )
IF(MATLAB_FOUND)
  MESSAGE(STATUS "Matlab found at ${MATLAB_INCLUDE_DIR}")
ELSE(MATLAB_FOUND)
  MESSAGE(FATAL_ERROR "Matlab not found")
ENDIF(MATLAB_FOUND)


# source and header files
SET(HEADERS
	include/classification/AgeBasedExampleManagement.hpp
	include/classification/BinaryClassifier.hpp
	include/classification/ConfidenceBasedExampleManagement.hpp
	include/classification/EmptyExampleManagement.hpp
	include/classification/ExampleManagement.hpp
	include/classification/FixedTrainableProbabilisticSvmClassifier.hpp
	include/classification/FrameBasedExampleManagement.hpp
	include/classification/HistogramIntersectionKernel.hpp
	include/classification/IImg.hpp
	include/classification/Kernel.hpp
	include/classification/KernelVisitor.hpp
	include/classification/LinearKernel.hpp
	include/classification/PolynomialKernel.hpp
	include/classification/ProbabilisticClassifier.hpp
	include/classification/ProbabilisticRvmClassifier.hpp
	include/classification/ProbabilisticSvmClassifier.hpp
	include/classification/ProbabilisticTwoStageClassifier.hpp
	include/classification/ProbabilisticWvmClassifier.hpp
	include/classification/RbfKernel.hpp
	include/classification/RvmClassifier.hpp
	include/classification/SvmClassifier.hpp
	include/classification/TrainableBinaryClassifier.hpp
	include/classification/TrainableClassifier.hpp
	include/classification/TrainableProbabilisticClassifier.hpp
	include/classification/TrainableProbabilisticSvmClassifier.hpp
	include/classification/TrainableProbabilisticTwoStageClassifier.hpp
	include/classification/TrainableSvmClassifier.hpp
	include/classification/UnlimitedExampleManagement.hpp
	include/classification/VectorBasedExampleManagement.hpp
	include/classification/VectorMachineClassifier.hpp
	include/classification/WvmClassifier.hpp
)
SET(SOURCE
	src/classification/AgeBasedExampleManagement.cpp
	src/classification/ConfidenceBasedExampleManagement.cpp
	src/classification/FrameBasedExampleManagement.cpp
	src/classification/IImg.cpp
	src/classification/ProbabilisticRvmClassifier.cpp
	src/classification/ProbabilisticSvmClassifier.cpp
	src/classification/ProbabilisticTwoStageClassifier.cpp
	src/classification/ProbabilisticWvmClassifier.cpp
	src/classification/RvmClassifier.cpp
	src/classification/SvmClassifier.cpp
	src/classification/TrainableProbabilisticSvmClassifier.cpp
	src/classification/TrainableSvmClassifier.cpp
	src/classification/UnlimitedExampleManagement.cpp
	src/classification/VectorBasedExampleManagement.cpp
	src/classification/VectorMachineClassifier.cpp
	src/classification/WvmClassifier.cpp
)

include_directories( "include" )

# add dependencies
include_directories( ${Logging_SOURCE_DIR}/include )
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${MATLAB_INCLUDE_DIR} )

# make library
add_library( ${SUBPROJECT_NAME} ${SOURCE} ${HEADERS} )
