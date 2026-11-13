# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# merges a.sim and b.sim as written by FEPX 3: restart point restart/restart.rank<r>, force
# tables results/forces/<faset> and crystal type bct
set (post_command rm -r test.sim)
set (test_command -S a.sim,b.sim -o test.sim -resmesh vol)
set (ref_files "")
set (ref_dirs ref.sim/)
include(../../test.cmake)
