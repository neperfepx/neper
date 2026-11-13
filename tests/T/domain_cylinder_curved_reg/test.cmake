# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 10 -domain "cylinder(1,1)" -reg 1 -id 1 -o test)
include(../../test.cmake)
