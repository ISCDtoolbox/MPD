cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_C_FLAGS "-w -g -O3")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

project(commons)

find_package(OpenMP REQUIRED)
if(NOT OPENMP_FOUND)
  message(ERROR " OpenMP library not found!")
endif(NOT OPENMP_FOUND)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
find_library(M_LIB m)
set(LIBRARIES ${M_LIB} "-lpthread")

#commons library
set(SOURCES_DIR sources)
file(GLOB_RECURSE source_files ${SOURCES_DIR}/*.c)
file(GLOB_RECURSE header_files ${SOURCES_DIR}/*.h)
include_directories(${SOURCES_DIR})
add_library(commons SHARED ${source_files})
target_link_libraries(commons ${LIBRARIES})
install(TARGETS commons LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/../../bin/lib")
install(FILES ${header_files} DESTINATION "${PROJECT_SOURCE_DIR}/include")

