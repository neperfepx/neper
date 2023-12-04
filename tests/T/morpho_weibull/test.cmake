# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n from_morpho -morpho "diameq:weibull(0.17,1)" -morphooptistop "itermax==1" -o test)
include(../../test.cmake)
