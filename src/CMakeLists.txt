# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

cmake_minimum_required(VERSION 3.0...3.10)
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

project(neper VERSION 7.0.0)

# Version: the version above and a suffix, "-dev" until the release (neperfepx-release sets the
# release's version and an empty suffix, then the next version and "-dev" on devel).  A build
# also shows the git hash and the commit date of its sources (cmake/BuildInfo.cmake).
set(NEPER_VERSION_SUFFIX "")
set(NEPER_VERSION \"${PROJECT_VERSION}${NEPER_VERSION_SUFFIX}\")

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8.1)
  message (SEND_ERROR "  GCC 4.8.1 or higher is required.  You are using version ${CMAKE_CXX_COMPILER_VERSION}")
endif()

set(ENABLE_PROFILING OFF CACHE BOOL "Build with gprof profiling support (-pg)")

# Set the build type for appropriate compile flags.  When profiling, default to
# RelWithDebInfo (optimized, with symbols) rather than Release; an explicit
# -DCMAKE_BUILD_TYPE=... always takes precedence.
if(NOT CMAKE_BUILD_TYPE)
  if(ENABLE_PROFILING)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build: Debug Release RelWithDebInfo" FORCE)
  else()
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build: Debug Release RelWithDebInfo" FORCE)
  endif()
endif()

set(BUILD_TESTING_MODE "Normal" CACHE STRING "Choose the testing mode: Normal Minimal Writing")

# should we use the optional libraries?
set(HAVE_OPENMP    ON  CACHE BOOL "Use OpenMP")
set(HAVE_GSL       ON  CACHE BOOL "Use GSL")
set(HAVE_MUPARSER  ON  CACHE BOOL "Use muParser") # sets to ON in ut, etc.
set(HAVE_OPENGJK   ON  CACHE BOOL "Use openGJK") # sets to ON in ut, etc.
set(HAVE_LIBSCOTCH ON  CACHE BOOL "Use Scotch") # sets to ON in ut, etc.
set(HAVE_PNG       OFF CACHE BOOL "Use the libpng library") # sets to OFF in ut, etc.
set(CMAKE_INSTALL_COMPLETION OFF CACHE BOOL "Install Bash completion files")
set(FORCE_BUILTIN_LIBSCOTCH OFF CACHE BOOL "Force to use the built-in libscotch")
set(FORCE_BUILTIN_GSL OFF CACHE BOOL "Force to use the built-in GSL")
set(REAL_PRINT_FORMAT \"%.12f\" CACHE STRING "Printing format of real numbers")
set(REAL_PRINT_FORMAT3 \"%15.12f\" CACHE STRING "Printing format of real numbers (15 characters)")
set(REAL_PRINT_FORMAT5 \"%17.12f\" CACHE STRING "Printing format of real numbers (17 characters)")

if (CMAKE_INSTALL_COMPLETION)
  if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set (CMAKE_INSTALL_COMPLETION_PREFIX "/usr/share" CACHE STRING "Installation directory of Bash completion files")
  elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set (CMAKE_INSTALL_COMPLETION_PREFIX "/usr/local/etc" CACHE STRING "Installation directory of Bash completion files")
  endif()
endif()

function(message)
  if(NOT MESSAGE_QUIET)
    _message(${ARGN})
  endif()
endfunction()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

if(ENABLE_PROFILING)
  if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang" AND NOT APPLE)
    message(STATUS "Profiling enabled (gprof): adding -pg")
    add_compile_options(-pg)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

    # gprof does not work reliably with position-independent executables, which
    # are the default on many recent Linux distributions.
    include(CheckCCompilerFlag)
    check_c_compiler_flag("-no-pie" HAVE_NO_PIE_FLAG)
    if(HAVE_NO_PIE_FLAG)
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
    endif()
    unset(HAVE_NO_PIE_FLAG CACHE) # hide from ccmake

    if(HAVE_OPENMP)
      message(WARNING "gprof only profiles the main thread: results are unreliable with OpenMP.  Consider -DHAVE_OPENMP=OFF.")
    endif()
  else()
    message(WARNING "ENABLE_PROFILING (gprof) is only supported with GCC/Clang on Linux; ignored.")
  endif()
endif()

# Gmsh library (mandatory)
find_path(GMSH_INCLUDE_DIR gmshc.h)
find_library(GMSH_LIBRARY gmsh)
if(GMSH_INCLUDE_DIR AND GMSH_LIBRARY)
  # version >= 4.9 (size callback with the lc argument)
  file(STRINGS "${GMSH_INCLUDE_DIR}/gmshc.h" GMSH_VERSION_LINES
       REGEX "#define GMSH_API_VERSION_(MAJOR|MINOR) ")
  string(REGEX REPLACE ".*MAJOR ([0-9]+).*" "\\1" GMSH_VERSION_MAJOR "${GMSH_VERSION_LINES}")
  string(REGEX REPLACE ".*MINOR ([0-9]+).*" "\\1" GMSH_VERSION_MINOR "${GMSH_VERSION_LINES}")
  set(GMSH_VERSION "${GMSH_VERSION_MAJOR}.${GMSH_VERSION_MINOR}")
  if(GMSH_VERSION VERSION_LESS 4.9)
    message(FATAL_ERROR "Gmsh library version ${GMSH_VERSION} found (${GMSH_LIBRARY}), but version 4.9 or higher is required - update it or set CMAKE_PREFIX_PATH")
  endif()
  message("-- Found Gmsh library: ${GMSH_LIBRARY} (version ${GMSH_VERSION})")
  include_directories(${GMSH_INCLUDE_DIR})
else()
  message(FATAL_ERROR "Could NOT find the Gmsh library (gmshc.h and libgmsh) - install it (e.g., package libgmsh-dev) or set CMAKE_PREFIX_PATH")
endif()

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
include_directories("${PROJECT_SOURCE_DIR}/neut/neut_struct")
include_directories("${PROJECT_SOURCE_DIR}/contrib/ut/src")
include_directories("${PROJECT_SOURCE_DIR}/contrib/orilib")
include_directories("${PROJECT_SOURCE_DIR}/contrib/tinycolormap")
include_directories("${PROJECT_SOURCE_DIR}/contrib/nanoflann")

if (HAVE_MUPARSER)
  include_directories("${PROJECT_SOURCE_DIR}/contrib/muparser")
  set(MUPARSER_SOURCE_DIR "${PROJECT_SOURCE_DIR}/contrib/muparser")
  set(MUPARSER_BINARY_DIR "${PROJECT_BINARY_DIR}/contrib/muparser")
  set(MUPARSER_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/contrib/muparser/include")
endif()

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

# GSL: system library if available, built-in otherwise (see contrib/gsl)
if(HAVE_GSL)
  if (FORCE_BUILTIN_GSL)
    unset(GSL_INCLUDE_DIR CACHE)
    unset(GSL_LIBRARY CACHE)
    unset(GSL_LIBRARY_DEBUG CACHE)
    unset(GSL_CBLAS_LIBRARY CACHE)
    unset(GSL_CBLAS_LIBRARY_DEBUG CACHE)
  else()
    find_package(GSL QUIET)
  endif()

  if (GSL_FOUND AND NOT FORCE_BUILTIN_GSL)
    message("-- Found GSL: ${GSL_LIBRARY} (version ${GSL_VERSION})")
    # A shared GSL library is linked to its CBLAS library (libgslcblas or
    # another one, e.g. in conda), and Neper does not call CBLAS directly, so
    # that linking to a CBLAS library is not needed (and would tie Neper to
    # it); a static GSL library needs it.
    set(GSL_LIBRARIES ${GSL_LIBRARY})
    if (GSL_LIBRARY MATCHES "\\.a$")
      list(APPEND GSL_LIBRARIES ${GSL_CBLAS_LIBRARY})
    endif()
  else()
    if (NOT FORCE_BUILTIN_GSL)
      message("-- Could NOT find system GSL - using built-in")
    else()
      message("-- Ignored system GSL - using built-in")
    endif()
    add_subdirectory(contrib/gsl EXCLUDE_FROM_ALL)
    set(GSL_LIBRARIES gsl)
    set(GSL_INCLUDE_DIRS "${PROJECT_BINARY_DIR}/contrib/gsl")
  endif()
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
    add_subdirectory(contrib/scotch EXCLUDE_FROM_ALL)
    find_package(Threads REQUIRED QUIET)
    list (APPEND EXTRA_LIBS ${CMAKE_THREAD_LIBS_INIT})
    unset (Scotch_INCLUDE_DIR CACHE)
    set(scotch_LIBRARY scotch)
    set(scotcherr_LIBRARY scotcherr)
    set(scotcherrexit_LIBRARY scotcherrexit)
    set(Scotch_LIBRARIES ${scotch_LIBRARY} ${scotcherr_LIBRARY}
                         ${scotcherrexit_LIBRARY})
    list(APPEND EXTRA_LIBS scotch scotcherr scotcherrexit)
    include_directories("${PROJECT_BINARY_DIR}/contrib/scotch") # scotch.h
    set(BUILTIN_LIBSCOTCH ON)
  endif()
endif()

add_definitions(-Wall -W -Wno-unknown-pragmas -Wno-deprecated-declarations)
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 9.0)
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-copy")
endif()

set(BUILD_SHARED_LIBS OFF)

set(MESSAGE_QUIET ON)

add_subdirectory(contrib/openGJK EXCLUDE_FROM_ALL)
if (HAVE_MUPARSER)
  add_subdirectory(contrib/muparser EXCLUDE_FROM_ALL)
endif()
add_subdirectory(contrib/ut/src EXCLUDE_FROM_ALL)
add_subdirectory(contrib/orilib EXCLUDE_FROM_ALL)
add_subdirectory(neut)
set(MESSAGE_QUIET OFF)
add_subdirectory(neper_t)
add_subdirectory(neper_m)
add_subdirectory(neper_v)
add_subdirectory(neper_s)

set(MESSAGE_QUIET OFF)

add_executable(neper neper.c)

# The build information (git hash and commit date of the sources, cmake/BuildInfo.cmake), written
# into neper_build_info.h at every build (the file is rewritten only when it changes).
set(neper_build_info_command ${CMAKE_COMMAND} -DSOURCE_DIR=${PROJECT_SOURCE_DIR}/..
    -DOUTPUT=${PROJECT_BINARY_DIR}/neper_build_info.h -DVERSION=${PROJECT_VERSION}
    -DSUFFIX=${NEPER_VERSION_SUFFIX} -P ${PROJECT_SOURCE_DIR}/cmake/BuildInfo.cmake)
execute_process(COMMAND ${neper_build_info_command})
add_custom_target(neper_build_info COMMAND ${neper_build_info_command}
                  BYPRODUCTS ${PROJECT_BINARY_DIR}/neper_build_info.h VERBATIM)
add_dependencies(neper neper_build_info)

# the built-in scotch.h is generated at build time (and is included by
# neut_mesh_op.h)
if(BUILTIN_LIBSCOTCH)
  foreach(target neut neper_t neper_m neper_v neper_s neper)
    add_dependencies(${target} scotch_h)
  endforeach()
endif()

list(APPEND EXTRA_LIBS openGJKlib)
if (HAVE_MUPARSER)
  list(APPEND EXTRA_LIBS muparser)
endif()
list(APPEND EXTRA_LIBS ut)
list(APPEND EXTRA_LIBS orilib-lib)
list(APPEND EXTRA_LIBS neut)
list(APPEND EXTRA_LIBS neper_t)
list(APPEND EXTRA_LIBS neper_m)
list(APPEND EXTRA_LIBS neper_v)
list(APPEND EXTRA_LIBS neper_s)
list(APPEND EXTRA_LIBS ${GMSH_LIBRARY})
# (threads: Gmsh worker watchdog, see nem_mesh_gmsh3.c)
find_package(Threads REQUIRED QUIET)
list(APPEND EXTRA_LIBS ${CMAKE_THREAD_LIBS_INIT})

# Hide variables that come from subdirectories #################################
unset(HAVE_GLPK CACHE)           # hide from ccmake (comes from orilib)
unset(VERSION_ACCURATE CACHE)    # hide from ccmake (comes from openGJK)
unset(HAVE_PNG CACHE)            # hide from ccmake (comes from Orilib)
unset(ENABLE_SAMPLES CACHE)
unset(ENABLE_WIDE_CHAR CACHE)

target_link_libraries(neper ${EXTRA_LIBS})

install(TARGETS neper DESTINATION bin
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
               GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# the orilib program (Orilib's functions from the command line, also used by
# the Python interface), built and installed with neper
add_dependencies(neper orilib-prog)
install(PROGRAMS ${CMAKE_BINARY_DIR}/orilib DESTINATION bin
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
               GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/data/
        DESTINATION share/neper
        FILES_MATCHING
        PATTERN "*")

# remove the orientation-space trees written by former installations (they
# are now generated at run time and saved in the cache directory)
install(CODE "
  file(GLOB _neper_trees
       \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/share/neper/orispace/*/*.etree\"
       \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/share/neper/orispace/*/*.ntree\")
  if(_neper_trees)
    message(STATUS \"Removing the orientation-space trees of a former installation\")
    file(REMOVE \${_neper_trees})
  endif()
")

add_custom_target(userinstall
    COMMAND ${CMAKE_COMMAND}
        -DCMAKE_INSTALL_PREFIX=$ENV{HOME}/.local
        -DUSERINSTALL=ON
        -P cmake_install.cmake
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Installing to $HOME/.local"
)
add_dependencies(userinstall neper)

# conda package (recipe in ../conda/recipe), built from the local source tree
# and tested with rattler-build, in conda/<platform>/
find_program(RATTLER_BUILD rattler-build)
mark_as_advanced(RATTLER_BUILD)
if(RATTLER_BUILD)
  add_custom_target(conda
    COMMAND ${RATTLER_BUILD} build -r ${PROJECT_SOURCE_DIR}/../conda/recipe
            -c conda-forge --output-dir ${CMAKE_BINARY_DIR}/conda
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/..
    COMMENT "Building and testing the conda package in ${CMAKE_BINARY_DIR}/conda"
    USES_TERMINAL
    VERBATIM
  )
else()
  add_custom_target(conda
    COMMAND ${CMAKE_COMMAND} -E echo
            "rattler-build not found: install it (e.g., conda install -n base -c conda-forge rattler-build) and rerun cmake."
    COMMAND ${CMAKE_COMMAND} -E false
    VERBATIM
  )
endif()

add_custom_target(devinstall
  COMMAND ${CMAKE_SOURCE_DIR}/data/dev/dev.sh
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

message(STATUS "Neper is set to be installed in: ${CMAKE_INSTALL_PREFIX}/bin")

if(CMAKE_INSTALL_COMPLETION AND NOT USERINSTALL)
  if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set (CMAKE_INSTALL_COMPLETION_PREFIX_FULL "${CMAKE_INSTALL_COMPLETION_PREFIX}/bash-completion/completions")
  elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set (CMAKE_INSTALL_COMPLETION_PREFIX_FULL "${CMAKE_INSTALL_COMPLETION_PREFIX}/bash_completion.d")
  endif()

  install(FILES contrib/bashcomp/neper-completion.bash
          DESTINATION ${CMAKE_INSTALL_COMPLETION_PREFIX_FULL}
          RENAME neper)
endif()

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

  # Python interface (../python), if a Python 3 interpreter is available
  find_package(Python3 COMPONENTS Interpreter QUIET)
  if(Python3_Interpreter_FOUND AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../python/tests)
    add_test(NAME python
             COMMAND ${CMAKE_COMMAND} -E env
                     NEPER_EXECUTABLE=$<TARGET_FILE:neper>
                     ${Python3_EXECUTABLE} -B -m unittest discover
                     -s ${CMAKE_CURRENT_SOURCE_DIR}/../python/tests)
    execute_process(COMMAND ${Python3_EXECUTABLE} -c "import numpy"
                    RESULT_VARIABLE NUMPY_STATUS OUTPUT_QUIET ERROR_QUIET)
    if(NOT NUMPY_STATUS EQUAL 0)
      message(STATUS "NumPy not found by ${Python3_EXECUTABLE}: the Python reader tests will be skipped (set Python3_EXECUTABLE)")
    endif()
  endif()
endif()
