# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -M n5-id1.tess -interface cohesive -order 1 -format msh,inp,per -o test)
include(../../test.cmake)
