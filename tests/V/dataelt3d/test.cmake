# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.msh -dataelt3dcol id -lightambient 1 -lightsource none -imageformat pov -print test)
include(../../test.cmake)
