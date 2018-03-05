#CMAKE FOR The Versor Geometric Algebra Mathematics Library
#github.com/wolftype/versor.git

#VERSION
cmake_minimum_required(VERSION 3.0)

#POLICIES
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 OLD)
endif(COMMAND cmake_policy)

#PROJECT
project(VERSOR)

#optional flags and their defaults for building graphics and examples
set(BUILD_GRAPHICS OFF CACHE BOOL "build graphics")
set(BUILD_EXAMPLES OFF CACHE BOOL "build examples")
set(BUILD_SCRATCH OFF CACHE BOOL "build scratch")

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

#some additional flags
set (VERSOR_FLAGS "-ftemplate-depth-1200 -Wno-switch -Wno-deprecated-declarations -Wno-int-to-pointer-cast")

#default release type if none specified
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE RelWithDebInfo)
endif ()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(BUILD_TYPE_FLAGS "-g -O0")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
  set(BUILD_TYPE_FLAGS "-g0 -O3")
elseif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  set(BUILD_TYPE_FLAGS "-g -O3")
else()
  message(FATAL_ERROR "invalid build type or failure to set a build type.  specify Release, Debug, or RelWithDebInfo")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BUILD_TYPE_FLAGS} ${VERSOR_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdinc++ ${BUILD_TYPE_FLAGS} ${VERSOR_FLAGS}")

#All .cpp files in src/space get built into libvsr
file(GLOB core src/space/*.cpp)
add_library(vsr ${core})
link_directories(${CMAKE_SOURCE_DIR}/lib)
set(vsr_libraries vsr)

#project include directories
include_directories(BEFORE ${PROJECT_SOURCE_DIR})
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)

#CORE TEST TARGETS -- all files in tests get built to test math library functionality
#set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
#set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/bin")

file(GLOB tests tests/*.cpp)
foreach(filename ${tests})
  get_filename_component(target ${filename} NAME_WE)
  add_executable(${target} ${filename} )
  target_link_libraries(${target} vsr )
endforeach()

#Add Headers as Custom Target for Editing in IDES (such as QT Creator, etc)
FILE(GLOB_RECURSE HeaderFiles "${PROJECT_SOURCE_DIR}/include/*")
add_custom_target(headers SOURCES ${HeaderFiles})
#Add scripts as custom target for same reason
FILE(GLOB ScriptFiles "${PROJECT_SOURCE_DIR}/*.sh")
add_custom_target(scr SOURCES ${ScriptFiles})

#build libvsrGfx
if(${BUILD_GRAPHICS})

  add_subdirectory(ext/gfx)
  get_property(GFX_LIBRARIES GLOBAL PROPERTY GFX_LIBRARIES)
  get_property(GFX_INCLUDE_DIR GLOBAL PROPERTY GFX_INCLUDE_DIR)
  get_property(GFX_LINK_DIR GLOBAL PROPERTY GFX_LINK_DIR)

  include_directories(${GFX_INCLUDE_DIR})
  link_directories(${GFX_LINK_DIR})

  foreach (lib ${GFX_LIBRARIES})
  message ("GFXLIB:" ${lib})
  endforeach()

  #get all source files
  file(GLOB graphicsGL src/draw/GL/*.cpp)
  file(GLOB graphicsGLES src/draw/GLES/*.cpp)

  #which graphics backend to use
  #todo: change this flag name to ${GLES} Flag
  if(${RASPBERRY})
    add_library(vsrGfx ${graphicsGLES})
  else()
    add_library(vsrGfx ${graphicsGL} ${graphicsGLES})
  endif()

  set(vsr_libraries ${vsr_libraries} vsrGfx)
  set(libraries ${vsr_libraries} ${GFX_LIBRARIES}  )

  #potentially build examples
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
set_property(GLOBAL PROPERTY VSR_LIBRARIES ${vsr_libraries})
set_property(GLOBAL PROPERTY VSR_INCLUDE_DIR "${inc_dirs}")
set_property(GLOBAL PROPERTY VSR_LINK_DIR "${link_dirs}")
set_property(GLOBAL PROPERTY VSR_FLAGS "${VERSOR_FLAGS}")

install(TARGETS ${vsr_libraries}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

foreach(lib ${vsr_libraries})
  message(STATUS "libraries='${lib}'")
endforeach()

install(DIRECTORY include/vsr/detail DESTINATION include/vsr)
install(DIRECTORY include/vsr/draw DESTINATION include/vsr)
install(DIRECTORY include/vsr/form   DESTINATION include/vsr)
install(DIRECTORY include/vsr/space  DESTINATION include/vsr)
install(DIRECTORY include/vsr/util   DESTINATION include/vsr)
install(FILES include/vsr/vsr.h include/vsr/vsr_app.h DESTINATION include/vsr)

##PRINT DIRECTORIES
message("VERSOR LINK DIRECTORES")
foreach(d ${link_dirs})
  message(STATUS "link dir='${d}'")
endforeach()
message("VERSOR INCLUDE DIRECTORES")
foreach(d ${inc_dirs})
  message(STATUS "include dir='${d}'")
endforeach()


