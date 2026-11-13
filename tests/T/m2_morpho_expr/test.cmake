# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 2::5 -morpho "voronoi::(id==1)?\"centroidal\":\"gg\"" -morphooptistop "default::(id==1)?\"val=1e-4||iter=10000\":\"itermax=10\"" -o test)
include(../../test.cmake)
