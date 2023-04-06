# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.tess -space ipf -pfmode density -pfgridsize 50 -datacellscale 0:10:20:30:40:50 -imageformat asy -print test)
include(../../test.cmake)
