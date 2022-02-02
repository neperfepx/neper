# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.tess -showpoly "z<0.5" -showface "z>0.9" -showedge "(y>0.9)&&(x<0.1)" -showver "((x>0.9)&&(y>0.9))&&(z>0.9)" -print test)
include(../../test.cmake)
