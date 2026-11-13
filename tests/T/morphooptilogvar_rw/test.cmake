# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# rw is the orientation weight, undefined here (-1), not the Laguerre weight.
set (test_command -T -n 2 -morpho gg -morphooptilogvar iter,id,rw -morphooptistop itermax=1 -o test)
include(../../test.cmake)
