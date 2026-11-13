# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n from_morpho -morpho "tesr:file(n10-id2.tesr)" -morphooptialgo "subplex(percellerror)" -morphooptistop itermax=300 -morphooptilogval iter,val,valmin -o test)
include(../../test.cmake)
