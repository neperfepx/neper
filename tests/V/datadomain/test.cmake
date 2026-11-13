# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.tess,n2-id1.msh -showdomain 1 -datadomainrad "default*2" -datadomaincol red -imageformat pov -print test)
include(../../test.cmake)
