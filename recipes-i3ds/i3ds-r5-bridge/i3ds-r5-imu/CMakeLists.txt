cmake_minimum_required(VERSION 2.8)

################################################################################
# Set some project wide defintions
################################################################################

project (i3ds-imu)

set (PROJECT_AUTHOR "SINTEF Digital")
set (PROJECT_URL "http://i3ds-h2020.eu/")
set (PROJECT_LICENSE "MPLv2")
set (PROJECT_VERSION "1.0")

set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scripts)

option(BUILD_EMBEDDED "Build for embedded systems" OFF)
option(GENERATE_ASN "Generate new source files from ASN.1 definitions" ON)

if (BUILD_EMBEDDED)
  if(NOT DEFINED CMAKE_FIND_ROOT_PATH)
    MESSAGE(FATAL_ERROR "CMAKE_FIND_ROOT_PATH path must be set when compiling for embedded")
  endif(NOT DEFINED CMAKE_FIND_ROOT_PATH)
  SET(CMAKE_C_COMPILER "aarch64-linux-gnu-gcc")
  SET(CMAKE_CXX_COMPILER "aarch64-linux-gnu-g++")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
endif (BUILD_EMBEDDED)

find_package (Boost COMPONENTS program_options log REQUIRED)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")
endif()

include_directories (include/)

################################################################################
# Add sources and targets
################################################################################

set(SRCS
  src/i3ds_r5_imu.cpp
  src/remoteproc_imu.cpp
  src/remoteproc_bridge.cpp)

set (LIBS
  zmq
  pthread
  i3ds
  ${Boost_LIBRARIES}
  )

add_executable (i3ds_imu ${SRCS})
target_link_libraries (i3ds_imu ${LIBS})
target_compile_features(i3ds_imu PRIVATE cxx_range_for)
install(TARGETS i3ds_imu DESTINATION bin)
