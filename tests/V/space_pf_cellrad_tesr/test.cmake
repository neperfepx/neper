# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.tesr -space pf -datacellrad radeq*0.1 -imageformat asy -print test)
include(../../test.cmake)
