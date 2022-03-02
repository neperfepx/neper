# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.tess -datavercol "real:min(x*10,5)" -dataverrad 0.1 -showcell 0 -showver all -print test)
include(../../test.cmake)
