cmake_minimum_required(VERSION 2.8)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#set(CMAKE_MACOSX_RPATH TRUE)

project(mmg LANGUAGES C)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_RELEASE_VERSION "5.3.12")
set(CMAKE_RELEASE_DATE "Oct. 2, 2018" )
set(DEF_POSIX "#define POSIX")
set(DEF_GNU "#define GNU")

# Create mmgcommon.h file with the good release and architecture infos.
configure_file(${PROJECT_SOURCE_DIR}/sources/common/mmgcommon.h.in
               ${PROJECT_SOURCE_DIR}/sources/common/mmgcommon.h @ONLY)

# Add the source files to the mmg library
add_library(mmg SHARED sources/common/anisomovpt.c
                       sources/common/anisosiz.c
                       sources/common/API_functions.c
                       sources/common/API_functionsf.c
                       sources/common/bezier.c
                       sources/common/boulep.c
                       sources/common/chrono.c
                       sources/common/eigenv.c
                       sources/common/hash.c
                       sources/common/inout.c
                       sources/common/intmet.c
                       sources/common/isosiz.c
                       sources/common/librnbg.c
                       sources/common/mettools.c
                       sources/common/mmg.c
                       sources/common/quality.c
                       sources/common/scalem.c
                       sources/common/tools.c

                       sources/mmg3d/analys_3d.c
                       sources/mmg3d/anisomovpt_3d.c
                       sources/mmg3d/anisosiz_3d.c
                       sources/mmg3d/API_functions_3d.c
                       sources/mmg3d/API_functionsf_3d.c
                       sources/mmg3d/bezier_3d.c
                       sources/mmg3d/boulep_3d.c
                       sources/mmg3d/cenrad_3d.c
                       sources/mmg3d/chkmsh_3d.c
                       sources/mmg3d/colver_3d.c
                       sources/mmg3d/delaunay_3d.c
                       sources/mmg3d/hash_3d.c
                       sources/mmg3d/inout_3d.c
                       sources/mmg3d/intmet_3d.c
                       sources/mmg3d/isosiz_3d.c
                       sources/mmg3d/libmmg3d.c
                       sources/mmg3d/libmmg3df.c
                       sources/mmg3d/libmmg3d_tools.c
                       sources/mmg3d/libmmg3d_toolsf.c
                       sources/mmg3d/librnbg_3d.c
                       sources/mmg3d/mmg3d1.c
                       sources/mmg3d/mmg3d1_delone.c
                       sources/mmg3d/mmg3d1_pattern.c
                       sources/mmg3d/mmg3d2.c
                       sources/mmg3d/mmg3d3.c

                       sources/mmg3d/movpt_3d.c
                       sources/mmg3d/optbdry_3d.c
                       sources/mmg3d/optlap_3d.c  # Develop branch
                       sources/mmg3d/opttyp_3d.c
                       #sources/mmg3d/octree_3d.c # Master branch
                       sources/mmg3d/PRoctree_3d.c  # Develop branch
                       sources/mmg3d/quality_3d.c
                       sources/mmg3d/split_3d.c
                       sources/mmg3d/swap_3d.c
                       sources/mmg3d/swapgen_3d.c
                       sources/mmg3d/tools_3d.c
                       sources/mmg3d/variadic_3d.c
                       sources/mmg3d/velextls_3d.c
                       sources/mmg3d/zaldy_3d.c)

# Find the math, Elas, and Scotch libraries (libptscotch-dev)
find_library(M_LIB m)
set(SCOTCH_DIR "" CACHE PATH "Installation directory for scotch")
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
find_package(Scotch)

find_path(ELAS_INCLUDE_DIR NAMES elastic.h HINTS "${PROJECT_SOURCE_DIR}/../elasticity/include")
find_library(ELAS_LIBRARY NAMES elasticity HINTS "${PROJECT_SOURCE_DIR}/../../bin/lib")
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ELAS DEFAULT_MSG ELAS_INCLUDE_DIR ELAS_LIBRARY)
message(STATUS "Vertex insertion by delaunay kernel")

# Set all target and properties of the mmg library
target_include_directories(mmg
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sources/common>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sources/mmg3d>
    $<BUILD_INTERFACE:${SCOTCH_INCLUDE_DIRS}>
    $<BUILD_INTERFACE:${ELAS_INCLUDE_DIR}>)
set_property(TARGET mmg PROPERTY C_STANDARD 99)
target_compile_definitions(mmg PUBLIC USE_ELAS USE_SCOTCH)
target_compile_options(mmg PUBLIC -Wno-char-subscripts -Wno-unused-result)
target_link_libraries(mmg PUBLIC ${M_LIB}
                                 ${SCOTCH_LIBRARIES}
                                 ${ELAS_LIBRARY})

# Set the installation properties of the mmg library
install(TARGETS mmg LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/../../bin/lib")
install(FILES sources/common/libmmgtypes.h
#              sources/common/libmmgtypesf.h
              sources/mmg3d/libmmg3d.h
#              sources/mmg3d/libmmg3df.h
        DESTINATION "${PROJECT_SOURCE_DIR}/include")

# Create and install the mmg3d executable
project(mmg3d LANGUAGES C)
set(CMAKE_BUILD_TYPE "Release")
add_executable(mmg3d sources/mmg3d/mmg3d.c)
target_link_libraries(mmg3d mmg)
set_property(TARGET mmg3d PROPERTY C_STANDARD 99)
install(TARGETS mmg3d RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../../bin")

