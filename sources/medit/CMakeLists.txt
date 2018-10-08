cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
set(CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/../../bin/lib")
#set(CMAKE_MACOSX_RPATH TRUE)

macro(DATE RESULT)
  execute_process(COMMAND "date" "+%Y-%m-%d" OUTPUT_VARIABLE ${RESULT}
                                             OUTPUT_STRIP_TRAILING_WHITESPACE)
endmacro()

macro(TIME RESULT)
  execute_process(COMMAND "date" "+%H:%M:%S" OUTPUT_VARIABLE ${RESULT}
                                             OUTPUT_STRIP_TRAILING_WHITESPACE)
endmacro()

project(medit3d LANGUAGES C)
set(CMAKE_BUILD_TYPE "Release")

# Write the compilation date in a file entitled compil.date in the sources directory
DATE(day)
TIME(hour)
file(WRITE sources/compil.date "#define COMPIL \"${day} ${hour}\"")

# Add the source files to the medit3d library
add_library(medit3d SHARED sources/animat.c
                             sources/bbfile.c
                             sources/camera.c
                             sources/cenrad.c
                             sources/chrono.c
                             sources/clip.c
                             sources/clipvol.c
                             sources/critip.c
                             sources/cube.c
                             sources/dlists.c
                             sources/eigenv.c
                             sources/ellipse.c
                             sources/geometry.c
                             sources/gisfil.c
                             sources/hash.c
                             sources/ilists.c
                             sources/image.c
                             sources/inmsh2.c
                             sources/inout.c
                             sources/items.c
                             sources/keyboard.c
                             sources/libmesh5.c
                             sources/listnum.c
                             sources/material.c
                             sources/memory.c
                             sources/menus.c
                             sources/mesh.c
                             sources/mlists.c
                             sources/morphing.c
                             sources/mouse.c
                             sources/normal.c
                             sources/param.c
                             sources/parsar.c
                             sources/parsop.c
                             sources/particle.c
                             sources/path.c
                             sources/persp.c
                             sources/picking.c
                             sources/psfile.c
                             sources/scene.c
                             sources/scissor.c
                             sources/sftcpy.c
                             sources/status.c
                             sources/stream.c
                             sources/tiles.c
                             sources/transform.c
                             sources/util.c
                             sources/vector.c
                             sources/view.c
                             sources/zaldy1.c
                             sources/zaldy2.c)

# Find the OpenGl and GLUT libraries (freeglut3-dev, libxmu-dev and libxi-dev)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

# Set all target and properties of the medit3d library
target_include_directories(medit3d
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sources>
    $<BUILD_INTERFACE:${OpenGL_INCLUDE_DIRS}>
    $<BUILD_INTERFACE:${GLUT_INCLUDE_DIRS}>)
set_property(TARGET medit3d PROPERTY C_STANDARD 99)
target_link_libraries(medit3d PUBLIC ${OPENGL_LIBRARIES}
                                     ${GLUT_LIBRARIES})

# Set the installation properties of the medit3d library
install(TARGETS medit3d LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/../../bin/lib")
install(FILES sources/chrono.h
              sources/eigenv.h
              sources/extern.h
              sources/grafic.h
              sources/image.h
              sources/libmesh5.h
              sources/medit.h
              sources/memory.h
              sources/mesh.h
              sources/sproto.h DESTINATION "${PROJECT_SOURCE_DIR}/include")

# Create and install the corresponding medit executable
project(medit LANGUAGES C)
set(CMAKE_BUILD_TYPE "Release")
add_executable(medit sources/medit.c)
target_link_libraries(medit PUBLIC medit3d)
set_property(TARGET medit PROPERTY C_STANDARD 99)
install(TARGETS medit RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../../bin")

