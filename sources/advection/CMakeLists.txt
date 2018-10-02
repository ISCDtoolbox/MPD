cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILD_TYPE "Release")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../commons/include")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../bin/lib")
set(CMAKE_C_FLAGS "-w -g -O3")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#advection library
project(advection)
file(GLOB_RECURSE source_files sources/*.c)
file(GLOB_RECURSE header_files sources/*.h)
add_library(advection SHARED ${source_files})
find_library(commons NAMES commons HINTS "${PROJECT_SOURCE_DIR}/../../bin/lib")
target_link_libraries(advection ${commons})
install(TARGETS advection LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/../../bin/lib")
install(FILES ${header_files} DESTINATION "${PROJECT_SOURCE_DIR}/include")

#executable
project(main)
add_executable(advect sources/advect.c)
target_link_libraries(advect commons advection)
install(TARGETS advect RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../../bin")

