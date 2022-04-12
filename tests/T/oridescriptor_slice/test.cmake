# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 2 -oridescriptor quaternion -transform "slice(1,0,1,1)" -o test)
include(../../test.cmake)
