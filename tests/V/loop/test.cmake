# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.msh -loop F 1 0.1 1.2 -datanodecoo "file(xyz.stnode)" -datanodecoofact F -print testF -endloop)
include(../../test.cmake)
