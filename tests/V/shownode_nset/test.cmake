# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.msh -showelt none -shownode "nset==\"z1\"" -datanoderad 0.05 -imageformat pov -print test)
include(../../test.cmake)
