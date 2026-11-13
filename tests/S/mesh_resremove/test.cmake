# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# removes result odf, which has auxiliary (hidden) files
set (pre_command cp -r n2-id1.sim test.sim)
set (post_command rm -r test.sim)
set (test_command -S test.sim -resmesh !odf)
set (absent_files test.sim/results/mesh/odf)
set (ref_files "")
set (ref_dirs ref.sim/)
include(../../test.cmake)
