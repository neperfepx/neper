# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# merges a.sim (steps 0-1) and b.sim (steps 0-1, step 0 = step 1 of a.sim)
set (post_command rm -r test.sim)
set (test_command -S a.sim,b.sim -o test.sim)
set (ref_files "")
set (ref_dirs ref.sim/)
include(../../test.cmake)
