#CMAKE FILE FOR SCRATCH DIRECTORY OF VERSOR GEOMETRIC ALGEBRA LIBRARY

#VERSION
cmake_minimum_required(VERSION 2.8)

#POLICIES
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 OLD)
endif(COMMAND cmake_policy)

#PROJECT
project(VERSORSCRATCH)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -U__STRICT_ANSI__ -O3 -ftemplate-depth-1200 -Wno-switch -Wno-deprecated-declarations -Wno-int-to-pointer-cast")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -U__STRICT_ANSI__ -nostdinc++ -O3 -ftemplate-depth-1200 -Wno-switch -Wno-int-to-pointer-cast") 

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../ext/gfx/cmake/modules")

  #GLV should be optional
  if(NOT GLV_INCLUDE_DIR)
    set(GLV_INCLUDE_DIR “${CMAKE_SOURCE_DIR}/../ext/glv”)
  endif()
  find_package(GLV)

#GET INCLUDE DIRECTORIES, LINK DIRECTORIES, AND LINKED LIBS
get_property(GFX_LIBRARIES GLOBAL PROPERTY GFX_LIBRARIES)
get_property(GFX_INCLUDE_DIR GLOBAL PROPERTY GFX_INCLUDE_DIR)
get_property(GFX_LINK_DIR GLOBAL PROPERTY GFX_LINK_DIR)


#PROJECT INCLUDE AND LINK


include_directories(${PROJECT_SOURCE_DIR}/../ )
include_directories(${CMAKE_SOURCE_DIR}/../vsr/)
include_directories(${CMAKE_SOURCE_DIR}/../vsr/space)
include_directories(${CMAKE_SOURCE_DIR}/../vsr/draw)
include_directories(${CMAKE_SOURCE_DIR}/../vsr/util)
include_directories(${CMAKE_SOURCE_DIR}/../vsr/detail)
include_directories(${CMAKE_SOURCE_DIR}/../vsr/z_deprecated)
include_directories(${GFX_INCLUDE_DIR})

include_directories(${GLV_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/../ext/glv)
include_directories(${CMAKE_SOURCE_DIR}/../ext/gfx)


link_directories(${PROJECT_SOURCE_DIR}/../build/)
link_directories(${GFX_LINK_DIR})

set(vsr_libraries vsr vsrGfx)
set(libraries ${vsr_libraries} ${GFX_LIBRARIES}  ) 

#Boosts TEST TARGETS
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
file(GLOB projects projects/*/*.cpp)
foreach(filename ${projects})
  get_filename_component(target ${filename} NAME_WE)
  add_executable(${target} ${filename} )
  target_link_libraries(${target} ${libraries} )
endforeach()


#PRINT DIRECTORIES
get_property(link_dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
message("VERSOR LINK DIRECTORES")
foreach(d ${link_dirs})
  message(STATUS "link dir='${d}'")
endforeach()
get_property(inc_dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message("VERSOR INCLUDE DIRECTORES")
foreach(d ${inc_dirs})
  message(STATUS "include dir='${d}'")
endforeach()

