include_directories(${EIGEN3_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

# Produce a FastSLAM shared library. (For re-use of executables)
add_library(FastSLAM1
    compute_weight.cpp  fastslam1_sim.cpp  predict.cpp
)

target_link_libraries(FastSLAM1 FastSLAM_core) # We don't depend on anything

install(FILES
    compute_weight.h  fastslam1_sim.h  predict.h
    DESTINATION include/fslam/fastslam1
  )

install(TARGETS
  FastSLAM1
  DESTINATION lib
  )

# Add an executable that currentlyi runs the simulator
set(FSLAM_USED_LIBS FastSLAM1)
add_fslam_tool(fastslam1_simulation main.cpp)

