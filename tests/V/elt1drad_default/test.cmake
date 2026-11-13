# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.msh -showelt1d all -dataelt1drad "real:(domtype==1)?default*1.5:default" -imageformat pov -print test)
include(../../test.cmake)
