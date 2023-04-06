# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.msh -space pf -pfmode density -pfgridsize 50 -dataelsetweight id -imageformat asy -print test)
include(../../test.cmake)
