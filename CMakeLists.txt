CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Name of the project
project(simple-ecs)

# Set default build type to Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type (Debug or Release)" FORCE)
endif()

message(STATUS "CMake build type: " ${CMAKE_BUILD_TYPE})

# Version of the project
set(PROJECT_VERSION "0.1.0")

# If build type is Debug, generate debugging information and don't optimise
if (CMAKE_BUILD_TYPE STREQUAL Debug)
  add_definitions(-O0)
  set(CMAKE_C_FLAGS "-g -Wall -pedantic")
else()
  add_definitions(-O3)
  set(CMAKE_C_FLAGS "-Wall -pedantic")
endif()

# C standard for the compiler to use
set(CMAKE_CXX_STANDARD 17)

############################################
#                                          #
#              Dependencies                #
#                                          #
############################################

find_package(PkgConfig)

pkg_check_modules(PKG_CONFIG "pkg-config")


############################################
#                                          #
#                  Tests                   #
#                                          #
############################################

pkg_check_modules(GTEST "gtest")

enable_testing()

add_executable(test_simple_ecs
  tests/test_simple_ecs.cpp
  )

add_test(NAME
  test_simple_ecs
  COMMAND ./test_simple_ecs
  )

target_link_libraries(test_simple_ecs
  ${GTEST_LIBRARIES}
  # Link libraries needed in this test
  )
