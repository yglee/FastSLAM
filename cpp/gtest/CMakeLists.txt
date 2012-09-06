include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

# Define here any libraries that you'll use later on in this
# file. Things like OpenCL or Boost libraries would go here. (Or your
# own FSLAM libs).
set(FSLAM_USED_LIBS FastSLAM1 FastSLAM_core)

# Tests
fslam_add_test(TestFS1AgainstMatlab.cpp)

# Support Files
file(COPY ${CMAKE_SOURCE_DIR}/cpp/gtest/example_webmap.mat DESTINATION ${CMAKE_BINARY_DIR}/cpp/gtest/)
