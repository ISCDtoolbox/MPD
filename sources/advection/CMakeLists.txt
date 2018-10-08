cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#set(CMAKE_MACOSX_RPATH TRUE)

project(advection LANGUAGES C)
set(CMAKE_BUILD_TYPE "Release")

# Add the source files to the advection library
add_library(advection SHARED sources/ad_calls.c
                             sources/advec1_2d.c
                             sources/advec1_3d.c
                             sources/hash.c
                             sources/inout.c)

# Find the commons library
find_library(commons NAMES commons HINTS "${PROJECT_SOURCE_DIR}/../../bin/lib")

# Set all target and properties of the advection library
target_link_libraries(advection PUBLIC ${commons})
target_include_directories(advection
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sources>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../commons/include>)
set_property(TARGET advection PROPERTY C_STANDARD 99)

# Set the installation properties of the advection library
install(TARGETS advection LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/../../bin/lib")
install(FILES sources/ad_calls.h
              sources/advect.h DESTINATION "${PROJECT_SOURCE_DIR}/include")

# Create and install the corresponding advect executable
project(advect LANGUAGES C)
set(CMAKE_BUILD_TYPE "Release")
add_executable(advect sources/advect.c)
target_link_libraries(advect PUBLIC advection ${commons})
set_property(TARGET advect PROPERTY C_STANDARD 99)
install(TARGETS advect RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../../bin")

