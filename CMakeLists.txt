#CMAKE FOR The Versor Geometric Algebra Mathematics Library

#VERSION
cmake_minimum_required(VERSION 2.8)

#POLICIES
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 OLD)
endif(COMMAND cmake_policy)

#PROJECT
project(VERSOR)

#optional flags for building graphics and examples
set(BUILD_GRAPHICS OFF CACHE BOOL "build graphics")
set(BUILD_EXAMPLES OFF CACHE BOOL "build examples")
set(BUILD_SCRATCH OFF CACHE BOOL "build scratch")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -U__STRICT_ANSI__ -O3 -ftemplate-depth-1200 -Wno-switch -Wno-deprecated-declarations -Wno-int-to-pointer-cast")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -U__STRICT_ANSI__ -nostdinc++ -O3 -ftemplate-depth-1200 -Wno-switch -Wno-int-to-pointer-cast")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -ftemplate-depth-1200 -Wno-switch -Wno-deprecated-declarations -Wno-int-to-pointer-cast")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdinc++ -O3 -ftemplate-depth-1200 -Wno-switch -Wno-int-to-pointer-cast")


#All .cpp files in src/space get built into libvsr
file(GLOB core src/space/*.cpp)
add_library(vsr ${core})
link_directories(${PROJECT_BINARY_DIR})
#PROJECT INCLUDE AND LINK
target_include_directories(vsr PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(vsr PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(vsr PUBLIC ${PROJECT_SOURCE_DIR}/include/vsr)
target_include_directories(vsr PUBLIC ${PROJECT_SOURCE_DIR}/include/vsr/detail)
target_include_directories(vsr PUBLIC ${PROJECT_SOURCE_DIR}/include/vsr/space)
target_include_directories(vsr PUBLIC ${PROJECT_SOURCE_DIR}/include/vsr/form)
target_include_directories(vsr PUBLIC ${PROJECT_SOURCE_DIR}/include/vsr/util)



#CORE TEST TARGETS -- all files in tests get built to test math library functionality
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
file(GLOB tests tests/*.cpp)
foreach(filename ${tests})
  get_filename_component(target ${filename} NAME_WE)
  add_executable(${target} ${filename} )
  target_link_libraries(${target} vsr )
endforeach()

#define ${vsr_libraries}
set(vsr_libraries vsr)


#Add Headers as Custom Target for Editing in IDES (such as QT Creator, etc)
FILE(GLOB_RECURSE HeaderFiles "${PROJECT_SOURCE_DIR}/include/*")
add_custom_target(headers SOURCES ${HeaderFiles})
#Add scripts as custom target for same reason
FILE(GLOB ScriptFiles "${PROJECT_SOURCE_DIR}/*.sh")
add_custom_target(scr SOURCES ${ScriptFiles})

if(${BUILD_GRAPHICS})

  #OPTIONAL ADD GFX AS SUBDIRECTORY TO BUILD GRAPHICS DEMOS
  add_subdirectory(ext/gfx)

  #GET INCLUDE DIRECTORIES, LINK DIRECTORIES, AND LINKED LIBS
  get_property(GFX_LIBRARIES GLOBAL PROPERTY GFX_LIBRARIES)
  get_property(GFX_INCLUDE_DIR GLOBAL PROPERTY GFX_INCLUDE_DIR)
  get_property(GFX_LINK_DIR GLOBAL PROPERTY GFX_LINK_DIR)

  include_directories(${GFX_INCLUDE_DIR})
  link_directories(${GFX_LINK_DIR})

  file(GLOB graphicsGL src/draw/GL/*.cpp)
  file(GLOB graphicsGLES src/draw/GLES/*.cpp)

  #todo: change this flag name to ${GLES} Flag
  if(${RASPBERRY})
    add_library(vsrGfx ${graphicsGLES}) 
  else()
    add_library(vsrGfx ${graphicsGL} ${graphicsGLES})
  endif()

  target_include_directories(vsrGfx PUBLIC ${PROJECT_SOURCE_DIR}/include/vsr)
  target_include_directories(vsrGfx PUBLIC ${PROJECT_SOURCE_DIR}/include/vsr/draw)

  set(vsr_libraries  ${vsr_libraries} vsrGfx)
  set(libraries ${vsr_libraries} ${GFX_LIBRARIES}  )

  file(GLOB examples examples/*.cpp)
  foreach(filename ${examples})
    get_filename_component(target ${filename} NAME_WE)
    if (${BUILD_EXAMPLES})
      add_executable(${target} ${filename} )
    else()
      add_executable(${target} EXCLUDE_FROM_ALL ${filename} )
    endif()
    target_link_libraries(${target} ${libraries})
  endforeach()
  add_subdirectory(scratch)

endif()


get_property(link_dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
get_property(inc_dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#set_property(GLOBAL PROPERTY VSR_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set_property(GLOBAL PROPERTY VSR_LIBRARIES ${vsr_libraries})
set_property(GLOBAL PROPERTY VSR_INCLUDE_DIR "${inc_dirs}")
set_property(GLOBAL PROPERTY VSR_LINK_DIR "${link_dirs}")


##PRINT DIRECTORIES
#message("VERSOR LINK DIRECTORES")
#foreach(d ${link_dirs})
#  message(STATUS "link dir='${d}'")
#endforeach()
#message("VERSOR INCLUDE DIRECTORES")
#foreach(d ${inc_dirs})
#  message(STATUS "include dir='${d}'")
#endforeach()


