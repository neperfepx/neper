# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -V "pts1:file(pts1),pts2:file(pts2)" -datapts1rad 0.1*id -datapts2rad 0.2*id -imageformat pov -print test)
include(../../test.cmake)
