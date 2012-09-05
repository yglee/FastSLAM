include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${OPENCL_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

link_libraries(${OPENCL_LIBRARIES})

add_library(OPENCL_WRAPPER
	openCLWrapper.cpp 
	vector_add_cpu.cpp
)

target_link_libraries(OPENCL_WRAPPER)

install(FILES
    openCLWrapper.h
    utilities.h
    vector_add_cpu.h
    DESTINATION include/fslam/test
  )

install(TARGETS
    OPENCL_WRAPPER
    DESTINATION lib
    )

set(FSLAM_USED_LIBS ${OPENCL_LIBRARIES} OPENCL_WRAPPER)
add_fslam_tool(fslam_gpu test_main.cc)
