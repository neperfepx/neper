# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 100 -id 1 -domain "cylinder(1,0.5)" -transform "translate(1,2,3),rmseed(id==1)" -o test)
include(../../test.cmake)
