cmake_minimum_required(VERSION 2.6)
project(neper_s C)

option(HAVE_GSL         "Use the GNU Scientific Library(GSL)" ON )

include_directories("${PROJECT_BINARY_DIR}")

add_definitions(-Wall -W -g -O3)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})

add_library(neper_s
neper_s.c
nes_utils/nes_utils1.c
nes_input/nes_input1.c
nes_input/nes_input2.c
nes_input/nes_input3.c
nes_sim/nes_sim1.c
nes_sim/nes_sim2.c
nes_sim/nes_sim3.c
nes_sim/nes_sim_fepxparse/nes_sim_fepxparse1.c
nes_sim/nes_sim_write/nes_sim_write1.c
nes_sim/nes_sim_write/nes_sim_write2.c
nes_sim/nes_sim_write/nes_sim_write3.c
nes_sim/nes_sim_write/nes_sim_write4.c
nes_pproc/nes_pproc1.c
nes_pproc/nes_pproc2.c
nes_pproc/nes_pproc_entity/nes_pproc_entity1.c
nes_pproc/nes_pproc_entity/nes_pproc_entity2.c
)

set(EXTRA_LIBS ${EXTRA_LIBS} ut)
set(EXTRA_LIBS ${EXTRA_LIBS} orilib)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_library(MATHLIB m)
if(MATHLIB)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${MATHLIB})
endif()

if(HAVE_GSL)
  find_package(GSL REQUIRED)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${GSL_LIBRARIES})
  include_directories(${GSL_INCLUDE_DIRS})
endif()

if(HAVE_LIBSCOTCH)
  find_package(LIBSCOTCH REQUIRED)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${Scotch_LIBRARIES})
  include_directories(${Scotch_INCLUDE_DIRS})
  find_package(Threads REQUIRED)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${CMAKE_THREAD_LIBS_INIT})
endif()

if(HAVE_OPENMP)
  find_package(OpenMP REQUIRED)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${OpenMP_C_FLAGS})
  add_definitions(${OpenMP_C_FLAGS})
endif()

if(HAVE_NLOPT)                                     # NLOPT
  find_package(NLOPT REQUIRED)                      # NLOPT
  set(EXTRA_LIBS ${EXTRA_LIBS} ${NLOPT_LIBRARIES}) # NLOPT
  include_directories(${NLOPT_INCLUDE_DIRS})       # NLOPT
endif()

target_link_libraries(neper_s ${EXTRA_LIBS})
