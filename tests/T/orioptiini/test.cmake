# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# ori, theta and weight are read from files, with the ":" syntax of -morphooptiini (ref written by hand).
set (test_command -T -n 3 -crysym cubic -ori "odf(input.odf)" -orisampling uniform -orioptiini "ori:file(oris),theta:file(orit),weight:file(oriw)" -orioptilogvar iter,id,r1,rw,rt -orioptistop itermax=1 -o test)
include(../../test.cmake)
