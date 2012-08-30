# Install script for directory: /home/ylee8/FastSLAM/cpp/core

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/fslam/core" TYPE FILE FILES
    "/home/ylee8/FastSLAM/cpp/core/add_control_noise.h"
    "/home/ylee8/FastSLAM/cpp/core/compute_jacobians.h"
    "/home/ylee8/FastSLAM/cpp/core/data_associate_known.h"
    "/home/ylee8/FastSLAM/cpp/core/KF_cholesky_update.h"
    "/home/ylee8/FastSLAM/cpp/core/multivariate_gauss.h"
    "/home/ylee8/FastSLAM/cpp/core/stratified_random.h"
    "/home/ylee8/FastSLAM/cpp/core/predict_true.h"
    "/home/ylee8/FastSLAM/cpp/core/add_feature.h"
    "/home/ylee8/FastSLAM/cpp/core/compute_steering.h"
    "/home/ylee8/FastSLAM/cpp/core/feature_update.h"
    "/home/ylee8/FastSLAM/cpp/core/KF_joseph_update.h"
    "/home/ylee8/FastSLAM/cpp/core/particle.h"
    "/home/ylee8/FastSLAM/cpp/core/printMat.h"
    "/home/ylee8/FastSLAM/cpp/core/stratified_resample.h"
    "/home/ylee8/FastSLAM/cpp/core/add_observation_noise.h"
    "/home/ylee8/FastSLAM/cpp/core/configfile.h"
    "/home/ylee8/FastSLAM/cpp/core/get_observations.h"
    "/home/ylee8/FastSLAM/cpp/core/line_plot_conversion.h"
    "/home/ylee8/FastSLAM/cpp/core/pi_to_pi.h"
    "/home/ylee8/FastSLAM/cpp/core/resample_particles.h"
    "/home/ylee8/FastSLAM/cpp/core/TransformToGlobal.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFastSLAM_core.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFastSLAM_core.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFastSLAM_core.so"
         RPATH "/usr/local/lib")
  ENDIF()
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/ylee8/FastSLAM/cmake/cpp/core/libFastSLAM_core.so")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFastSLAM_core.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFastSLAM_core.so")
    FILE(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFastSLAM_core.so"
         OLD_RPATH "::::::::::::::"
         NEW_RPATH "/usr/local/lib")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libFastSLAM_core.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE PERMISSIONS GROUP_READ OWNER_READ FILES "/home/ylee8/FastSLAM/cpp/core/example_webmap.mat")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

