include_directories(${EIGEN3_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

# Produce a FastSLAM shared library. (For re-use of executables)
add_library(FastSLAM_core
    add_control_noise.cpp      compute_steering.cpp      get_observations.cpp      stratified_resample.cpp
    add_feature.cpp            configfile.cpp            KF_cholesky_update.cpp    multivariate_gauss.cpp  resample_particles.cpp  TransformToGlobal.cpp
    add_observation_noise.cpp  data_associate_known.cpp  KF_joseph_update.cpp      particle.cpp            predict_true.cpp
    compute_jacobians.cpp      feature_update.cpp        line_plot_conversion.cpp  pi_to_pi.cpp            stratified_random.cpp
  )
target_link_libraries(FastSLAM_core) # We don't depend on anything

install(FILES
    add_control_noise.h      compute_jacobians.h  data_associate_known.h  KF_cholesky_update.h    multivariate_gauss.h  stratified_random.h   predict_true.h
    add_feature.h            compute_steering.h   feature_update.h        KF_joseph_update.h      particle.h            printMat.h            stratified_resample.h
    add_observation_noise.h  configfile.h         get_observations.h      line_plot_conversion.h  pi_to_pi.h            resample_particles.h  TransformToGlobal.h
    DESTINATION include/fslam/core
  )

install(TARGETS
  FastSLAM_core
  DESTINATION lib
  )

# Add files that are just example data
install( FILES example_webmap.mat
  PERMISSIONS GROUP_READ OWNER_READ
  DESTINATION bin )
