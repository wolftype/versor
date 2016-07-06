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

#OPTIONAL ADD GFX AS SUBDIRECTORY TO BUILD GRAPHICS DEMOS
add_subdirectory(../ext/gfx gfxbuild)

#GET INCLUDE DIRECTORIES, LINK DIRECTORIES, AND LINKED LIBS FROM GFX LIBRARY
get_property(GFX_LIBRARIES GLOBAL PROPERTY GFX_LIBRARIES)
get_property(GFX_INCLUDE_DIR GLOBAL PROPERTY GFX_INCLUDE_DIR)
get_property(GFX_LINK_DIR GLOBAL PROPERTY GFX_LINK_DIR)


#PROJECT INCLUDE AND LINK
include_directories(${PROJECT_SOURCE_DIR}/../ )
include_directories(${PROJECT_SOURCE_DIR}/../include/)
include_directories(${PROJECT_SOURCE_DIR}/../include/vsr/)
include_directories(${PROJECT_SOURCE_DIR}/../include/vsr/detail)
include_directories(${PROJECT_SOURCE_DIR}/../include/vsr/space)
include_directories(${PROJECT_SOURCE_DIR}/../include/vsr/form)
include_directories(${PROJECT_SOURCE_DIR}/../include/vsr/draw)
include_directories(${PROJECT_SOURCE_DIR}/../include/vsr/util)
include_directories(${GFX_INCLUDE_DIR})

#LINK DIRECTORIES
link_directories(${PROJECT_SOURCE_DIR}/../build/)
link_directories(${GFX_LINK_DIR})

#LIBRARIES
set(vsr_libraries vsr vsrGfx)
set(libraries ${vsr_libraries} ${GFX_LIBRARIES}) 

#TARGETS
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
file(GLOB projects projects/*/*.cpp)
foreach(filename ${projects})
  get_filename_component(target ${filename} NAME_WE)
  add_executable(${target} ${filename} )
  target_link_libraries(${target} ${libraries} )
endforeach()


#PRINT DIRECTORIES
get_property(VSR_LINK_DIRECTORIES DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
message("VERSOR LINK DIRECTORES")
foreach(d ${VSR_LINK_DIRECTORIES})
  message(STATUS "VSR_LINK_DIRECTORIES='${d}'")
endforeach()
get_property(VSR_INCLUDE_DIRECTORIES DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message("VERSOR INCLUDE DIRECTORES")
foreach(d ${VSR_INCLUDE_DIRECTORIES})
  message(STATUS "VSR_INCLUDE_DIRECTORIES='${d}'")
endforeach()

