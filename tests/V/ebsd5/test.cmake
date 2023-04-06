# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -V ebsd.tesr -datavoxcol ori -showvox all -imageformat pov -print test)
include(../../test.cmake)
