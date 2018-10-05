cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#set(CMAKE_MACOSX_RPATH TRUE)

project(elasticity LANGUAGES C)
set(CMAKE_BUILD_TYPE "Release")

# Add the source files to the (linear) elasticity library
add_library(elasticity SHARED sources/elasti1_2d.c
                              sources/elasti1_3d.c
                              sources/hash.c
                              sources/inout.c
                              sources/ls_calls.c
                              sources/packing.c)

# Find the commons library
find_library(commons NAMES commons HINTS "${PROJECT_SOURCE_DIR}/../../bin/lib")

# Set all target and properties of the elasticity library
target_link_libraries(elasticity PUBLIC ${commons})
target_include_directories(elasticity 
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sources>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../commons/include>)
set_property(TARGET elasticity PROPERTY C_STANDARD 99)

# Set the installation properties of the elasticity library
install(TARGETS elasticity LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/../../bin/lib")
install(FILES sources/ls_calls.h
              sources/elastic.h DESTINATION "${PROJECT_SOURCE_DIR}/include")
  
# Create and install the corresponding elastic executable
project(elastic LANGUAGES C)
set(CMAKE_BUILD_TYPE "Release")
add_executable(elastic sources/elastic.c)
target_link_libraries(elastic PUBLIC elasticity ${commons})
set_property(TARGET elastic PROPERTY C_STANDARD 99)
install(TARGETS elastic RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../../bin")

