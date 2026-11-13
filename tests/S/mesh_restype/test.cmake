# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# foo: 6 values, not a symmetric tensor; bar: skew-symmetric tensor
set (pre_command cp -r n2-id1.sim test.sim)
set (post_command rm -r test.sim)
set (test_command -S test.sim -reselt "foo:file(foo,type=vector),foo2,foo6,bar:file(bar,type=skewtensor),bar12,bar21")
set (ref_files "")
set (ref_dirs ref.sim/)
include(../../test.cmake)
