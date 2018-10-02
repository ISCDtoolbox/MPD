## =============================================================================
##  This file is part of the mmg software package for the tetrahedral
##  mesh modification.
##  Copyright (c) Bx INP/Inria/UBordeaux/UPMC, 2004- .
##
##  mmg is free software: you can redistribute it and/or modify it
##  under the terms of the GNU Lesser General Public License as published
##  by the Free Software Foundation, either version 3 of the License, or
##  (at your option) any later version.
##
##  mmg is distributed in the hope that it will be useful, but WITHOUT
##  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
##  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
##  License for more details.
##
##  You should have received a copy of the GNU Lesser General Public
##  License and of the GNU General Public License along with mmg (in
##  files COPYING.LESSER and COPYING). If not, see
##  <http://www.gnu.org/licenses/>. Please read their terms carefully and
##  use this copy of the mmg distribution only if you accept them.
## =============================================================================

cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_C_FLAGS "-Wno-char-subscripts ${CMAKE_C_FLAGS}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

project(mmg)

# Create mmgcommon.h file with the good release and architecture infos.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sources/common/mmgcommon.h.in
  ${PROJECT_BINARY_DIR}/sources/common/mmgcommon.h @ONLY)
file(GLOB_RECURSE source_files sources/common/*.c sources/mmg3d/*.c)
set(header_files ${PROJECT_SOURCE_DIR}/sources/mmg3d/libmmg3d.h
  ${PROJECT_SOURCE_DIR}/sources/mmg3d/libmmg3df.h
  ${PROJECT_SOURCE_DIR}/sources/common/libmmgtypes.h
  ${PROJECT_SOURCE_DIR}/sources/common/libmmgtypesf.h)
add_library(mmg SHARED ${source_files})

#libraries
find_library(M_LIB m)
set(LIBRARIES ${M_LIB} ${LIBRARIES})

set(SCOTCH_DIR "" CACHE PATH "Installation directory for scotch")
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
find_package(Scotch REQUIRED)
IF(NOT SCOTCH_FOUND)
  message(ERROR " SCOTCH library not found!")
ENDIF(NOT SCOTCH_FOUND)
include_directories(${SCOTCH_INCLUDE_DIRS})
set(CMAKE_C_FLAGS "-DUSE_SCOTCH ${CMAKE_C_FLAGS}")
message(STATUS "Compilation with scotch: ${SCOTCH_LIBRARIES}")
set(LIBRARIES ${LIBRARIES} ${SCOTCH_LIBRARIES})

find_path(ELAS_INCLUDE_DIR NAMES elastic.h HINTS "${PROJECT_SOURCE_DIR}/../elasticity/include")
find_library(ELAS_LIBRARY NAMES elasticity HINTS "${PROJECT_SOURCE_DIR}/../../bin/lib")
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ELAS DEFAULT_MSG ELAS_INCLUDE_DIR ELAS_LIBRARY)
IF(NOT ELAS_FOUND)
  message(ERROR " elasticity library not found!")
ENDIF(NOT ELAS_FOUND)
include_directories(${ELAS_INCLUDE_DIR})
set(CMAKE_C_FLAGS "-DUSE_ELAS ${CMAKE_C_FLAGS}")
message(STATUS "Compilation with the Elas library: ${ELAS_LIBRARY}")
set(LIBRARIES ${LIBRARIES} ${ELAS_LINK_FLAGS})
set(LIBRARIES ${LIBRARIES} ${ELAS_LIBRARY})

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/sources/common)
target_link_libraries(mmg ${LIBRARIES})
install(TARGETS mmg LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/../../bin/lib")
install(FILES ${header_files} DESTINATION ${PROJECT_SOURCE_DIR}/include)

#executable
project(main)
add_executable(mmg3d sources/mmg3d/mmg3d.c)
target_link_libraries(mmg3d mmg)
install(TARGETS mmg3d RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../../bin")

