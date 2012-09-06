macro(add_fslam_executable name)
  add_executable(${name} ${ARGN})
  foreach(lib ${FSLAM_USED_LIBS})
    if( NOT ${lib} STREQUAL "optimized" AND
        NOT ${lib} STREQUAL "debug" )
      target_link_libraries( ${name} ${lib} )
    endif()
  endforeach(lib)
  if ( FSLAM_ENABLE_PROFILING )
    target_link_libraries( ${name} ${PROFILER_LIBRARY} )
  endif()
  if ( FSLAM_ENABLE_TCMALLOC )
    target_link_libraries( ${name} ${TCMALLOC_LIBRARY} )
  endif()
  if ( MKL_LIBRARIES )
    target_link_libraries( ${name} ${MKL_LIBRARIES} )
  endif()
endmacro(add_fslam_executable name)

macro(add_fslam_tool name)
  add_fslam_executable(${name} ${ARGN})
  if( FSLAM_BUILD_TOOLS )
    message(STATUS  "Installing: " ${name} )
    install(TARGETS ${name} RUNTIME DESTINATION bin)
  endif()
  set_target_properties(${name} PROPERTIES FOLDER "tools")
endmacro(add_fslam_tool name)

function(fslam_enable_testing)
  set(GTEST_DIR ${CMAKE_SOURCE_DIR}/thirdparty/gtest)
  include_directories(${VISIONWORKBENCH_INCLUDE_DIRS})
  include_directories(${GTEST_DIR}/include)
  add_library(gtest SHARED EXCLUDE_FROM_ALL
    ${GTEST_DIR}/src/gtest-all.cc
    ${CMAKE_SOURCE_DIR}/cpp/gtest/test_main.cc
    )
  target_link_libraries(gtest
    ${VISIONWORKBENCH_CORE_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    )
  set_target_properties(gtest
    PROPERTIES
    COMPILE_FLAGS "-DTEST_SRCDIR=\\\"${CMAKE_CURRENT_SOURCE_DIR}\\\" -DTEST_DSTDIR=\\\"${CMAKE_CURRENT_BINARY_DIR}\\\""
    )

  add_custom_target(check)
  add_dependencies(check gtest)
endfunction(fslam_enable_testing)


function(fslam_add_test source_file)
  string(REGEX REPLACE "^([A-Za-z0-9_]*)\\.([A-Za-z0-9]*)" "\\1" executable "${source_file}")

  add_executable(${executable} EXCLUDE_FROM_ALL ${source_file} )
  target_link_libraries( ${executable} gtest )
  foreach(lib ${FSLAM_USED_LIBS})
    target_link_libraries( ${executable} ${lib} )
  endforeach(lib)

  set_target_properties(${executable}
    PROPERTIES
    COMPILE_FLAGS "-DTEST_SRCDIR=\\\"${CMAKE_CURRENT_SOURCE_DIR}\\\" -DTEST_DSTDIR=\\\"${CMAKE_CURRENT_BINARY_DIR}\\\""
    )
  add_custom_target(${executable}Exec ${CMAKE_CURRENT_BINARY_DIR}/${executable}
    DEPENDS ${executable}
    )
  add_dependencies(check ${executable}Exec)

  if ( MKL_LIBRARIES )
    target_link_libraries( ${executable} ${MKL_LIBRARIES} )
  endif()

  # file(READ "${source_file}" contents)
  # string(REGEX MATCHALL "TEST_?F?\\(([A-Za-z_0-9 ,]+)\\)" found_tests ${contents})
  # foreach(hit ${found_tests})
  #   string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+), *([A-Za-z_0-9]+) *\\).*" "\\1.\\2" test_name ${hit})
  #   message("Added test name: " ${executable} "\t" ${test_name})
  #   add_test(${test_name} ${executable} --gtest_filter=${test_name})
  # endforeach()
endfunction(fslam_add_test)
