# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# merges ../merge/a.sim and ../merge/b.sim into test.sim (.sim extension
# added to -o)
set (post_command rm -r test.sim)
set (test_command -S ../merge/a.sim,../merge/b.sim -o test)
set (ref_files "")
set (ref_dirs ref.sim/)
include(../../test.cmake)
