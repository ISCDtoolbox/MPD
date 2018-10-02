cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_C_FLAGS "-w -g -O3")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/../../bin/lib")

macro(DATE RESULT)
  execute_process(COMMAND "date" "+%Y-%m-%d" OUTPUT_VARIABLE ${RESULT} OUTPUT_STRIP_TRAILING_WHITESPACE)
endmacro()
macro(TIME RESULT)
  execute_process(COMMAND "date" "+%H:%M:%S" OUTPUT_VARIABLE ${RESULT} OUTPUT_STRIP_TRAILING_WHITESPACE)
endmacro()

#medit library
project(medit3d)
DATE(day)
TIME(hour)
file(WRITE sources/compil.date "#define COMPIL \"${day} ${hour}\"")
file(GLOB_RECURSE source_files sources/*.c)
file(GLOB_RECURSE header_files sources/*.h)
add_library(medit3d SHARED ${source_files})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
  message(ERROR "GLUT not found!")
endif(NOT GLUT_FOUND)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
  message(ERROR "OPENGL not found!")
endif(NOT OPENGL_FOUND)

set(GL_LIBS ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
target_link_libraries(medit3d ${GL_LIBS})
install(TARGETS medit3d LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/../../bin/lib")
install(FILES ${header_files} DESTINATION "${PROJECT_SOURCE_DIR}/include")

#executable
project(main)
add_executable(medit sources/medit.c)
target_link_libraries(medit medit3d)
install(TARGETS medit RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/../../bin")

