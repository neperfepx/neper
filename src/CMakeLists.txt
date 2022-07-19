# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 3.0)
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

set(NEPER_VERSION \"4.4.2-33\")
project(neper)

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8.1)
  message (SEND_ERROR "  GCC 4.8.1 or higher is required.  You are using version ${CMAKE_CXX_COMPILER_VERSION}")
endif()

# Set the build type for appropriate compile flags
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build: Debug Release" FORCE)
endif()

set(BUILD_TESTING_MODE "Normal" CACHE STRING "Choose the testing mode: Normal Minimal Writing")

# should we use the optional libraries?
set(HAVE_NLOPT     ON  CACHE BOOL "Use NLopt")
set(HAVE_OPENMP    ON  CACHE BOOL "Use OpenMP")
set(HAVE_GSL       ON  CACHE BOOL "Use GSL")
set(HAVE_MUPARSER  ON  CACHE BOOL "Use muParser") # sets to ON in ut, etc.
set(HAVE_OPENGJK   ON  CACHE BOOL "Use openGJK") # sets to ON in ut, etc.
set(HAVE_LIBSCOTCH ON  CACHE BOOL "Use Scotch") # sets to ON in ut, etc.
set(HAVE_PNG       OFF CACHE BOOL "Use the libpng library") # sets to OFF in ut, etc.
set(FORCE_BUILTIN_LIBSCOTCH OFF CACHE BOOL "Force to use the built-in libscotch")
set(FORCE_BUILTIN_NLOPT     OFF CACHE BOOL "Force to use the built-in nlopt")

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  set (CMAKE_INSTALL_PREFIX_COMPLETION "/usr/share" CACHE STRING "Installation directory of Bash completion files")
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  set (CMAKE_INSTALL_PREFIX_COMPLETION "/usr/local/etc" CACHE STRING "Installation directory of Bash completion files")
endif()

function(message)
  if(NOT MESSAGE_QUIET)
    _message(${ARGN})
  endif()
endfunction()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# configure a header file to pass some of the CMake settings to the
# source code
configure_file(
  "${PROJECT_SOURCE_DIR}/neper_config.h.in"
  "${PROJECT_BINARY_DIR}/neper_config.h"
)

################################################################################
# Including directories ########################################################
################################################################################

# allow to #include random files, from anywhere, by specifying its path
# relative to ${PROJECT_SOURCE_DIR}
include_directories("${PROJECT_SOURCE_DIR}")

# path to neper_config.h, which is only in ${PROJECT_BINARY_DIR}
include_directories("${PROJECT_BINARY_DIR}")

# including all subdirectories, whick makes sure that they have access to each
# other's include files (all need ut, neper_* need neut, neut needs neper_t,
# etc.).  The order does not matter.
include_directories("${PROJECT_SOURCE_DIR}/neper_t")
include_directories("${PROJECT_SOURCE_DIR}/neper_m")
include_directories("${PROJECT_SOURCE_DIR}/neper_s")
include_directories("${PROJECT_SOURCE_DIR}/neper_v")
include_directories("${PROJECT_SOURCE_DIR}/neut")
include_directories("${PROJECT_SOURCE_DIR}/neut/neut_structs")
include_directories("${PROJECT_SOURCE_DIR}/contrib/ut/src")
include_directories("${PROJECT_SOURCE_DIR}/contrib/orilib")
include_directories("${PROJECT_SOURCE_DIR}/contrib/tinycolormap")
include_directories("${PROJECT_SOURCE_DIR}/contrib/nanoflann")

include_directories("${PROJECT_SOURCE_DIR}/contrib/muparser")
set(MUPARSER_SOURCE_DIR "${PROJECT_SOURCE_DIR}/contrib/muparser")
set(MUPARSER_BINARY_DIR "${PROJECT_BINARY_DIR}/contrib/muparser")
set(MUPARSER_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/contrib/muparser/include")

# The following include_directories are due to the presence of local
# *_config.h files (built in their subdirectories).  These subdirectories are
# #included by other subdirectories. UT_BINARY_DIR and UT_INCLUDE_DIR are
# useful to orilib.
include_directories("${PROJECT_BINARY_DIR}/contrib/orilib")
include_directories("${PROJECT_BINARY_DIR}/contrib/ut/src")
set(UT_BINARY_DIR "${PROJECT_BINARY_DIR}/contrib/ut/src")
set(UT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/contrib/ut/src")

################################################################################
# Managing dependencies ########################################################
################################################################################

if(HAVE_GSL)
  find_package(GSL REQUIRED)
  list (APPEND EXTRA_LIBS ${GSL_LIBRARIES})
  include_directories(${GSL_INCLUDE_DIRS})
endif()

if(HAVE_OPENMP)
  if(APPLE AND XCODE)
      set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include" CACHE INTERNAL "OpenMP flags for Xcode toolchain.")
      set(OpenMP_CXX_LIB_NAMES "omp" CACHE INTERNAL "OpenMP lib name for Xcode toolchain.")
      set(OpenMP_omp_LIBRARY "/usr/local/opt/libomp/lib/libomp.dylib" CACHE INTERNAL "OpenMP lib name for Xcode toolchain.")
  endif()

  find_package(OpenMP REQUIRED)
  list (APPEND EXTRA_LIBS ${OpenMP_C_FLAGS})
  add_definitions(${OpenMP_C_FLAGS})
endif()

if(HAVE_LIBSCOTCH)
  if (FORCE_BUILTIN_LIBSCOTCH)
    unset(Scotch_INCLUDE_DIR CACHE)
    unset(scotch_LIBRARY CACHE)
    unset(scotcherr_LIBRARY CACHE)
    unset(scotcherrexit_LIBRARY CACHE)
    unset(Scotch_LIBRARIES CACHE)
  endif()

  if (NOT FORCE_BUILTIN_LIBSCOTCH)
    find_package(LIBSCOTCH QUIET)
  endif()

  if (EXISTS "${scotch_LIBRARY}")
    message("-- Found Scotch: ${scotch_LIBRARY}")
    list (APPEND EXTRA_LIBS ${Scotch_LIBRARIES})
    include_directories(${Scotch_INCLUDE_DIRS})
    find_package(Threads REQUIRED QUIET)
    list (APPEND EXTRA_LIBS ${CMAKE_THREAD_LIBS_INIT})
  else (${scotch_LIBRARY})
    if (NOT FORCE_BUILTIN_LIBSCOTCH)
      message("-- Could NOT find system Scotch - using built-in")
    else()
      message("-- Ignored system Scotch - using built-in")
    endif()
    add_subdirectory(contrib/scotch)
    find_package(Threads REQUIRED QUIET)
    list (APPEND EXTRA_LIBS ${CMAKE_THREAD_LIBS_INIT})
    unset (Scotch_INCLUDE_DIR CACHE)
    set(scotch_LIBRARY scotch)
    set(scotcherr_LIBRARY scotcherr)
    set(scotcherrexit_LIBRARY scotcherrexit)
    set(Scotch_LIBRARIES ${scotch_LIBRARY} ${scotcherr_LIBRARY}
                         ${scotcherrexit_LIBRARY})
    list(APPEND EXTRA_LIBS scotch scotcherr scotcherrexit)
    include_directories("${PROJECT_SOURCE_DIR}/contrib/scotch/src/libscotch")
    include_directories("${PROJECT_BINARY_DIR}/contrib/scotch")
  endif()
endif()

if (HAVE_NLOPT)
  if (FORCE_BUILTIN_NLOPT)
    unset(NLOPT_LIBRARY CACHE)
    unset(NLOPT_INCLUDE_DIR CACHE)
  endif()

  if (NOT FORCE_BUILTIN_NLOPT)
    find_package(NLOPT QUIET)
  endif()

  if(EXISTS "${NLOPT_LIBRARIES}")
    message("-- Found NLopt: ${NLOPT_LIBRARIES}")
    list (APPEND EXTRA_LIBS ${NLOPT_LIBRARIES})
    include_directories(${NLOPT_INCLUDE_DIRS})
  else(${NLOPT_LIBRARIES})
    if (NOT FORCE_BUILTIN_NLOPT)
      message("-- Could NOT find system NLopt - using built-in")
    else()
      message("-- Ignored system NLopt - using built-in")
    endif()
    set(NLOPT_CXX ON)
    set(NLOPT_FORTRAN OFF)
    set(NLOPT_GUILE OFF)
    set(NLOPT_MATLAB OFF)
    set(NLOPT_OCTAVE OFF)
    set(NLOPT_PYTHON OFF)
    set(NLOPT_SWIG OFF)
    set(NLOPT_TESTS OFF)
    set(BUILD_SHARED_LIBS OFF)
    set(MESSAGE_QUIET ON)
    add_subdirectory(contrib/nlopt)
    set(MESSAGE_QUIET OFF)
    set(NLOPT_INCLUDE_DIR "${PROJECT_BINARY_DIR}/contrib/nlopt/src/api")
    set(NLOPT_INCLUDE_DIRS "${NLOPT_INCLUDE_DIR}")
    set(NLOPT_LIBRARY "nlopt")
    set(NLOPT_LIBRARIES "nlopt")
    list(APPEND EXTRA_LIBS nlopt)
    include_directories("${PROJECT_SOURCE_DIR}/contrib/nlopt")
    include_directories("${PROJECT_BINARY_DIR}/contrib/nlopt/src/api")
  endif()
endif()

add_definitions(-Wall -W -Wno-unknown-pragmas -Wno-deprecated-declarations)
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.0)
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-copy")
endif()

set(BUILD_SHARED_LIBS OFF)

set(MESSAGE_QUIET ON)

add_subdirectory(contrib/openGJK)
add_subdirectory(contrib/muparser)
add_subdirectory(contrib/ut/src)
add_subdirectory(contrib/orilib)
add_subdirectory(neut)
set(MESSAGE_QUIET OFF)
add_subdirectory(neper_t)
add_subdirectory(neper_m)
add_subdirectory(neper_v)
add_subdirectory(neper_s)

set(MESSAGE_QUIET OFF)

add_executable(neper neper.c)

list(APPEND EXTRA_LIBS openGJKlib)
list(APPEND EXTRA_LIBS muparser)
list(APPEND EXTRA_LIBS ut)
list(APPEND EXTRA_LIBS orilib)
list(APPEND EXTRA_LIBS neut)
list(APPEND EXTRA_LIBS neper_t)
list(APPEND EXTRA_LIBS neper_m)
list(APPEND EXTRA_LIBS neper_v)
list(APPEND EXTRA_LIBS neper_s)

# Hide variables that come from subdirectories #################################
unset(HAVE_GLPK CACHE)           # hide from ccmake (comes from orilib)
unset(VERSION_ACCURATE CACHE)    # hide from ccmake (comes from openGJK)
unset(INSTALL_BIN_DIR CACHE)     # hide from ccmake (comes from NLopt)
unset(INSTALL_CMAKE_DIR CACHE)   # hide from ccmake (comes from NLopt)
unset(INSTALL_DATA_DIR CACHE)    # hide from ccmake (comes from NLopt)
unset(INSTALL_INCLUDE_DIR CACHE) # hide from ccmake (comes from NLopt)
unset(INSTALL_LIB_DIR CACHE)     # hide from ccmake (comes from NLopt)
unset(INSTALL_MAN_DIR CACHE)     # hide from ccmake (comes from NLopt)
unset(THREADLOCAL CACHE)         # hide from ccmake (comes from NLopt)
unset(WITH_THREADLOCAL CACHE)    # hide from ccmake (comes from NLopt)
unset(NLOPT_TESTS CACHE)         # hide from ccmake (comes from NLopt)
unset(HAVE_PNG CACHE)            # hide from ccmake (comes from Orilib)
unset(BUILD_SHARED_LIBS CACHE)   # hide from ccmake (comes from NLopt)

target_link_libraries(neper ${EXTRA_LIBS})

install(TARGETS neper DESTINATION bin
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
	 GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

message(STATUS "Neper is set to be installed in: ${CMAKE_INSTALL_PREFIX}/bin and ${CMAKE_INSTALL_PREFIX_COMPLETION}")

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  set (CMAKE_INSTALL_PREFIX_COMPLETION_FULL "${CMAKE_INSTALL_PREFIX_COMPLETION}/bash-completion/completions")
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  set (CMAKE_INSTALL_PREFIX_COMPLETION_FULL "${CMAKE_INSTALL_PREFIX_COMPLETION}/bash_completion.d")
endif()

install(FILES contrib/bashcomp/neper-completion.bash
        DESTINATION ${CMAKE_INSTALL_PREFIX_COMPLETION_FULL}
        RENAME neper)

# Testing ######################################################################

include(CTest)

file(GLOB_RECURSE tests_t ../tests/T/*/*.cmake)
file(GLOB_RECURSE tests_m ../tests/M/*/*.cmake)
file(GLOB_RECURSE tests_s ../tests/S/*/*.cmake)
file(GLOB_RECURSE tests_v ../tests/V/*/*.cmake)

if(BUILD_TESTING)
  foreach(test ${tests_t} ${tests_m} ${tests_s} ${tests_v})
    get_filename_component(test_name ${test} NAME)
    string(REPLACE ".cmake" "" test_name ${test_name})
    get_filename_component(test_dir ${test} DIRECTORY)
    get_filename_component(test_path ${test} PATH)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/../tests/" "" test_label ${test_dir})
    add_test(NAME ${test_label}
             COMMAND ${CMAKE_COMMAND}
             -Dtest_prog=$<TARGET_FILE:neper>
             -Dtest_mode=${BUILD_TESTING_MODE}
             -P ${test}
             WORKING_DIRECTORY ${test_dir})
  endforeach()
endif()
