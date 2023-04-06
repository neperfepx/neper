# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -V "pts1:file(pts1),pts2:file(pts2)" -showpts1 0 -showpts2 0 -imageformat pov -print test)
include(../../test.cmake)
