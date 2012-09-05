include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

# Produce a test shared library. (For re-use of executables)
add_library(test_core
	test.cpp
)

target_link_libraries(test_core FastSLAM_core) # We don't depend on anything

install(FILES
    test.h
    DESTINATION include/fslam/gtest
  )

install(TARGETS
  test_core
  DESTINATION lib
  )

# Add an executable that currently runs the simulator
set(FSLAM_USED_LIBS test_core)
add_fslam_tool(test main.cpp)

