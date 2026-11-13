# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# simulation with results at steps 0, 2 and 3 (step 1 not printed)
set (pre_command cp -r n2-id1.sim test.sim)
set (post_command rm -r test.sim)
set (test_command -S test.sim -resmesh stress_eq -reselset stress,ori,gos)
set (absent_files test.sim/results/mesh/stress_eq/stress_eq.step1
                  test.sim/results/elsets/stress/stress.step1
                  test.sim/results/elsets/ori/ori.step1
                  test.sim/results/elsets/gos/gos.step1)
set (ref_files "")
set (ref_dirs ref.sim/)
include(../../test.cmake)
