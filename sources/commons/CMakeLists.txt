cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
#set(CMAKE_VERBOSE_MAKEFILE ON)

project(commons LANGUAGES C)
set(CMAKE_BUILD_TYPE "Release")

# Add the source files to the commons library
add_library(commons SHARED sources/chrono.c
                           sources/eigen.c
                           sources/libmesh5.c
                           sources/lplib3.c
                           sources/solve.c
                           sources/sparse.c)

# Find the OpenMp (libgomp1, -fopenmp with gcc compiler),
# the Threads (libpthread-stubs0-dev, -pthread with gcc compiler)
# and math libraries (-lm with gcc compiler)
#
# IMPORTANT REMARK: when -fopenmp option is set in gcc, the preprocessor compiler
#                   creates a preprocessor constant _OPENMP yyyymmdd where yyyy is
#                   the year mm the month and dd the day of the preprocessor compilation.
#                   This preprocessor constant is used in sparse.c to include (or not)
#                   the omp.h file and thus considering or not the parallelization.
#
# MINOR REMARK: adding -lpthread option with gcc only links the pthread library while
#               the pre-defined macros do not get defined. The -pthread option tells
#               the compiler to link in the pthread library as well as configure the
#               compilation for threads. Tendancy is to add -pthread option instead of
#               -lpthread
find_package(OpenMP REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)
find_library (M_LIB m)

# Set all target and properties of the commons library
target_include_directories(commons
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sources>)
set_property(TARGET commons PROPERTY C_STANDARD 99)
target_link_libraries(commons PUBLIC ${M_LIB}
                                     Threads::Threads
                                     ${OpenMP_C_FLAGS})

# Set the installation properties of the commons library
install(TARGETS commons LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/../../bin/lib")
install(FILES sources/chrono.h
              sources/eigen.h
              sources/libmesh5.h
              sources/lplib3.h
              sources/sparse.h DESTINATION "${PROJECT_SOURCE_DIR}/include")

