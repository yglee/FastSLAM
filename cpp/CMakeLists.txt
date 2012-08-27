include_directories(${EIGEN3_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

# Produce a FastSLAM shared library. (For re-use of executables)
add_library(FastSLAM
  KF_cholesky_update.cpp    add_observation_noise.cpp fastslam2_sim.cpp         predict.cpp               stratified_resample.cpp
  KF_joseph_update.cpp      compute_jacobians.cpp     feature_update.cpp        multivariate_gauss.cpp    predict_true.cpp
  TransformToGlobal.cpp     compute_steering.cpp      gauss_evaluate.cpp        observe_heading.cpp       resample_particles.cpp
  add_control_noise.cpp     configfile.cpp            get_observations.cpp      particle.cpp              sample_proposal.cpp
  add_feature.cpp           data_associate_known.cpp  line_plot_conversion.cpp  pi_to_pi.cpp              stratified_random.cpp
  )
target_link_libraries(FastSLAM) # We don't depend on anything

install(FILES
  KF_cholesky_update.h    compute_jacobians.h     gauss_evaluate.h        pi_to_pi.h              stratified_random.h
  KF_joseph_update.h      compute_steering.h      get_observations.h      predict.h               stratified_resample.h
  TransformToGlobal.h     configfile.h            line_plot_conversion.h  predict_true.h
  add_control_noise.h     data_associate_known.h  multivariate_gauss.h    printMat.h
  add_feature.h           fastslam2_sim.h         observe_heading.h       resample_particles.h
  add_observation_noise.h feature_update.h        particle.h              sample_proposal.h
  DESTINATION include/fslam
  )

install(TARGETS
  FastSLAM
  DESTINATION lib
  )

# Add an executable that currently runs the simulator
set(FSLAM_USED_LIBS FastSLAM)
add_fslam_tool(fastslam_simulation main.cpp)

# Add files that are just example data
install( FILES example_webmap.mat
  PERMISSIONS GROUP_READ OWNER_READ
  DESTINATION bin )