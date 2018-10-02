cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_C_FLAGS "-w -g -O3 ${CMAKE_C_FLAGS}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

project(mpd)
file(GLOB_RECURSE source_files sources/*.c)
add_executable(mpdProgram ${source_files})

#libraries
FIND_LIBRARY(M_LIB m)
set(LAPACKE_DIR "" CACHE PATH "Installation directory for lapacke")
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
find_package(LAPACKE REQUIRED)
IF(NOT LAPACKE_FOUND)
  message(ERROR " LAPACKE library not found!")
ENDIF(NOT LAPACKE_FOUND)
include_directories(${LAPACKE_INCLUDE_DIRS})
set(CMAKE_C_FLAGS "-DUSE_LAPACKE ${CMAKE_C_FLAGS}")
message(STATUS "Compilation with lapacke: ${LAPACKE_LIBRARIES}")
set(LIBRARIES ${LIBRARIES} ${LAPACKE_LIBRARIES})

find_package(OpenMP REQUIRED)
if(NOT OPENMP_FOUND)
  message(ERROR " OpenMP library not found!")
endif(NOT OPENMP_FOUND)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")

set(LIBRARIES ${M_LIB} "-llapacke" "-fopenmp")
target_link_libraries(mpdProgram ${LIBRARIES})
install(TARGETS mpdProgram RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../../bin")


 



