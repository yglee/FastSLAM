include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${OPENCL_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

add_library(OPENCLWRAPPER
	openCLWrapper.cpp 
)

install(FILES
    openCLWrapper.h
    DESTINATION include/fslam/test
  )

install(TARGETS
    OPENCLWRAPPER
    DESTINATION lib
)

set(FSLAM_USED_LIBS ${OPENCL_LIBRARIES} OPENCLWRAPPER)
add_fslam_tool(fslam_gpu test_main.cc)
