# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 100 -domain "sphere(2):translate(0.5,0.5,0.5):rotate(1,1,0,30)" -o test)
include(../../test.cmake)
