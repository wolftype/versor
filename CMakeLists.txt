#CMAKE FOR The Versor Geometric Algebra Mathematics Library
#github.com/wolftype/versor.git

#cmake version
cmake_minimum_required(VERSION 3.10)

#PROJECT
project(versor
        VERSION 0.2.0
        DESCRIPTION "A (fast) Generic C++ library for Geometric Algebras, including Euclidean, Projective, Conformal, Spacetime (etc)."
        HOMEPAGE_URL "http://versor.mat.ucsb.edu/"
        LANGUAGES CXX C)

#support for packaging
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

#POLICIES
#if(COMMAND cmake_policy)
#  cmake_policy(SET CMP0003 OLD)
#endif(COMMAND cmake_policy)
set(OpenGL_GL_PREFERENCE "GLVND")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  SET( LINUX 1)
  message ("LINUX" )
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET( APPLE 1 )
  message ("APPLE")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  SET( WINDOWS 1)
  message ("WINDOWS")
endif()

#optional flags and their defaults for building graphics and examples
set(BUILD_GRAPHICS OFF CACHE BOOL "build graphics")
set(BUILD_EXAMPLES OFF CACHE BOOL "build examples")

set(USE_DOUBLE_PRECISION ON CACHE BOOL "double precision")

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

#some additional flags
set (VERSOR_FLAGS -ftemplate-depth-1200)

if (NOT WINDOWS)
  list(APPEND VERSOR_FLAGS
    -Wno-switch
    -Wno-deprecated-declarations
    -Wno-int-to-pointer-cast
)
endif()

#double or float precision
if (${USE_DOUBLE_PRECISION})
  message ("BUILDING VERSOR USING DOUBLE PRECISION (DEFAULT)")
  set (VERSOR_FLAGS ${VERSOR_FLAGS} -DVSR_PRECISION_DOUBLE)
else()
  message ("BUILDING VERSOR USING FLOAT PRECISION")
  set (VERSOR_FLAGS ${VERSOR_FLAGS} -DVSR_PRECISION_FLOAT)
endif()

#default release type if none specified
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE RelWithDebInfo)
endif ()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(BUILD_TYPE_FLAGS
    -g
    -O0
  )
elseif(CMAKE_BUILD_TYPE MATCHES Release)
  set(BUILD_TYPE_FLAGS
    -g0
    -O3
  )
elseif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  set(BUILD_TYPE_FLAGS
    -g
    -O3
  )
else()
  message(FATAL_ERROR "invalid build type or failure to set a build type.  specify Release, Debug, or RelWithDebInfo")
endif()

# The static library (and dependencies).
set(vsr_libraries vsr)
# But we compile and install the shared library too.
set(_install_targets ${vsr_libraries} vsr_shared)

#All .cpp files in src/space get built into libvsr.a and libvsr_shared.so.
file(GLOB core src/space/*.cpp)

add_library(vsr ${core})
add_library(vsr_shared SHARED ${core})

# Even we don't install, we want to be able to use this project from the binary directory.
set(_OUT_LIB ${CMAKE_BINARY_DIR}/lib)
set(_OUT_BIN ${CMAKE_BINARY_DIR}/bin)

foreach(target ${_install_targets})
  target_include_directories(${target}
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
  )
  # Propagate the selected precision as a compile definition to dependents.
  if (USE_DOUBLE_PRECISION)
    target_compile_definitions(${target} PUBLIC VSR_PRECISION_DOUBLE)
  else()
    target_compile_definitions(${target} PUBLIC VSR_PRECISION_FLOAT)
  endif()
  set_target_properties(${target} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    ARCHIVE_OUTPUT_DIRECTORY "${_OUT_LIB}"  # .a, .lib
    LIBRARY_OUTPUT_DIRECTORY "${_OUT_LIB}"  # .so, .dylib
    RUNTIME_OUTPUT_DIRECTORY "${_OUT_BIN}"  # .dll/.so loader stubs, executables
  )
  if (CMAKE_CONFIGURATION_TYPES)  # multi-config generators
    foreach(cfg IN LISTS CMAKE_CONFIGURATION_TYPES)
      string(TOUPPER "${cfg}" cfgU)
      set_target_properties(${target} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_${cfgU} "${_OUT_LIB}"
        LIBRARY_OUTPUT_DIRECTORY_${cfgU} "${_OUT_LIB}"
        RUNTIME_OUTPUT_DIRECTORY_${cfgU} "${_OUT_BIN}"
      )
    endforeach()
  endif()
  add_library(versor::${target} ALIAS ${target})
  target_compile_options(${target}
    PRIVATE
      ${BUILD_TYPE_FLAGS}
      ${VERSOR_FLAGS}
  )
endforeach()

# Build-tree output dirs (also for multi-config generators).
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${_OUT_LIB})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${_OUT_LIB})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${_OUT_BIN})
if (CMAKE_CONFIGURATION_TYPES)
  foreach (cfg IN LISTS CMAKE_CONFIGURATION_TYPES)
    string (TOUPPER "${cfg}" cfgU)
    set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${cfgU} ${_OUT_LIB})
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfgU} ${_OUT_LIB})
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfgU} ${_OUT_BIN})
  endforeach ()
endif ()

if (APPLE)
set_target_properties(vsr_shared PROPERTIES BUNDLE TRUE)
else()
set_target_properties(vsr_shared PROPERTIES BUNDLE FALSE)
endif()

#Add Headers as Custom Target for Editing in IDES (such as QT Creator, etc)
FILE(GLOB_RECURSE HeaderFiles "${PROJECT_SOURCE_DIR}/include/*")
add_custom_target(headers SOURCES ${HeaderFiles})
#Add scripts as custom target for same reason
FILE(GLOB ScriptFiles "${PROJECT_SOURCE_DIR}/*.sh")
add_custom_target(scr SOURCES ${ScriptFiles})

#build googletest
# Prevent overriding the parent project's compiler/linker
# settings on Windows
enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(ext/gtest EXCLUDE_FROM_ALL)
#get_property(GTEST_INCLUDE_DIR GLOBAL PROPERTY gtest_build_include_dirs)
include_directories(SYSTEM ${gtest_SOURCE_DIR} ${gtest_SOURCE_DIR}/include)
file(GLOB tests tests/*.cpp)
foreach(filename ${tests})
  get_filename_component(target ${filename} NAME_WE)
  add_executable(${target} ${filename} )
  target_link_libraries(${target} gtest_main)
  target_include_directories(${target} PRIVATE ${CMAKE_SOURCE_DIR}/include)
  add_test (NAME ${target}_test COMMAND ${target})
endforeach()

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
    target_link_libraries(vsrGfx vsr)
  endif()

  set(vsr_libraries ${vsr_libraries} vsrGfx)
  set(libraries ${vsr_libraries} ${GFX_LIBRARIES})
  list(APPEND _install_targets vsrGfx)

  message ("LIBBBBS: ")
  foreach(lib ${libraries})
    message (${lib})
  endforeach()

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

else ()

  set(libraries ${vsr_libraries})

  #potentially build examples
  set (examples examples/xAlgebra.cpp)
  foreach(filename ${examples})
    message (${filename})
    get_filename_component(target ${filename} NAME_WE)
    if (${BUILD_EXAMPLES})
      add_executable(${target} ${filename} )
    else()
      add_executable(${target} EXCLUDE_FROM_ALL ${filename} )
    endif()
    target_link_libraries(${target} ${libraries})
  endforeach()

endif()

# Backward compatibility only for projects that expect GLOBAL VSR_* variables to be set.
# Normally one should not use this, just link with versor::vsr (or versor::vsr_shared).
option(VERSOR_EXPORT_LEGACY_GLOBALS "Export old-style GLOBAL properties" ON)
if (VERSOR_EXPORT_LEGACY_GLOBALS)

  # Build a concrete include-dir list for legacy consumers.
  set (inc_dirs)
  list (APPEND inc_dirs
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}
  )
  if (TARGET gtest OR TARGET gtest_main)
    list (APPEND inc_dirs
      ${gtest_SOURCE_DIR}
      ${gtest_SOURCE_DIR}/include
    )
  endif ()
  if (BUILD_GRAPHICS)
    list (APPEND inc_dirs ${GFX_INCLUDE_DIR})
  endif ()

  set (link_dirs ${_OUT_LIB})
  if (BUILD_GRAPHICS)
    list (APPEND link_dirs ${GFX_LINK_DIR})
  endif ()

  list (REMOVE_DUPLICATES inc_dirs)
  list (REMOVE_DUPLICATES link_dirs)

  set_property(GLOBAL PROPERTY VSR_LIBRARIES   ${vsr_libraries})
  set_property(GLOBAL PROPERTY VSR_INCLUDE_DIR ${inc_dirs})
  set_property(GLOBAL PROPERTY VSR_LINK_DIR    ${link_dirs})
  set_property(GLOBAL PROPERTY VSR_FLAGS       ${VERSOR_FLAGS})

  message(DEBUG "VSR_LIBRARIES = \"${vsr_libraries}\"")
  message(DEBUG "VSR_INCLUDE_DIR = \"${inc_dirs}\"")
  message(DEBUG "VSR_LINK_DIR = \"${link_dirs}\"")
  message(DEBUG "VSR_FLAGS = \"${VERSOR_FLAGS}\"")

  # Legacy projects *might* depend on this?
  include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)
  link_directories(${_OUT_LIB})

  ##PRINT DIRECTORIES
  message("VERSOR LINK DIRECTORES")
  foreach(d ${link_dirs})
    message(STATUS "link dir='${d}'")
  endforeach()
  message("VERSOR INCLUDE DIRECTORES")
  foreach(d ${inc_dirs})
    message(STATUS "include dir='${d}'")
  endforeach()
endif ()

# The targets that we install.
install(TARGETS ${_install_targets}
  EXPORT versorTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

foreach(lib ${_install_targets})
  message(STATUS "libraries='${lib}'")
endforeach()

# Install everything from include/.
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export targets for install-tree.
install(EXPORT versorTargets
  NAMESPACE versor::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/versor
)

# Also export for build-tree (optional but handy for superbuilds).
export(EXPORT versorTargets
  NAMESPACE versor::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/versorTargets.cmake"
)

# Generate and install versorConfig.cmake and versorConfigVersion.cmake.
configure_package_config_file(
  "${CMAKE_CURRENT_LIST_DIR}/cmake/versorConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/versorConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/versor"
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/versorConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/versorConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/versorConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/versor"
)
