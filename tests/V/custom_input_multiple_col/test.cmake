# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -V "pts1:file(pts1),pts2:file(pts2)" -datapts1col id -datapts2col pink -imageformat pov -print test)
include(../../test.cmake)
