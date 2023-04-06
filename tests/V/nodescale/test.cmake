# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.msh -showelt none -shownode all -datanodecol "real:file(n2-id1.stnode)" -datanodescale -1:1 -imageformat pov -print test)
include(../../test.cmake)
