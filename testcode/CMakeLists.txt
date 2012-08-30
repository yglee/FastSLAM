include_directories(${EIGEN3_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

install(FILES
	vector_add_cpu.h
    DESTINATION include/fslam/test # core
  )

set(FSLAM_USED_LIBS )
add_fslam_tool(fslam_gpu test_main.cc)
