cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#set(CMAKE_MACOSX_RPATH TRUE)

project(mshdistance LANGUAGES C)
set(CMAKE_BUILD_TYPE "Release")

# Add the source files to the mshdistance library
add_library(mshdistance SHARED sources/boulep.c
                               sources/bucket.c
                               sources/hash.c
                               sources/inout.c
                               sources/locelt.c
                               sources/memory.c
                               sources/mshdis1_2d.c
                               sources/mshdis1_3d.c
                               sources/queue.c
                               sources/scalem.c
                               sources/specdist_2d.c
                               sources/specdist_3d.c
                               sources/tools_2d.c
                               sources/tools_3d.c)

# Find the commons library
find_library(commons NAMES commons HINTS "${PROJECT_SOURCE_DIR}/../../bin/lib")
   
# Set all target and properties of the mshdistance library
target_link_libraries(mshdistance PUBLIC ${commons})
target_include_directories(mshdistance
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sources>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../commons/include>)
set_property(TARGET mshdistance PROPERTY C_STANDARD 99)

# Set the installation properties of the mshdistance library
install(TARGETS mshdistance LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/../../bin/lib")
install(FILES sources/memory.h
              sources/mshdist.h DESTINATION "${PROJECT_SOURCE_DIR}/include")
  
# Create and install the corresponding mshdist executable
project(mshdist LANGUAGES C)
set(CMAKE_BUILD_TYPE "Release")
add_executable(mshdist sources/mshdist.c)
target_link_libraries(mshdist PUBLIC mshdistance ${commons})
set_property(TARGET mshdist PROPERTY C_STANDARD 99)
install(TARGETS mshdist RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../../bin")

