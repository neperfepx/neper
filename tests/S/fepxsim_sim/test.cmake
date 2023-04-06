# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -S 02_fcc -o test.sim)
set (post_command rm -r test.sim)
set (ref_dirs ref.sim/)
include(../../test.cmake)
