# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 2 -morpho gg -morphooptigrid "diameq:regular(-1,10,110),sphericity:regular(-0.1,1.1,120),1-sphericity:regular(-0.1,1.1,120)" -morphooptistop itermax=10 -o test)
include(../../test.cmake)
