include_directories(${EIGEN3_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

# Produce a FastSLAM shared library. (For re-use of executables)
add_library(FastSLAM2
    fastslam2_sim.cpp  gauss_evaluate.cpp observe_heading.cpp  predict.cpp  sample_proposal.cpp  
)
target_link_libraries(FastSLAM2 FastSLAM_core) # We don't depend on anything

install(FILES
    fastslam2_sim.h  gauss_evaluate.h  observe_heading.h  predict.h  sample_proposal.h
    DESTINATION include/fslam/fastslam2
  )

install(TARGETS
  FastSLAM2
  DESTINATION lib
  )

# Add an executable that currentlyi runs the simulator
set(FSLAM_USED_LIBS FastSLAM2)
add_fslam_tool(fastslam2_simulation main.cpp)

