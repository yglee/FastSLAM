include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

# Define here any libraries that you'll use later on in this
# file. Things like OpenCL or Boost libraries would go here. (Or your
# own FSLAM libs).
set(FSLAM_USED_LIBS FastSLAM1)

# Tests
fslam_add_test( test.cpp )

