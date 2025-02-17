# SPDX-FileCopyrightText: 2006-2021 Istituto Italiano di Tecnologia (IIT)
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(demo)
enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS 1)
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER "Autogen Targets")
set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "Generated Files")

find_package(YCM 0.11.0 REQUIRED)
find_package(YARP COMPONENTS os sig idl_tools catch2 REQUIRED)

include(Catch)
# Test timeout.
set(YARP_TEST_TIMEOUT_DEFAULT 120)
if (APPLE)
  set(YARP_TEST_TIMEOUT_DEFAULT 300)
endif()
set(YARP_TEST_TIMEOUT ${YARP_TEST_TIMEOUT_DEFAULT} CACHE STRING "Timeout for unit tests")
mark_as_advanced(YARP_TEST_TIMEOUT)
function(yarp_catch_discover_tests _target)
  # Workaround to force catch_discover_tests to run tests under valgrind
  if(DEFINED YARP_TEST_LAUNCHER)
    set_property(TARGET ${_target} PROPERTY CROSSCOMPILING_EMULATOR "${YARP_TEST_LAUNCHER}")
  endif()
  catch_discover_tests(
    ${_target}
    EXTRA_ARGS "-s" "--colour-mode default"
    PROPERTIES
      TIMEOUT ${YARP_TEST_TIMEOUT}
    ${ARGN}
  )
endfunction()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/catch2)

set(IDL_FILES
  annotated.thrift
  demo.thrift
  mapOfLists.thrift
  namespaced.thrift
  objects3D.thrift
  wrapping.thrift
  sub/directory/clock_rpc.thrift
  settings.thrift
)

# Test using yarp_idl_to_dir
set(generated_libs_dir "${CMAKE_CURRENT_BINARY_DIR}/yarp_idl_to_dir")
yarp_idl_to_dir(
  INPUT_FILES ${IDL_FILES}
  OUTPUT_DIR "${generated_libs_dir}"
  SOURCES_VAR IDL_GEN_SRCS
  HEADERS_VAR IDL_GEN_HDRS
  INCLUDE_DIRS_VAR IDL_INCLUDE_DIRS
  THRIFT_INCLUDE_PREFIX
  THRIFT_NO_NAMESPACE_PREFIX
)

add_executable(demo_yarp_idl_to_dir)
target_compile_definitions(demo_yarp_idl_to_dir
  PRIVATE
    THRIFT_INCLUDE_PREFIX
    THRIFT_NO_NAMESPACE_PREFIX
)
target_sources(demo_yarp_idl_to_dir
  PRIVATE
    main.cpp
    ${IDL_GEN_SRCS}
    ${IDL_GEN_HDRS}
)
target_include_directories(demo_yarp_idl_to_dir PRIVATE ${generated_libs_dir}/include)
target_link_libraries(demo_yarp_idl_to_dir
  PRIVATE
    YARP::YARP_os
    YARP::YARP_init
    YARP::YARP_sig
    YARP::YARP_catch2
)

yarp_catch_discover_tests(
  demo_yarp_idl_to_dir
  TEST_PREFIX demo::yarp_idl_to_dir::
)


# Test using yarp_add_idl
yarp_add_idl(IDL_GEN_FILES ${IDL_FILES})
add_executable(demo_yarp_add_idl)
target_compile_definitions(demo_yarp_add_idl
  PRIVATE
    THRIFT_INCLUDE_PREFIX
    THRIFT_NO_NAMESPACE_PREFIX
)
target_sources(demo_yarp_add_idl
  PRIVATE
    main.cpp
    ${IDL_GEN_FILES}
)
target_link_libraries(demo_yarp_add_idl
  PRIVATE
    YARP::YARP_os
    YARP::YARP_init
    YARP::YARP_sig
    YARP::YARP_catch2
)

yarp_catch_discover_tests(
  demo_yarp_add_idl
  TEST_PREFIX demo::yarp_add_idl::
)
