# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 2 -crysym cubic -ori "odf(mesh=file(rod.msh),val=file(f),sigma=10)" -o test)
include(../../test.cmake)
