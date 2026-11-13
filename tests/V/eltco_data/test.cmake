# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.msh -showelt3d 0 -showeltco "x<0.5" -dataeltcocol real:z -dataeltcotrs 0.2 -dataeltcoedgecol blue -dataeltcoedgerad 0.005 -imageformat pov -print test)
include(../../test.cmake)
