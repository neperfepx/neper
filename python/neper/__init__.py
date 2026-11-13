# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

"""Python interface to Neper (https://neper.info).

The Neper modules are run as functions, whose arguments are Neper's
command-line options::

    import neper

    neper.T(n=100, id=1, o="poly")              # neper -T -n 100 -id 1 -o poly
    neper.M("poly.tess", elttype="hex")         # neper -M poly.tess -elttype hex
    neper.V("poly.tess", datacellcol="id", print="img")

Each function runs the neper program and returns a :class:`Result`, which
provides the written files.  Errors raise :class:`NeperError`.

Neper's files are read into NumPy arrays by :func:`read` (or
:func:`read_tess`, :func:`read_tesr`, :func:`read_mesh`, :func:`read_sim` and
:func:`read_stat`), as in ``tess = neper.T(n=100).read("tess")``, and arrays
are passed to Neper as data files (see :func:`datafile`).
"""

from ._core import (
    NeperError,
    Result,
    T,
    M,
    S,
    V,
    data_dir,
    executable,
    get_defaults,
    module,
    run,
    set_defaults,
    version,
)
from ._ori import ori_angle, ori_convert
from ._io import (
    DataFile,
    EltBlock,
    Mesh,
    Odf,
    Sim,
    Tess,
    Tesr,
    datafile,
    read,
    read_mesh,
    read_odf,
    read_sim,
    read_stat,
    read_tesr,
    read_tess,
)

__version__ = "0.1.0"

__all__ = [
    "DataFile",
    "EltBlock",
    "Mesh",
    "Odf",
    "Sim",
    "Tess",
    "Tesr",
    "datafile",
    "read",
    "read_mesh",
    "read_odf",
    "read_sim",
    "read_stat",
    "read_tesr",
    "read_tess",
    "NeperError",
    "Result",
    "T",
    "M",
    "S",
    "V",
    "data_dir",
    "executable",
    "get_defaults",
    "module",
    "ori_angle",
    "ori_convert",
    "run",
    "set_defaults",
    "version",
]
