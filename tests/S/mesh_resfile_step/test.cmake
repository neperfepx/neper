# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# no results: the steps are set by foo.step* (0-2), and then used for bar (a
# single file) and baz (an expression)
set (pre_command cp -r n2-id1.sim test.sim)
set (post_command rm -r test.sim)
set (test_command -S test.sim -resnode "foo:file(foo),bar:file(bar),baz:2*foo")
set (ref_files "")
set (ref_dirs ref.sim/)
include(../../test.cmake)
