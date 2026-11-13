# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# Builds the subset of GSL 2.7.1 used by Neper as a static library (see
# README.md).

project(gsl C)

# GSL sources include their headers as <gsl/gsl_*.h>; GSL's build system
# gathers all headers into a gsl/ directory, which we mimic here.
file(GLOB GSL_HEADERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
     "${CMAKE_CURRENT_SOURCE_DIR}/gsl_*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/*/gsl_*.h")
foreach(header ${GSL_HEADERS})
  get_filename_component(name ${header} NAME)
  configure_file(${header} "${CMAKE_CURRENT_BINARY_DIR}/gsl/${name}" COPYONLY)
endforeach()

set(GSL_SOURCES
  blas/blas.c
  block/init.c
  cblas/caxpy.c
  cblas/ccopy.c
  cblas/cdotc_sub.c
  cblas/cdotu_sub.c
  cblas/cgemm.c
  cblas/cgemv.c
  cblas/cgerc.c
  cblas/cgeru.c
  cblas/chemm.c
  cblas/chemv.c
  cblas/cher.c
  cblas/cher2.c
  cblas/cher2k.c
  cblas/cherk.c
  cblas/cscal.c
  cblas/csscal.c
  cblas/cswap.c
  cblas/csymm.c
  cblas/csyr2k.c
  cblas/csyrk.c
  cblas/ctrmm.c
  cblas/ctrmv.c
  cblas/ctrsm.c
  cblas/ctrsv.c
  cblas/dasum.c
  cblas/daxpy.c
  cblas/dcopy.c
  cblas/ddot.c
  cblas/dgemm.c
  cblas/dgemv.c
  cblas/dger.c
  cblas/dnrm2.c
  cblas/drot.c
  cblas/drotg.c
  cblas/drotm.c
  cblas/drotmg.c
  cblas/dscal.c
  cblas/dsdot.c
  cblas/dswap.c
  cblas/dsymm.c
  cblas/dsymv.c
  cblas/dsyr.c
  cblas/dsyr2.c
  cblas/dsyr2k.c
  cblas/dsyrk.c
  cblas/dtrmm.c
  cblas/dtrmv.c
  cblas/dtrsm.c
  cblas/dtrsv.c
  cblas/dzasum.c
  cblas/dznrm2.c
  cblas/icamax.c
  cblas/idamax.c
  cblas/isamax.c
  cblas/izamax.c
  cblas/sasum.c
  cblas/saxpy.c
  cblas/scasum.c
  cblas/scnrm2.c
  cblas/scopy.c
  cblas/sdot.c
  cblas/sdsdot.c
  cblas/sgemm.c
  cblas/sgemv.c
  cblas/sger.c
  cblas/snrm2.c
  cblas/srot.c
  cblas/srotg.c
  cblas/srotm.c
  cblas/srotmg.c
  cblas/sscal.c
  cblas/sswap.c
  cblas/ssymm.c
  cblas/ssymv.c
  cblas/ssyr.c
  cblas/ssyr2.c
  cblas/ssyr2k.c
  cblas/ssyrk.c
  cblas/strmm.c
  cblas/strmv.c
  cblas/strsm.c
  cblas/strsv.c
  cblas/xerbla.c
  cblas/zaxpy.c
  cblas/zcopy.c
  cblas/zdotc_sub.c
  cblas/zdotu_sub.c
  cblas/zdscal.c
  cblas/zgemm.c
  cblas/zgemv.c
  cblas/zgerc.c
  cblas/zgeru.c
  cblas/zhemm.c
  cblas/zhemv.c
  cblas/zher.c
  cblas/zher2.c
  cblas/zher2k.c
  cblas/zherk.c
  cblas/zscal.c
  cblas/zswap.c
  cblas/zsymm.c
  cblas/zsyr2k.c
  cblas/zsyrk.c
  cblas/ztrmm.c
  cblas/ztrmv.c
  cblas/ztrsm.c
  cblas/ztrsv.c
  complex/math.c
  err/error.c
  err/stream.c
  interpolation/accel.c
  interpolation/cspline.c
  interpolation/inline.c
  interpolation/interp.c
  interpolation/linear.c
  linalg/invtri.c
  linalg/lu.c
  linalg/tridiag.c
  linalg/trimult.c
  matrix/copy.c
  matrix/init.c
  matrix/matrix.c
  matrix/rowcol.c
  matrix/submatrix.c
  permutation/init.c
  permutation/permute.c
  poly/zsolve.c
  poly/zsolve_init.c
  randist/gauss.c
  randist/shuffle.c
  rng/inline.c
  rng/mt.c
  rng/ranlxd.c
  rng/rng.c
  specfunc/elementary.c
  specfunc/erfc.c
  specfunc/exp.c
  specfunc/gamma.c
  specfunc/log.c
  specfunc/psi.c
  specfunc/trig.c
  specfunc/zeta.c
  sys/coerce.c
  sys/fdiv.c
  sys/infnan.c
  vector/copy.c
  vector/init.c
  vector/subvector.c
  vector/vector.c
  vector/view.c
)

add_library(gsl STATIC ${GSL_SOURCES})

set_target_properties(gsl PROPERTIES C_STANDARD 99 POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(gsl PRIVATE HAVE_CONFIG_H)
target_compile_options(gsl PRIVATE -w)
target_include_directories(gsl
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
  PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

find_library(MATHLIB m)
if(MATHLIB)
  target_link_libraries(gsl PUBLIC ${MATHLIB})
endif()
