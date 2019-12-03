cmake_minimum_required(VERSION 3.0)

project( shapelib )

#set( CMAKE_AUTOMOC OFF )
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -shared -fPIC")

file( GLOB header_list *.h )
file( GLOB source_list *.c )

#add_library(shapelib SHARED ${header_list} ${source_list} ) #shapelib name should be same
add_library(shapelib SHARED  ${source_list} ) #shapelib name should be same
#add_custom_target( ${PROJECT_NAME} ALL)

