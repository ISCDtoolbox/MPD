cmake_minimum_required(VERSION 2.8)

project(mpd LANGUAGES C)
set(CMAKE_BUILD_TYPE "Release")

# Add the source files to the mmg executable
add_executable(mpdProgram sources/loadParameters.c
                          sources/loadChemistry.c
                          sources/loadMesh.c
                          sources/adaptMesh.c
                          sources/optimization.c
                          sources/main.c)

# Find the math, openmp (libgomp1, -fopenmp with gcc compiler)
# and lapacke libraries (liblapacke-dev, -llapacke with gcc compiler)
find_library(M_LIB m)
find_package(OpenMP REQUIRED)
set(LAPACKE_DIR "" CACHE PATH "Installation directory for lapacke")
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
find_package(LAPACKE REQUIRED)

# Set all target and properties of the mpd executable
target_include_directories(mpdProgram
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sources>
    $<BUILD_INTERFACE:${LAPACKE_INCLUDE_DIRS}>)
set_property(TARGET mpdProgram PROPERTY C_STANDARD 99)
target_compile_definitions(mpdProgram PUBLIC USE_LAPACKE)
target_link_libraries(mpdProgram PUBLIC ${M_LIB}
                                        ${LAPACKE_LIBRARIES}
                                        ${OpenMP_C_FLAGS})

# Install the executable
install(TARGETS mpdProgram RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../../bin")

