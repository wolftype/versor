#CMAKE FILE FOR SCRATCH DIRECTORY OF VERSOR GEOMETRIC ALGEBRA LIBRARY

#VERSION
cmake_minimum_required(VERSION 2.8)

#POLICIES
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 OLD)
endif(COMMAND cmake_policy)

#PROJECT
project(VERSOR_SCRATCH)

file(GLOB projects projects/*/*.cpp)
foreach(filename ${projects})
  get_filename_component(target ${filename} NAME_WE)
  add_executable(${target} EXCLUDE_FROM_ALL ${filename} )
  target_link_libraries(${target} ${libraries} )
endforeach()
