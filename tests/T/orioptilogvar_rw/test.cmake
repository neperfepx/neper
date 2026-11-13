# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# Uses the default algorithm (subplex), and rw is the orientation weight (1 by default),
# not the Laguerre weight.
set (test_command -T -n 2 -crysym cubic -ori "odf(input.odf)" -orisampling uniform -orioptilogvar iter,id,rw -orioptistop itermax=3 -o test)
include(../../test.cmake)
