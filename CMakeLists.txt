

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(PolyCrop)
add_subdirectory(shapelib)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
include_directories(Shapefiles)
#link_directories(${CMAKE_SOURCE_DIR}/build/shapelib)


find_package(PCL 1.7 REQUIRED)
set (CMAKE_CXX_STANDARD 11)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable (GLRenderSHP GLRenderSHP.cpp )
#add_executable (GLRenderSHP GLRenderSHP.cpp shapelib/shapefil.h)
#target_link_libraries (GLRenderSHP ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PCL_LIBRARIES} libshapelib.so)
target_link_libraries (GLRenderSHP ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PCL_LIBRARIES} shapelib)


