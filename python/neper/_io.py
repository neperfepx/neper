# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

"""Reading Neper's files into NumPy arrays, and writing data files.

Identifiers follow Neper's convention: entities are numbered from 1, and
row ``i`` of an array describes entity ``i + 1``, while entity references
(e.g. the vertices of an edge) are identifiers, i.e. start from 1.
"""

import atexit
import os
import re
import shutil
import tempfile
from pathlib import Path

__all__ = [
    "Tess",
    "Tesr",
    "Mesh",
    "EltBlock",
    "Odf",
    "Sim",
    "read",
    "read_tess",
    "read_tesr",
    "read_mesh",
    "read_odf",
    "read_sim",
    "read_stat",
    "DataFile",
    "datafile",
]


def _np():
    try:
        import numpy
    except ImportError as e:  # pragma: no cover
        raise ImportError("reading Neper's files requires NumPy (pip install numpy)") from e
    return numpy


# Number of parameters of the orientation descriptors
_ORI_SIZE = {
    "euler-bunge": 3,
    "euler-kocks": 3,
    "euler-roe": 3,
    "rodrigues": 3,
    "rotmat": 9,
    "axis-angle": 4,
    "angle": 1,
    "quaternion": 4,
}

_ORI_SHORT = {"e": "euler-bunge", "ek": "euler-kocks", "er": "euler-roe", "R": "rodrigues",
              "rtheta": "axis-angle", "q": "quaternion", "m": "miller"}


def _orides(des, fix=None):
    """Expand an orientation descriptor as Neper does (e.g. ``"e"`` gives
    ``"euler-bunge:passive"``).  *fix* fixes the convention of former files,
    as Neper does: ``"all"`` swaps the convention (tess files of version <=
    3.4), and ``"explicit"`` swaps it only if it is explicit (tesr files of
    version <= 2.1)."""
    parts = des.split(":")
    name = _ORI_SHORT.get(parts[0], parts[0])
    conv = parts[1] if len(parts) > 1 else "passive"
    if fix == "all" or (fix == "explicit" and len(parts) > 1):
        conv = {"active": "passive", "passive": "active"}.get(conv, conv)
    return f"{name}:{conv}"


def _version(string):
    try:
        return tuple(int(x) for x in string.split(".")[:2])
    except ValueError:
        return (0, 0)


class _Tokens:
    """Whitespace-separated tokens of a text file."""

    def __init__(self, text, path):
        self.toks = text.split()
        self.pos = 0
        self.path = path

    def error(self, message):
        return ValueError(f"{self.path}: {message} (token {self.pos + 1})")

    def peek(self):
        return self.toks[self.pos] if self.pos < len(self.toks) else None

    def next(self):
        if self.pos >= len(self.toks):
            raise self.error("unexpected end of file")
        self.pos += 1
        return self.toks[self.pos - 1]

    def expect(self, tok):
        got = self.next()
        if got != tok:
            raise self.error(f"`{tok}' expected, `{got}' found")

    def int(self):
        tok = self.next()
        try:
            return int(tok)
        except ValueError:
            raise self.error(f"integer expected, `{tok}' found") from None

    def take(self, n):
        if self.pos + n > len(self.toks):
            raise self.error("unexpected end of file")
        self.pos += n
        return self.toks[self.pos - n:self.pos]

    def ints(self, n):
        return _np().array(self.take(n), dtype=int)

    def floats(self, n):
        return _np().array(self.take(n), dtype=float)

    def body(self):
        """Tokens until the next section (token starting with *)."""
        beg = self.pos
        while self.pos < len(self.toks) and not self.toks[self.pos].startswith("*"):
            self.pos += 1
        return self.toks[beg:self.pos]

    def subsection(self):
        """Name of the next subsection (*name), or None."""
        tok = self.peek()
        if tok is not None and tok.startswith("*") and not tok.startswith("**"):
            return self.next()
        return None


class _Data:
    """Base class of the file contents: attributes, listed by repr."""

    def __repr__(self):
        np = _np()
        lines = [f"<neper.{type(self).__name__} {self.path!s}>" if getattr(self, "path", None)
                 else f"<neper.{type(self).__name__}>"]
        for key, val in self.__dict__.items():
            if key.startswith("_") or key == "path" or val is None:
                continue
            if isinstance(val, np.ndarray):
                desc = f"array{val.shape} {val.dtype}"
            elif isinstance(val, list):
                desc = f"list[{len(val)}]"
            elif isinstance(val, dict):
                desc = f"dict[{len(val)}]"
            else:
                desc = repr(val)
            lines.append(f"  {key}: {desc}")
        return "\n".join(lines)


def _cell_section(t, obj, n, version, kind):
    """**cell subsections common to tess and tesr files."""
    np = _np()
    while True:
        sub = t.subsection()
        if sub is None:
            break
        if sub == "*id":
            obj.cell_id = t.ints(n)
        elif sub in ("*lam", "*lamid"):
            obj.cell_lam = t.ints(n)
        elif sub in ("*mode", "*modeid"):
            obj.cell_mode = t.ints(n)
        elif sub == "*group":
            obj.cell_group = t.ints(n)
        elif sub == "*crysym":
            obj.cell_crysym = t.next()
        elif sub == "*seed":
            vals = t.body()
            dim = len(vals) // n - 2 if n else 3
            rows = np.array(vals, dtype=float).reshape(n, dim + 2)
            obj.seed_coo = np.zeros((n, 3))
            obj.seed_coo[:, :dim] = rows[:, 1:dim + 1]
            obj.seed_weight = rows[:, -1]
            # versions <= 3.5 (tess) and <= 2.2 (tesr) record sqrt (w)
            old = version <= (3, 5) if kind == "tess" else version <= (2, 2)
            if old:
                obj.seed_weight = obj.seed_weight ** 2
        elif sub == "*ori":
            des = t.next()
            if kind == "tess":
                fix = "all" if version <= (3, 4) else None
            else:
                fix = "explicit" if version <= (2, 1) else None
            obj.cell_orides = _orides(des, fix)
            vals = t.body()
            obj.cell_ori = _ori_array(vals, n, obj.cell_orides, t)
        elif sub == "*orispread":
            obj.cell_orispread = t.take(n)
        elif sub == "*weight":
            obj.cell_weight = t.floats(n)
        else:  # e.g. *coo, *vol, *convexity of former tesr files
            t.body()


def _ori_array(vals, n, des, t):
    np = _np()
    if n == 0:
        return np.zeros((0, _ORI_SIZE.get(des.split(":")[0], 3)))
    if len(vals) % n:
        raise t.error("unexpected number of orientation values")
    try:
        return np.array(vals, dtype=float).reshape(n, -1)
    except ValueError:
        raise t.error("orientation labels are not supported") from None


class Tess(_Data):
    """Tessellation (.tess file), read by :func:`read_tess`.

    Main attributes (row ``i`` describes entity ``i + 1``):

    - ``dim``, ``type`` (``"standard"`` or ``"periodic"``), ``format``;
    - cells: ``cell_qty``, ``cell_id``, ``seed_coo`` (``(n, 3)``),
      ``seed_weight``, ``cell_ori`` (``(n, k)``), ``cell_orides``
      (orientation descriptor, e.g. ``"rodrigues:passive"``),
      ``cell_crysym``, ``cell_lam``, ``cell_mode``, ``cell_group``,
      ``cell_orispread``, ``cell_weight``;
    - vertices: ``ver_coo`` (``(n, 3)``), ``ver_state``;
    - edges: ``edge_vers`` (``(n, 2)``), ``edge_state``;
    - faces: ``face_vers`` and ``face_edges`` (lists of arrays; edges are
      signed by their orientations in the faces), ``face_eq`` (``(n, 4)``:
      ``d, a, b, c`` with ``a x + b y + c z = d``), ``face_state``,
      ``face_pt``, ``face_ptcoo``;
    - polyhedra: ``poly_faces`` (list of arrays, signed: positive if the
      face normal points outwards);
    - domain: ``dom_type``, ``dom_ver_*``, ``dom_edge_*``, ``dom_face_*``;
    - periodicity (if any): ``periodic``, ``periodic_dist``, ``per_seed``,
      ``per_ver``, ``per_edge``, ``per_face`` (rows: secondary id, primary
      id, shift along x, y and z[, orientation]);
    - scales (multiscale tessellation): ``scale_qty``, ``cell_scaleid``
      (``(n, scale_qty)``).
    """

    def __init__(self, path=None):
        self.path = Path(path) if path is not None else None
        self.format = self.dim = self.type = None
        self.cell_qty = 0
        self.cell_id = self.cell_lam = self.cell_mode = self.cell_group = None
        self.cell_crysym = None
        self.seed_coo = self.seed_weight = None
        self.cell_ori = self.cell_orides = None
        self.cell_orispread = self.cell_weight = None
        self.ver_coo = self.ver_state = None
        self.edge_vers = self.edge_state = None
        self.face_vers = self.face_edges = None
        self.face_eq = self.face_state = self.face_pt = self.face_ptcoo = None
        self.poly_faces = None
        self.dom_type = None
        self.dom_ver_coo = self.dom_ver_label = self.dom_ver_tessver = None
        self.dom_edge_vers = self.dom_edge_type = self.dom_edge_parms = None
        self.dom_edge_label = self.dom_edge_tessedges = None
        self.dom_face_vers = self.dom_face_edges = self.dom_face_type = None
        self.dom_face_parms = self.dom_face_label = self.dom_face_tessfaces = None
        self.periodic = self.periodic_dist = None
        self.per_seed = self.per_ver = self.per_edge = self.per_face = None
        self.scale_qty = None
        self.cell_scaleid = None

    @property
    def ver_qty(self):
        return 0 if self.ver_coo is None else len(self.ver_coo)

    @property
    def edge_qty(self):
        return 0 if self.edge_vers is None else len(self.edge_vers)

    @property
    def face_qty(self):
        return 0 if self.face_vers is None else len(self.face_vers)

    @property
    def poly_qty(self):
        return 0 if self.poly_faces is None else len(self.poly_faces)


def read_tess(path):
    """Read a tessellation file (.tess) and return a :class:`Tess`."""
    np = _np()
    path = Path(path)
    t = _Tokens(path.read_text(), path)
    tess = Tess(path)

    t.expect("***tess")
    t.expect("**format")
    tess.format = t.next()
    version = _version(tess.format)
    if version < (3, 3):
        raise ValueError(f"{path}: tess file format {tess.format} is not supported "
                         "(convert it with neper -T -loadtess)")
    t.expect("**general")
    tess.dim = t.int()
    tess.type = t.next()

    while True:
        sec = t.next()
        if sec == "***end":
            break
        if sec == "**cell":
            n = tess.cell_qty = t.int()
            _cell_section(t, tess, n, version, "tess")
        elif sec == "**vertex":
            n = t.int()
            rows = np.array(t.take(5 * n), dtype=float).reshape(n, 5)
            tess.ver_coo = rows[:, 1:4]
            tess.ver_state = rows[:, 4].astype(int)
        elif sec == "**edge":
            n = t.int()
            rows = t.ints(4 * n).reshape(n, 4)
            tess.edge_vers = rows[:, 1:3]
            tess.edge_state = rows[:, 3]
        elif sec == "**face":
            n = t.int()
            tess.face_vers, tess.face_edges = [], []
            tess.face_eq = np.zeros((n, 4))
            tess.face_state = np.zeros(n, dtype=int)
            tess.face_pt = np.zeros(n, dtype=int)
            tess.face_ptcoo = np.zeros((n, 3))
            for i in range(n):
                t.int()
                tess.face_vers.append(t.ints(t.int()))
                tess.face_edges.append(t.ints(t.int()))
                tess.face_eq[i] = t.floats(4)
                tess.face_state[i] = t.int()
                tess.face_pt[i] = t.int()
                tess.face_ptcoo[i] = t.floats(3)
        elif sec == "**polyhedron":
            n = t.int()
            tess.poly_faces = []
            for i in range(n):
                t.int()
                tess.poly_faces.append(t.ints(t.int()))
        elif sec == "**domain":
            _tess_domain(t, tess, version)
        elif sec == "**periodicity":
            _tess_periodicity(t, tess)
        elif sec == "**scale":
            t.expect("*general")
            tess.scale_qty = t.int()
            t.expect("*cellid")
            rows = t.ints((tess.scale_qty + 1) * tess.cell_qty)
            tess.cell_scaleid = rows.reshape(tess.cell_qty, -1)[:, 1:]
        else:
            raise t.error(f"unknown section `{sec}'")

    return tess


def _tess_domain(t, tess, version):
    np = _np()
    while True:
        sub = t.subsection()
        if sub is None:
            break
        if sub == "*general":
            tess.dom_type = t.next()
        elif sub == "*vertex":
            n = t.int()
            tess.dom_ver_coo = np.zeros((n, 3))
            tess.dom_ver_label, tess.dom_ver_tessver = [], np.zeros(n, dtype=int)
            for i in range(n):
                t.int()
                tess.dom_ver_coo[i] = t.floats(3)
                tess.dom_ver_label.append(t.next())
                t.int()  # always 1
                tess.dom_ver_tessver[i] = t.int()
        elif sub == "*edge":
            n = t.int()
            tess.dom_edge_vers, tess.dom_edge_type, tess.dom_edge_parms = [], [], []
            tess.dom_edge_label, tess.dom_edge_tessedges = [], []
            for i in range(n):
                t.int()
                tess.dom_edge_vers.append(t.ints(t.int()))
                if version >= (3, 5):
                    tess.dom_edge_type.append(t.next())
                    tess.dom_edge_parms.append(t.floats(t.int()))
                else:
                    tess.dom_edge_type.append("line")
                    tess.dom_edge_parms.append(np.zeros(0))
                tess.dom_edge_label.append(t.next())
                tess.dom_edge_tessedges.append(t.ints(t.int()))
        elif sub == "*face":
            n = t.int()
            tess.dom_face_vers, tess.dom_face_edges = [], []
            tess.dom_face_type, tess.dom_face_parms = [], []
            tess.dom_face_label, tess.dom_face_tessfaces = [], []
            for i in range(n):
                t.int()
                tess.dom_face_vers.append(t.ints(t.int()))
                tess.dom_face_edges.append(t.ints(t.int()))
                tess.dom_face_type.append(t.next())
                tess.dom_face_parms.append(t.floats(t.int()))
                tess.dom_face_label.append(t.next())
                tess.dom_face_tessfaces.append(t.ints(t.int()))
        else:
            raise t.error(f"unknown domain subsection `{sub}'")


def _tess_periodicity(t, tess):
    while True:
        sub = t.subsection()
        if sub is None:
            break
        if sub == "*general":
            tess.periodic = t.ints(3).astype(bool)
            tess.periodic_dist = t.floats(3)
        else:
            ncol = 5 if sub in ("*seed", "*vertex") else 6
            n = t.int()
            rows = t.ints(ncol * n).reshape(n, ncol)
            key = {"*seed": "per_seed", "*vertex": "per_ver", "*edge": "per_edge",
                   "*face": "per_face"}.get(sub)
            if key is None:
                raise t.error(f"unknown periodicity subsection `{sub}'")
            setattr(tess, key, rows)


class Tesr(_Data):
    """Raster tessellation (.tesr file), read by :func:`read_tesr`.

    Main attributes:

    - ``dim``, ``format``, ``size`` (numbers of voxels), ``vsize`` (voxel
      sizes), ``origin``;
    - ``vox_cell``: the cells of the voxels (0 for void), an integer array
      indexed as ``[x, y, z]`` (``[x, y]`` in 2D), from 0;
    - ``vox_ori`` (``[x, y, z, k]``), ``vox_orides``, ``vox_oridef``
      (boolean): voxel orientations, if any;
    - cells (if any): ``cell_qty``, ``cell_id``, ``seed_coo``,
      ``seed_weight``, ``cell_ori``, ``cell_orides``, ``cell_crysym``,
      ``cell_group``, ``cell_orispread``.
    """

    def __init__(self, path=None):
        self.path = Path(path) if path is not None else None
        self.format = self.dim = None
        self.size = self.vsize = self.origin = None
        self.cell_qty = None
        self.cell_id = self.cell_group = self.cell_crysym = None
        self.seed_coo = self.seed_weight = None
        self.cell_ori = self.cell_orides = self.cell_orispread = None
        self.vox_cell = None
        self.vox_ori = self.vox_orides = self.vox_oridef = None


class _ByteTokens(_Tokens):
    """Tokens of a file that can contain binary data (tesr file)."""

    _TOKEN = re.compile(rb"\S+")

    def __init__(self, data, path):
        self.data = data
        self.offset = 0
        self.path = path
        self.pos = 0

    def peek(self):
        m = self._TOKEN.search(self.data, self.offset)
        return m.group().decode() if m else None

    def next(self):
        m = self._TOKEN.search(self.data, self.offset)
        if not m:
            raise self.error("unexpected end of file")
        self.offset = m.end()
        self.pos += 1
        return m.group().decode()

    def take(self, n):
        return [self.next() for _ in range(n)]

    def body(self):
        vals = []
        while True:
            tok = self.peek()
            if tok is None or tok.startswith("*"):
                return vals
            vals.append(self.next())

    def nextline_isfile(self):
        """Whether the next line is a *file line (the data that follow the
        current line can be binary, so they are not decoded)."""
        nl = self.data.find(b"\n", self.offset)
        return nl != -1 and self.data[nl + 1:nl + 200].lstrip(b" \t").startswith(b"*file ")

    def binary(self, nbytes):
        # the data start after the end of the current line
        nl = self.data.index(b"\n", self.offset)
        beg = nl + 1
        if beg + nbytes > len(self.data):
            raise self.error("unexpected end of file")
        self.offset = beg + nbytes
        return self.data[beg:self.offset]


_TESR_DTYPES = {"binary8": "u1", "binary16": "<i2", "binary16_big": ">i2",
                "binary32": "<i4", "binary32_big": ">i4"}


def read_tesr(path):
    """Read a raster tessellation file (.tesr) and return a :class:`Tesr`."""
    np = _np()
    path = Path(path)
    t = _ByteTokens(path.read_bytes(), path)
    tesr = Tesr(path)

    t.expect("***tesr")
    t.expect("**format")
    tesr.format = t.next()
    version = _version(tesr.format)
    dataformat = t.next() if version <= (2, 0) else None
    t.expect("**general")
    tesr.dim = t.int()
    tesr.size = t.ints(tesr.dim)
    tesr.vsize = t.floats(tesr.dim)
    tesr.origin = np.zeros(tesr.dim)
    while True:
        sub = t.subsection()
        if sub is None:
            break
        if sub == "*origin":
            tesr.origin = t.floats(tesr.dim)
        elif sub == "*hasvoid":
            t.take(1)
        else:
            raise t.error(f"unknown subsection `{sub}'")

    nvox = int(np.prod(tesr.size))
    shape = tuple(int(s) for s in tesr.size)

    while True:
        sec = t.next()
        if sec == "***end":
            break
        if sec == "**cell":
            n = tesr.cell_qty = t.int()
            _cell_section(t, tesr, n, version, "tesr")
        elif sec == "**data":
            fmt = dataformat if version <= (2, 0) else t.next()
            vals = _tesr_values(t, path, fmt, nvox, "int")
            tesr.vox_cell = vals.reshape(shape, order="F")
        elif sec == "**oridata":
            tesr.vox_orides = _orides(t.next())
            fmt = dataformat if version <= (2, 0) else None
            if fmt is None and not t.nextline_isfile():
                fmt = t.next()
            if fmt is None:  # written as *file only: binary
                fmt = "binary"
            k = _ORI_SIZE.get(tesr.vox_orides.split(":")[0])
            vals = _tesr_values(t, path, fmt, nvox * k if k else None, "ori")
            # one row per voxel, x varying first
            vals = vals.reshape(nvox, -1).reshape(shape[::-1] + (-1,))
            tesr.vox_ori = vals.transpose(tuple(range(tesr.dim))[::-1] + (tesr.dim,))
        elif sec == "**oridef":
            fmt = t.next()
            fmt = "binary8" if fmt == "binary" else fmt
            vals = _tesr_values(t, path, fmt, nvox, "int")
            tesr.vox_oridef = vals.reshape(shape, order="F").astype(bool)
        else:
            raise t.error(f"unknown section `{sec}'")

    if tesr.cell_qty is None and tesr.vox_cell is not None:
        tesr.cell_qty = int(tesr.vox_cell.max(initial=0))
    return tesr


def _tesr_values(t, path, fmt, qty, kind):
    """Voxel values, in-file (ascii or binary) or in a *file."""
    np = _np()
    if t.nextline_isfile():
        t.expect("*file")
        name = t.next()
        fpath = path.parent / name
        if fmt == "ascii":
            vals = fpath.read_text().split()
        else:
            raw = fpath.read_bytes()
    else:
        fpath = None
        if fmt == "ascii":
            vals = t.take(qty) if qty is not None else t.body()
        else:
            size = 8 if kind == "ori" else np.dtype(_TESR_DTYPES[fmt]).itemsize
            raw = t.binary(qty * size)

    if fmt == "ascii":
        return np.array(vals, dtype=float if kind == "ori" else int)
    if kind == "ori":
        return np.frombuffer(raw, dtype=np.float64).copy()
    if fmt not in _TESR_DTYPES:
        raise t.error(f"unknown data format `{fmt}'")
    return np.frombuffer(raw, dtype=_TESR_DTYPES[fmt]).astype(int)


# Gmsh element types: name, dimension, order, number of nodes
_GMSH_ELTS = {
    15: ("point", 0, 1, 1),
    1: ("line", 1, 1, 2), 8: ("line", 1, 2, 3),
    2: ("tri", 2, 1, 3), 9: ("tri", 2, 2, 6),
    3: ("quad", 2, 1, 4), 16: ("quad", 2, 2, 8), 10: ("quad", 2, 2, 9),
    4: ("tet", 3, 1, 4), 11: ("tet", 3, 2, 10),
    5: ("hex", 3, 1, 8), 17: ("hex", 3, 2, 20), 12: ("hex", 3, 2, 27),
    6: ("prism", 3, 1, 6), 18: ("prism", 3, 2, 15), 13: ("prism", 3, 2, 18),
    7: ("pyramid", 3, 1, 5),
    100: ("quad6", 2, 1, 6),  # Neper's 2D cohesive element
}


class EltBlock(_Data):
    """Elements of a same type, in a :class:`Mesh`.

    Attributes: ``type`` (``"tet"``, ``"hex"``, ``"tri"``, ``"quad"``,
    ``"prism"``, ``"line"`` or ``"point"``), ``dim``, ``order``,
    ``gmsh_type``, ``elt_id`` (``(n,)``), ``elt_nodes`` (``(n, k)``, node
    identifiers), ``elt_elset`` (``(n,)``, elset identifiers) and
    ``elt_part`` (``(n,)``, partitions, 0 if none)."""

    def __init__(self, gmsh_type, ids, nodes, elsets, parts):
        self.type, self.dim, self.order, _ = _GMSH_ELTS[gmsh_type]
        self.gmsh_type = gmsh_type
        self.elt_id = ids
        self.elt_nodes = nodes
        self.elt_elset = elsets
        self.elt_part = parts

    @property
    def elt_qty(self):
        return len(self.elt_id)

    def __repr__(self):
        return (f"<neper.EltBlock {self.elt_qty} {self.type} (dim {self.dim}, "
                f"order {self.order})>")


class Mesh(_Data):
    """Mesh (.msh file), read by :func:`read_mesh`.

    Main attributes:

    - ``node_coo`` (``(n, 3)``; node ``i + 1`` is row ``i``);
    - ``blocks``: the elements, as a list of :class:`EltBlock`, one per
      element type; method :meth:`elts` returns the elements of a dimension;
    - ``nsets`` (dictionary: label → node identifiers), ``fasets``
      (dictionary: label → (element identifiers, element nodes));
    - ``node_part`` (node partitions), ``periodicity`` (rows: secondary
      node, primary node, shift along x, y and z);
    - ``elset_ori`` (``(n, k)``), ``elset_orides``, ``elset_crysym``,
      ``elt_ori``, ``elt_orides``, ``elset_group``;
    - ``physical_names`` (dictionary: (dimension, identifier) → name),
      ``domain``, ``format``, ``mesh_version``.
    """

    def __init__(self, path=None):
        self.path = Path(path) if path is not None else None
        self.format = self.mesh_version = self.binary = None
        self.domain = self.topology = None
        self.node_coo = None
        self.blocks = []
        self.periodicity = None
        self.nsets = self.fasets = None
        self.node_part = None
        self.physical_names = None
        self.elset_ori = self.elset_orides = self.elset_crysym = None
        self.elt_ori = self.elt_orides = None
        self.elset_group = None

    @property
    def dim(self):
        """Dimension of the mesh (highest element dimension)."""
        return max((b.dim for b in self.blocks), default=0)

    @property
    def node_qty(self):
        return 0 if self.node_coo is None else len(self.node_coo)

    def elts(self, dim=None):
        """Return the elements of dimension *dim* (default: the mesh
        dimension) as an :class:`EltBlock` (the first one if there are
        several, e.g. with cohesive elements)."""
        dim = self.dim if dim is None else dim
        for b in self.blocks:
            if b.dim == dim:
                return b
        raise LookupError(f"no {dim}D elements")


def read_mesh(path):
    """Read a mesh file (.msh, Gmsh format version 2.2, ASCII or binary, as
    written by Neper) and return a :class:`Mesh`."""
    np = _np()
    path = Path(path)
    data = path.read_bytes()
    mesh = Mesh(path)
    pos = 0

    def line():
        nonlocal pos
        end = data.find(b"\n", pos)
        if end == -1:
            end = len(data)
        text = data[pos:end].decode().strip()
        pos = end + 1
        return text

    def lines_until(tag):
        nonlocal pos
        end = data.find(tag.encode(), pos)
        if end == -1:
            raise ValueError(f"{path}: `{tag}' not found")
        text = data[pos:end].decode()
        pos = data.find(b"\n", end) + 1 if data.find(b"\n", end) != -1 else len(data)
        return text

    while pos < len(data):
        tag = line()
        if not tag:
            continue
        if not tag.startswith("$"):
            raise ValueError(f"{path}: section expected, `{tag[:40]}' found")
        name = tag[1:]
        end = f"$End{name}"

        if name == "MeshFormat":
            fmt = line().split()
            mesh.format, mesh.binary = fmt[0], fmt[1] == "1"
            if not mesh.format.startswith("2."):
                raise ValueError(f"{path}: msh format {mesh.format} is not supported")
            if mesh.binary:
                one = np.frombuffer(data[pos:pos + 4], dtype=np.int32)[0]
                if one != 1:
                    raise ValueError(f"{path}: binary file of other endianness")
                pos += 5  # int + newline
            lines_until(end)
        elif name == "Nodes":
            n = int(line())
            if mesh.binary:
                rec = np.dtype([("id", "<i4"), ("coo", "<f8", 3)])
                arr = np.frombuffer(data, dtype=rec, count=n, offset=pos)
                pos += n * rec.itemsize
                lines_until(end)
                mesh.node_coo = _by_id(arr["id"], arr["coo"], n, path, "node")
            else:
                vals = np.array(lines_until(end).split(), dtype=float).reshape(n, 4)
                mesh.node_coo = _by_id(vals[:, 0].astype(int), vals[:, 1:], n, path, "node")
        elif name == "Elements":
            n = int(line())
            if mesh.binary:
                pos = _msh_elts_binary(data, pos, n, mesh)
                lines_until(end)
            else:
                _msh_elts_ascii(lines_until(end), n, mesh)
        elif name == "Periodicity":
            vals = lines_until(end).split()
            n = int(vals[0])
            mesh.periodicity = np.array(vals[1:], dtype=int).reshape(n, 5)
        elif name == "NSets":
            toks = lines_until(end).split()
            mesh.nsets = {}
            i, n = 1, int(toks[0])
            for _ in range(n):
                label, qty = toks[i], int(toks[i + 1])
                mesh.nsets[label] = np.array(toks[i + 2:i + 2 + qty], dtype=int)
                i += 2 + qty
        elif name == "Fasets":
            mesh.fasets = _msh_fasets(lines_until(end), mesh)
        elif name == "NodePartitions":
            vals = lines_until(end).split()
            n = int(vals[0])
            rows = np.array(vals[1:], dtype=int).reshape(n, 2)
            mesh.node_part = _by_id(rows[:, 0], rows[:, 1], n, path, "node")
        elif name == "PhysicalNames":
            text = lines_until(end).splitlines()
            mesh.physical_names = {}
            for row in text[1:]:
                parts = row.split(None, 2)
                if len(parts) == 3:
                    mesh.physical_names[(int(parts[0]), int(parts[1]))] = parts[2].strip('"')
        elif name in ("ElsetOrientations", "ElementOrientations"):
            text = lines_until(end).split()
            n, des = int(text[0]), _orides(text[1])
            rows = np.array(text[2:], dtype=float).reshape(n, -1)
            ori = _by_id(rows[:, 0].astype(int), rows[:, 1:], n, path, "entity")
            if name == "ElsetOrientations":
                mesh.elset_ori, mesh.elset_orides = ori, des
            else:
                mesh.elt_ori, mesh.elt_orides = ori, des
        elif name == "ElsetCrySym":
            mesh.elset_crysym = lines_until(end).split()[0]
        elif name == "Groups":
            toks = lines_until(end).split()
            n = int(toks[1])
            rows = np.array(toks[2:2 + 2 * n], dtype=int).reshape(n, 2)
            mesh.elset_group = _by_id(rows[:, 0], rows[:, 1], n, path, "elset")
        elif name == "MeshVersion":
            mesh.mesh_version = lines_until(end).strip()
        elif name == "Domain":
            mesh.domain = lines_until(end).strip()
        elif name == "Topology":
            mesh.topology = int(lines_until(end).split()[0])
        else:  # unknown section: skipped
            lines_until(end)

    # former mesh versions, converted as Neper does: opposite orientation
    # convention (<= 2.2) and faset element nodes in reverse order (<= 2.3)
    version = _version(mesh.mesh_version) if mesh.mesh_version else (0, 0)
    if mesh.elset_orides and version <= (2, 2):
        mesh.elset_orides = _orides(mesh.elset_orides, "all")
    if mesh.fasets and version <= (2, 3):
        mesh.fasets = {k: (ids, nodes[:, ::-1]) for k, (ids, nodes) in mesh.fasets.items()}

    return mesh


def _by_id(ids, vals, n, path, what):
    """Values ordered by identifier: row i is identifier i + 1.  If the
    identifiers are not 1 to n, the missing rows are NaN (or 0)."""
    np = _np()
    ids = np.asarray(ids)
    vals = np.asarray(vals)
    if np.array_equal(ids, np.arange(1, n + 1)):
        return vals.copy()
    if ids.min(initial=1) < 1 or len(np.unique(ids)) != len(ids):
        raise ValueError(f"{path}: invalid {what} identifiers")
    shape = (int(ids.max(initial=0)),) + vals.shape[1:]
    out = np.full(shape, np.nan) if vals.dtype.kind == "f" else np.zeros(shape, vals.dtype)
    out[ids - 1] = vals
    return out


def _msh_add(mesh, gmsh_type, rows):
    """Add elements (rows: id, elset, elset, part, nodes...) to the blocks."""
    np = _np()
    if gmsh_type not in _GMSH_ELTS:
        raise ValueError(f"{mesh.path}: unknown element type {gmsh_type}")
    rows = np.asarray(rows, dtype=int)
    mesh.blocks.append(EltBlock(gmsh_type, rows[:, 0], rows[:, 4:], rows[:, 1], rows[:, 3]))


def _msh_elts_ascii(text, n, mesh):
    np = _np()
    lines = text.split("\n")
    # elements are written by type: group consecutive lines of a same type
    cur, rows = None, []
    count = 0
    for row in lines:
        vals = row.split()
        if not vals:
            continue
        count += 1
        vals = [int(v) for v in vals]
        ntags = vals[2]
        if ntags != 3:  # general case: pad / truncate to 3 tags
            tags = (vals[3:3 + ntags] + [0, 0, 0])[:3]
            vals = vals[:3] + tags + vals[3 + ntags:]
        rec = [vals[0]] + vals[3:]
        if vals[1] != cur:
            if rows:
                _msh_add(mesh, cur, rows)
            cur, rows = vals[1], []
        rows.append(rec)
    if rows:
        _msh_add(mesh, cur, rows)
    if count != n:
        raise ValueError(f"{mesh.path}: {count} elements read, {n} expected")


def _msh_elts_binary(data, pos, n, mesh):
    np = _np()
    count = 0
    while count < n:
        gmsh_type, qty, ntags = np.frombuffer(data, dtype="<i4", count=3, offset=pos)
        pos += 12
        nnodes = _GMSH_ELTS[int(gmsh_type)][3]
        width = 1 + ntags + nnodes
        rows = np.frombuffer(data, dtype="<i4", count=qty * width, offset=pos)
        rows = rows.reshape(qty, width).astype(int)
        pos += 4 * qty * width
        tags = np.zeros((qty, 3), dtype=int)
        tags[:, :min(ntags, 3)] = rows[:, 1:1 + min(ntags, 3)]
        full = np.hstack([rows[:, :1], tags, rows[:, 1 + ntags:]])
        _msh_add(mesh, int(gmsh_type), full)
        count += qty
    return pos


def _msh_fasets(text, mesh):
    np = _np()
    lines = [l for l in text.split("\n") if l.strip()]
    fasets = {}
    n, i = int(lines[0]), 1
    for _ in range(n):
        label, qty = lines[i].strip(), int(lines[i + 1])
        rows = [[int(v) for v in l.split()] for l in lines[i + 2:i + 2 + qty]]
        if rows:
            arr = np.array(rows, dtype=int)
            fasets[label] = (arr[:, 0], arr[:, 1:])
        else:
            fasets[label] = (np.zeros(0, dtype=int), np.zeros((0, 0), dtype=int))
        i += 2 + qty
    return fasets


class Odf(_Data):
    """Orientation distribution function (.odf file), read by
    :func:`read_odf`.

    Attributes: ``format``, ``space`` (e.g. ``"rodrigues"``), ``crysym``,
    ``fr`` (fundamental region, e.g. ``"fr20"``), ``theta`` (kernel width,
    in degrees), ``elt_data`` (one value per element of the orientation-space
    mesh) and ``node_data`` (one value per node, if any).  Method
    :meth:`mesh` reads the orientation-space mesh."""

    def __init__(self, path=None):
        self.path = Path(path) if path is not None else None
        self.format = self.space = self.crysym = self.fr = self.theta = None
        self.elt_data = self.node_data = None

    def mesh(self):
        """Read the orientation-space mesh (:class:`Mesh`), from Neper's data
        directory (see :func:`neper.data_dir`)."""
        from ._core import data_dir

        return read_mesh(Path(data_dir()) / "orispace" / self.crysym / f"{self.fr}.msh")


def read_odf(path):
    """Read an orientation distribution function file (.odf) and return an
    :class:`Odf`."""
    np = _np()
    path = Path(path)
    t = _Tokens(path.read_text(), path)
    odf = Odf(path)
    t.expect("***odf")
    t.expect("**format")
    odf.format = t.next()
    while True:
        sec = t.next()
        if sec == "***end":
            break
        if sec == "**space":
            odf.space = t.next()
            while True:
                sub = t.subsection()
                if sub is None:
                    break
                val = t.next()
                if sub == "*crysym":
                    odf.crysym = val
                elif sub == "*fr":
                    odf.fr = val
                elif sub == "*theta":
                    odf.theta = float(val)
        elif sec in ("**eltdata", "**nodedata"):
            fmt = t.next()
            if fmt != "ascii":
                raise t.error(f"unknown data format `{fmt}'")
            if t.peek() == "*file":
                t.next()
                vals = (path.parent / t.next()).read_text().split()
            else:
                vals = t.body()
            setattr(odf, "elt_data" if sec == "**eltdata" else "node_data",
                    np.array(vals, dtype=float))
        else:
            raise t.error(f"unknown section `{sec}'")
    return odf


# Results of known types (see Neper's documentation, "Simulation Results")
_SIM_TYPES = {}
for _names, _type in [
        ("coo disp vel rss crss slip sliprate rotrate rotrate_spin rotrate_slip", "vector"),
        ("stress strain strain_el strain_pl defrate defrate_pl", "symtensor"),
        ("velgrad", "tensor"),
        ("spinrate", "skewtensor"),
        ("ori", "ori"),
        ("stress_eq strain_eq strain_el_eq strain_pl_eq defrate_eq defrate_pl_eq work work_pl "
         "workrate workrate_pl", "real")]:
    for _name in _names.split():
        _SIM_TYPES[_name] = _type

# Number of components of the result types
_SIM_TYPE_SIZE = {"int": 1, "real": 1, "vector": 3, "symtensor": 6, "skewtensor": 3,
                  "tensor": 9}


class Sim(_Data):
    """Simulation directory (.sim), read by :func:`read_sim`.

    Attributes: ``inputs`` (dictionary: extension → input file),
    ``entities`` (result entities, e.g. ``nodes``, ``elts``, ``elsets``,
    ``mesh``), ``steps`` (step numbers).

    Methods: :meth:`results` (result names of an entity), :meth:`read` (a
    result at a step or at all steps), :meth:`read_all` (all results),
    :meth:`result_type`, :meth:`write` (writes a new result, which can then
    be used by modules -S and -V), :meth:`remove`, and :meth:`tess`,
    :meth:`mesh` and :meth:`tesr` (input files, read once).  Attribute
    ``orides`` is the orientation descriptor of the orientation results."""

    def __init__(self, path):
        self.path = Path(path)
        self._cache = {}
        self.inputs = {}
        self.entities = []
        self.steps = []
        self.refresh()

    def refresh(self):
        """Update the inputs, entities and steps from the directory (e.g.
        after running module -S)."""
        inputs = sorted(p for p in (self.path / "inputs").glob("*") if p.is_file())
        self.inputs = {}
        for p in inputs:
            ext = p.suffix.lstrip(".")
            if ext and (ext not in self.inputs or p.stem == "simulation"):
                self.inputs[ext] = p
        res = self.path / "results"
        self.entities = sorted(p.name for p in res.iterdir() if p.is_dir()) \
            if res.is_dir() else []
        steps = set()
        if res.is_dir():
            for f in res.glob("*/*/*.step*"):
                m = re.search(r"\.step(\d+)$", f.name)
                if m:
                    steps.add(int(m.group(1)))
        self.steps = sorted(steps)

    def results(self, entity):
        """Names of the results of *entity* (e.g. ``"nodes"``)."""
        d = self.path / "results" / entity
        return sorted(p.name for p in d.iterdir() if p.is_dir()) if d.is_dir() else []

    def result_steps(self, entity, res):
        """Steps of a result."""
        d = self.path / "results" / entity / res
        return sorted(int(m.group(1)) for f in d.glob(f"{res}.step*")
                      if (m := re.search(r"\.step(\d+)$", f.name)))

    def result_type(self, entity, res):
        """Type of a result: from its definition file, or from its name for
        the results of known type (e.g. ``disp``: ``"vector"``), or None."""
        typ = _def_file(self.path / "results" / entity / res / f"{res}.def").get("type")
        return typ or _SIM_TYPES.get(res)

    def result_expr(self, entity, res):
        """Expression of a result (from its definition file), or None."""
        expr = _def_file(self.path / "results" / entity / res / f"{res}.def").get("expr")
        return " ".join(expr) if expr else None

    def entity_def(self, entity):
        """Definition of a user-defined entity (dictionary with keys ``type``
        and ``member``), or None."""
        f = self.path / "results" / entity / f"{entity}.def"
        return _def_file(f) if f.is_file() else None

    @property
    def orides(self):
        """Orientation descriptor of the orientation results (that of the
        input mesh, or tessellation), e.g. ``"rodrigues:active"``."""
        if "msh" in self.inputs:
            des = self.mesh().elset_orides
            if des:
                return des
        if "tess" in self.inputs:
            return self.tess().cell_orides
        if "tesr" in self.inputs:
            return self.tesr().cell_orides
        return None

    def entity_qty(self, entity):
        """Number of members of an entity (from the input files), or None
        if unknown."""
        if entity == "mesh" or entity in ("tess", "tesr"):
            return 1
        if entity in ("nodes", "elts", "elsets") and "msh" in self.inputs:
            try:
                mesh = self.mesh()
            except (ValueError, OSError):
                return None
            if not mesh.node_qty:
                return None
            if entity == "nodes":
                return mesh.node_qty
            elts = mesh.elts()
            return elts.elt_qty if entity == "elts" else int(elts.elt_elset.max(initial=0))
        if entity == "cells" and "tess" in self.inputs:
            return self.tess().cell_qty
        return None

    def read(self, entity, res, step=None):
        """Read result *res* of *entity*, at step *step*, as an array of shape
        ``(n, k)`` (one row per member of the entity, one column per
        component; ``(n,)`` for a single component).  If *step* is None,
        read all steps, as an array of shape ``(steps, n, k)`` (or
        ``(steps, n)``)."""
        np = _np()
        d = self.path / "results" / entity / res
        if step is None:
            steps = self.result_steps(entity, res)
            if not steps:
                files = sorted(f.name for f in d.glob("*") if f.suffix != ".def")
                raise FileNotFoundError(f"{d}: no step files (files: {', '.join(files)})")
            return np.stack([self.read(entity, res, s) for s in steps])
        f = d / f"{res}.step{step}"
        if not f.is_file():
            raise FileNotFoundError(f"{f} not found")
        arr = _loadtable(f)
        if isinstance(arr, np.ndarray) and arr.ndim == 2 and arr.shape[1] == 1:
            arr = arr[:, 0]
        return arr

    def read_all(self, entities=None):
        """Read all results (of *entities*, default all), as a dictionary
        entity → result → array of shape ``(steps, n, k)``."""
        out = {}
        for entity in entities or self.entities:
            out[entity] = {}
            for res in self.results(entity):
                if self.result_steps(entity, res):
                    out[entity][res] = self.read(entity, res)
        return out

    def write(self, entity, res, values, step=None, type=None, expr=None):
        """Write result *res* of *entity* (e.g. ``"elsets"``), which can then
        be used by modules -S and -V (as any other result).

        *values* is an array of shape ``(n, k)`` (or ``(n,)``) for step
        *step*, or, if *step* is None, of shape ``(steps, n, k)`` (or
        ``(steps, n)``) for all steps (``steps`` must then be the number of
        steps of the simulation).  The result *type* (``"int"``, ``"real"``,
        ``"vector"``, ``"symtensor"``, ``"skewtensor"``, ``"tensor"`` or
        ``"ori"``) is written to the definition file; it is determined from
        the values if not provided (``"int"`` or ``"real"`` for a single
        component, ``"vector"``, ``"symtensor"`` or ``"tensor"`` for 3, 6 or 9
        components).  *expr* is an optional description written to the
        definition file.  A result used in an expression of module -S must
        be written for all steps."""
        np = _np()
        values = np.asarray(values)
        if step is None:
            steps = self.steps or list(range(len(values)))
            if len(values) != len(steps):
                raise ValueError(f"{len(values)} steps of values for {len(steps)} steps "
                                 "(provide step=... to write a single step)")
            pairs = list(zip(steps, values))
        else:
            pairs = [(step, values)]

        ncomp = None
        for _, val in pairs:
            val = val.reshape(len(val), -1) if val.ndim else val.reshape(1, 1)
            ncomp = val.shape[1]
            qty = self.entity_qty(entity)
            if qty is not None and len(val) != qty:
                raise ValueError(f"{len(val)} values for {qty} {entity}")

        if type is None:
            if ncomp == 1:
                type = "int" if values.dtype.kind in "iub" else "real"
            else:
                type = {3: "vector", 6: "symtensor", 9: "tensor"}.get(ncomp)
                if type is None:
                    raise ValueError(f"cannot determine the type of {ncomp}-component values "
                                     "(provide type=...)")
        elif type in _SIM_TYPE_SIZE and _SIM_TYPE_SIZE[type] != ncomp:
            raise ValueError(f"{ncomp} components for type {type}")

        d = self.path / "results" / entity / res
        d.mkdir(parents=True, exist_ok=True)
        fmt = "%d" if type == "int" else "%.12g"
        for s, val in pairs:
            val = np.asarray(val).reshape(len(val), -1) if np.ndim(val) else np.reshape(val, (1, 1))
            if type == "int":
                val = val.astype(np.int64)
            np.savetxt(d / f"{res}.step{s}", val, fmt=fmt)
        text = f"*type\n {type}\n"
        if expr:
            text += f"*expr\n {expr}\n"
        (d / f"{res}.def").write_text(text)
        self.refresh()

    def remove(self, entity, res):
        """Remove result *res* of *entity*."""
        d = self.path / "results" / entity / res
        if not d.is_dir():
            raise FileNotFoundError(f"{d} not found")
        shutil.rmtree(d)
        self.refresh()

    def tess(self):
        """Read the input tessellation (:class:`Tess`, read once)."""
        if "tess" not in self._cache:
            self._cache["tess"] = read_tess(self.inputs["tess"])
        return self._cache["tess"]

    def tesr(self):
        """Read the input raster tessellation (:class:`Tesr`, read once)."""
        if "tesr" not in self._cache:
            self._cache["tesr"] = read_tesr(self.inputs["tesr"])
        return self._cache["tesr"]

    def mesh(self):
        """Read the input mesh (:class:`Mesh`, read once)."""
        if "msh" not in self._cache:
            self._cache["msh"] = read_mesh(self.inputs["msh"])
        return self._cache["msh"]


def _def_file(path):
    out = {}
    if not path.is_file():
        return out
    key = None
    for tok in path.read_text().split():
        if tok.startswith("*"):
            key = tok[1:]
            out.setdefault(key, [])
        elif key:
            out[key].append(tok)
    return {k: (v[0] if k == "type" and v else v) for k, v in out.items()}


def read_sim(path):
    """Read a simulation directory (.sim) and return a :class:`Sim`
    (results are read on demand)."""
    path = Path(path)
    if not (path / "results").is_dir() and not (path / "inputs").is_dir():
        raise FileNotFoundError(f"{path}: not a simulation directory")
    return Sim(path)


def _loadtable(path):
    """Table of numbers (one row per line), as a 2D array; rows of different
    lengths give a list of 1D arrays, and non-numeric values strings."""
    np = _np()
    text = Path(path).read_text()
    isint = not re.search(r"[.eEnN]", text)
    lines = [l for l in text.splitlines() if l.strip()]
    if lines:
        try:  # fast path: table of numbers
            arr = np.loadtxt(lines, ndmin=2, comments=None, dtype=int if isint else float)
            return arr
        except ValueError:  # rows of different lengths, or strings
            pass
    rows = [l.split() for l in lines]
    if not rows:
        return np.zeros((0, 0))
    try:
        rows = [[float(v) for v in r] for r in rows]
        numeric = True
    except ValueError:
        numeric = False
    if len({len(r) for r in rows}) == 1:
        arr = np.array(rows, dtype=float if numeric else object)
        if numeric and isint:
            arr = arr.astype(int)
        return arr
    return [np.array(r, dtype=float if numeric else object) for r in rows]


def read_stat(path, keys=None):
    """Read a statistics file (e.g. .stcell, .stelt), as an array with one row
    per entity and one column per value (integers if all values are).  If
    *keys* is provided (list, or comma-separated string, as provided to
    option -stat*), return a dictionary key → column instead (each key must
    then give a single value)."""
    arr = _loadtable(path)
    if keys is None:
        return arr
    if isinstance(keys, str):
        keys = _split_keys(keys)
    if isinstance(arr, list) or arr.ndim != 2 or arr.shape[1] != len(keys):
        ncol = "variable" if isinstance(arr, list) else arr.shape[1]
        raise ValueError(f"{path}: {ncol} columns for {len(keys)} keys (a key gives "
                         "several values?); read without keys")
    return {k: arr[:, i] for i, k in enumerate(keys)}


def _split_keys(string):
    """Split a comma-separated list of keys, ignoring the commas inside
    parentheses (e.g. ``"vol,x,body(1,2)"``)."""
    keys, depth, cur = [], 0, ""
    for c in string:
        if c == "," and depth == 0:
            keys.append(cur)
            cur = ""
            continue
        depth += (c == "(") - (c == ")")
        cur += c
    keys.append(cur)
    return [k for k in keys if k]


_READERS = {".odf": read_odf, ".tess": read_tess, ".tesr": read_tesr, ".msh": read_mesh,
            ".sim": read_sim}


def read(path):
    """Read a Neper file, according to its extension: .tess (:func:`read_tess`),
    .tesr (:func:`read_tesr`), .msh (:func:`read_mesh`), simulation directory
    (:func:`read_sim`), statistics files .st* (:func:`read_stat`), and other
    tables of numbers (e.g. .ori)."""
    path = Path(path)
    if path.is_dir():
        return read_sim(path)
    reader = _READERS.get(path.suffix)
    if reader:
        return reader(path)
    return _loadtable(path)


# Data files ##################################################################

_TMPDIR = None


def _tmpdir():
    global _TMPDIR
    if _TMPDIR is None:
        _TMPDIR = tempfile.mkdtemp(prefix="neper-")
        atexit.register(shutil.rmtree, _TMPDIR, True)
    return _TMPDIR


class DataFile:
    """Data written to a file, to be passed to Neper (see :func:`datafile`).

    ``str (datafile)`` is ``file(<path>)``, and attribute ``path`` is the
    file path."""

    def __init__(self, path):
        self.path = Path(path)

    def __str__(self):
        return f"file({self.path})"

    def __fspath__(self):
        return str(self.path)

    def __repr__(self):
        return f"<neper.DataFile {self.path}>"


def datafile(data, path=None, fmt=None):
    """Write *data* to a data file and return a :class:`DataFile`, which
    gives ``file(<path>)`` when passed to Neper (as an option value or in a
    string).

    *data* can be a 1D or 2D array (or list), with one row per entity
    (seed, cell, etc.), of numbers or strings (e.g. colors).  The file is
    written to *path* if provided, and to a temporary file (deleted when
    Python exits) otherwise.  Real numbers are written exactly, or with
    format *fmt* (e.g. ``"%.6f"``) if provided."""
    if path is None:
        fd, path = tempfile.mkstemp(suffix=".txt", dir=_tmpdir())
        os.close(fd)
    path = Path(path).resolve()
    lines = []
    for row in data:
        if hasattr(row, "tolist"):
            row = row.tolist()
        if not isinstance(row, (list, tuple)):
            row = [row]
        lines.append(" ".join((fmt % v if fmt else repr(v)) if isinstance(v, float)
                              else str(v) for v in row))
    path.write_text("\n".join(lines) + "\n")
    return DataFile(path)
