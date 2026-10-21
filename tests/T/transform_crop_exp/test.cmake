# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -loadtesr n2-id1.tesr -transform "crop(cube(0,5e-1,0,5E-1,0,1e+0)),autocrop" -format tesr -o test)
include(../../test.cmake)
