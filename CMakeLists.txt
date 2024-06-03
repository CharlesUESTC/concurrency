cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME "concurrency")
project(${PROJECT_NAME})
set(MAJOR_VERSION 24)
set(MINOR_VERSION 2)
set(PATCH_VERSION 0)


# CMAKE MODULE SETUP.
#####################
# Paths that are searched when `include(...)` is called.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/operations/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/thirdparty/cmake)

# Cotire (compile time reducer) is a CMake module that speeds up the build process
# of CMake based build system by fully automating techniques as precompiled
# header usage and single compilation unit builds for C and C++
include(cotire)

# CONFIGURATION.
################
include(OJConfigure)

# SUBDIRECTORIES.
#################
add_subdirectory(source)
