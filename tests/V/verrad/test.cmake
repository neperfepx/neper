# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.tess -showver all -dataverrad 0.05 -datavercol id -dataedgerad 0.02 -dataedgecol id -datapolycol id -imageformat pov -print test)
include(../../test.cmake)
