# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (pre_command cp -r n2-id1.sim test.sim)
set (post_command rm -r test.sim)
set (test_command -S test.sim -rescell x)
set (ref_dirs ref.sim/)
include(../../test.cmake)
