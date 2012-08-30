project(FSLAM CXX)
cmake_minimum_required(VERSION 2.8)

# Add path for custom modules
set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
  )

set(PACKAGE_VERSION "0.1")
set(PACKAGE_NAME "FastSLAM")
set(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_BUGREPORT "yeonjin.lee@nasa.gov")

# Local Custom Options
option(FSLAM_BUILD_TOOLS
  "Build the FastSLAM test tools." ON)
option(BUILD_SHARED_LIBS
  "Produce shared libraries." TRUE)
option(ENABLE_RPATHS
  "Use RPaths in executables to find shared libraries." TRUE)
option(ENABLE_PROFILING
  "Link against the google perftools." FALSE)
option(ENABLE_TCMALLOC
  "Link against the google perftools's tcmalloc." FALSE)
if (ENABLE_RPATHS)
  set(CMAKE_SKIP_RPATH                  FALSE CACHE BOOL "")
  set(CMAKE_SKIP_BUILD_RPATH            FALSE CACHE BOOL "")
  set(CMAKE_BUILD_WITH_INSTALL_RPATH    FALSE CACHE BOOL "")
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE  CACHE BOOL "")
  if(APPLE)
    set(CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/lib)
  endif(APPLE)
  mark_as_advanced(
    CMAKE_SKIP_RPATH
    CMAKE_SKIP_BUILD_RPATH
    CMAKE_BUILD_WITH_INSTALL_RPATH
    CMAKE_INSTALL_RPATH
    CMAKE_INSTALL_RPATH_USE_LINK_PATH
    )
endif(ENABLE_RPATHS)

# Find Dependencies
include(AddFSLAM)
find_package(Eigen3 REQUIRED)
find_package(GooglePerfTools)
find_package(OpenCL REQUIRED)

# Finish up local custom options based on what was found above
fslam_enable_testing()
if (ENABLE_PROFILING AND GOOGLE_PERFTOOLS_FOUND)
  set(FSLAM_ENABLE_PROFILING ON )
else()
  set(FSLAM_ENABLE_PROFILING OFF )
endif()

if (ENABLE_TCMALLOC AND GOOGLE_PERFTOOLS_FOUND)
  set(FSLAM_ENABLE_TCMALLOC ON)
else()
  set(FSLAM_ENABLE_TCMALLOC OFF)
endif()

# Add common search locations
include_directories(${FSLAM_SOURCE_DIR}/cpp)
include_directories(${FSLAM_BINARY_DIR}/cpp)

# Adding build directories
add_subdirectory(cpp/core)
add_subdirectory(cpp/fastslam1)
add_subdirectory(cpp/fastslam2)

# Adding test directories
add_subdirectory(cpp/gtest)
