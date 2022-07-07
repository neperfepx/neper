# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.msh -slicemesh "x=0.5" -dataelsetcol "ori:file(n2-id1.ori)" -print test)
include(../../test.cmake)
