# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

"""Orilib, Neper's library of orientation functions, from Python.

The functions of the orilib program are available as Python functions of the
same names, which apply to NumPy arrays::

    from neper import orilib

    orilib.ol_e_q([0, 30, 0])                       # quaternion, shape (4,)
    orilib.ol_e_q(euler)                            # euler: (n, 3) -> (n, 4)
    orilib.ol_q_q_disori(q1, q2, "cubic")           # disorientation angles

Each argument is an array whose last axis (or two last axes, for a rotation
matrix) holds the values of one item (e.g. the 4 components of a
quaternion), and whose leading axes, broadcast between the arguments, index
the items.  The results have the leading axes, followed by the axes of an
item.  See :func:`signature` (or ``help (orilib.<function>)``) for the
arguments of a function, and :func:`commands` for the list of functions.

The functions run the orilib program (installed with neper), once for all
items.
"""

import os
import re
import shutil
import subprocess

from ._core import NeperError, _error_message, _preexec, get_defaults
from ._orilib_doc import DOCS

__all__ = ["call", "commands", "signature", "run", "executable", "OrilibError"]

# shapes and kinds of the values of the types
_TYPES = {"q": ((4,), "f"), "g": ((3, 3), "f"), "vect": ((3,), "f"), "e": ((3,), "f"),
          "rtheta": ((4,), "f"), "r": ((3,), "f"), "p": ((2,), "f"), "m": ((6,), "i"),
          "R": ((3,), "f"), "pole": ((3,), "i"), "hpole": ((4,), "i"), "theta": ((), "f"),
          "int": ((), "i"), "real": ((), "f"), "str": ((), "s")}

_DESCRIPTIONS = {"q": "quaternion", "g": "rotation matrix", "vect": "vector",
                 "e": "Euler angles", "rtheta": "rotation axis / angle pair",
                 "r": "rotation axis", "p": "projection point", "m": "Miller indices",
                 "R": "Rodrigues vector", "pole": "pole (Miller indices)",
                 "hpole": "pole (4-index Miller indices)", "theta": "angle",
                 "int": "integer", "real": "real", "str": "string"}


class OrilibError(NeperError):
    """The orilib program failed (e.g. invalid input data)."""


def _type(name):
    """Shape and kind (``f``, ``i`` or ``s``) of a type, and whether it is a
    list (variable number of values)."""
    islist = name.endswith("*")
    name = name.rstrip("*")
    if name in _TYPES:
        shape, kind = _TYPES[name]
    else:
        m = re.fullmatch(r"(real|int)(\d+)(?:x(\d+))?", name)
        if not m:
            raise ValueError(f"unknown type `{name}'")
        shape = tuple(int(x) for x in m.groups()[1:] if x)
        kind = "f" if m.group(1) == "real" else "i"
    return shape, kind, islist


def executable():
    """Return the path of the orilib program: environment variable
    ``ORILIB_EXECUTABLE``, else ``orilib`` next to the neper program (see
    :func:`neper.executable`), else ``orilib`` in the PATH."""
    exe = os.environ.get("ORILIB_EXECUTABLE")
    if exe:
        exe = os.path.expanduser(exe)
        if not (os.path.isfile(exe) and os.access(exe, os.X_OK)):
            raise FileNotFoundError(f"orilib program `{exe}' not found")
        return os.path.abspath(exe)
    try:
        from ._core import _find_executable

        neper = _find_executable()
        for d in {os.path.dirname(neper), os.path.dirname(os.path.realpath(neper))}:
            exe = os.path.join(d, "orilib")
            if os.path.isfile(exe) and os.access(exe, os.X_OK):
                return exe
    except FileNotFoundError:
        pass
    exe = shutil.which("orilib")
    if exe is None:
        raise FileNotFoundError(
            "orilib program not found: it is built and installed with neper "
            "(or set ORILIB_EXECUTABLE)")
    return exe


_SIGNATURES = {}


def _commands():
    """Inputs and outputs of the functions, as given by the orilib program
    (orilib signatures): name -> {"in": [(name, type)], "out": [...]}."""
    exe = executable()
    if exe not in _SIGNATURES:
        sigs = {}
        for line in run("signatures", "").splitlines():
            toks = line.split()
            if not toks:
                continue
            sig = {"in": [], "out": []}
            for tok in toks[1:]:
                name, typ = tok.lstrip(">").split(":")
                sig["out" if tok.startswith(">") else "in"].append((name, typ))
            sigs[toks[0]] = sig
        _SIGNATURES[exe] = sigs
    return _SIGNATURES[exe]


def commands():
    """Return the names of the functions available (sorted)."""
    return sorted(_commands())


def signature(command):
    """Return the signature of a function, as a string, e.g.
    ``"ol_q_q_disori (q1: q, q2: q, crysym: str) -> theta: theta"``."""
    sig = _sig(command)
    ins = ", ".join(f"{n}: {t}" for n, t in sig["in"])
    outs = ", ".join(f"{n}: {t}" for n, t in sig["out"])
    return f"{command} ({ins}) -> {outs}"


def _sig(command):
    commands = _commands()
    if command not in commands:
        if command in run("listall", "").split():
            raise ValueError(f"function `{command}' is not available in Python "
                             "(use orilib.run)")
        raise AttributeError(f"unknown orilib function `{command}'")
    return commands[command]


_ENDMARK = "@orilib-end@"


def run(command, text, loop=False):
    """Run the orilib program on command *command* with input data *text*
    (string), in loop mode if *loop* is True, and return its output (string),
    with full-precision real values."""
    return _run(command, text, loop)


def _run(command, text, loop, endmark=None):
    env = dict(os.environ)
    env["ORILIB_FORMAT"] = "%.17g"
    if endmark:
        env["ORILIB_ENDMARK"] = endmark
    threads = get_defaults()["threads"]
    if threads is not None:
        env["OMP_NUM_THREADS"] = str(int(threads))
    cmd = [executable()] + (["loop"] if loop else []) + [command]
    proc = subprocess.run(cmd, input=text, capture_output=True, text=True, env=env,
                          preexec_fn=_preexec)
    if proc.returncode != 0:
        output = proc.stdout + proc.stderr
        raise OrilibError(_error_message(output, proc.returncode, "orilib"), proc.returncode,
                          cmd, output)
    return proc.stdout


def call(command, *args):
    """Run Orilib function *command* on *args* (see the module documentation),
    as in ``call("ol_e_q", [0, 30, 0])``.  Return the output (or a tuple of
    outputs).  If an output is a list whose length differs between the items
    (e.g. the equivalent poles of different poles), a list of the item
    outputs is returned instead."""
    import numpy as np

    sig = _sig(command)
    if len(args) != len(sig["in"]):
        raise TypeError(f"{signature(command)}: {len(sig['in'])} arguments expected, "
                        f"{len(args)} given")

    # arguments: arrays of shape (batch + item)
    arrays, batches = [], []
    for (name, typ), arg in zip(sig["in"], args):
        shape, kind, _ = _type(typ)
        if kind == "s":
            arr = np.asarray(arg, dtype=object)
        else:
            arr = np.asarray(arg)
            if kind == "i":
                if arr.size and not np.all(np.equal(np.mod(arr, 1), 0)):
                    raise ValueError(f"{name}: integer values expected")
                arr = arr.astype(np.int64)
            else:
                arr = arr.astype(float)
        nd = arr.ndim - len(shape)
        if nd < 0 or arr.shape[nd:] != shape:
            raise ValueError(f"{name} ({_DESCRIPTIONS.get(typ, typ)}): shape (..., "
                             + ", ".join(map(str, shape)) + f") expected, {arr.shape} given")
        arrays.append((arr, shape, kind))
        batches.append(arr.shape[:nd])
    batch = np.broadcast_shapes(*batches) if batches else ()
    n = int(np.prod(batch)) if batch else 1

    # input data: one line per item
    columns = []
    for arr, shape, kind in arrays:
        arr = np.broadcast_to(arr, batch + shape).reshape(n, int(np.prod(shape)))
        if kind == "f":
            columns.append(np.char.mod("%.17g", arr))
        elif kind == "i":
            columns.append(arr.astype(str))
        else:
            vals = arr.astype(str)
            if any(not v or re.search(r"\s", v) for v in vals.ravel()):
                raise ValueError("string arguments must be non-empty and contain no spaces")
            columns.append(vals)
    if n == 0:
        text = ""
    else:
        table = np.concatenate(columns, axis=1) if columns else np.empty((n, 0), str)
        rows = [" ".join(r) for r in table.tolist()]
        text = _run(command, "\n".join(rows) + "\n", True, _ENDMARK)
    outs = _parse(command, sig, text, n, batch)
    if isinstance(outs, list):
        return outs
    return outs[0] if len(outs) == 1 else tuple(outs)


def _parse(command, sig, text, n, batch):
    """Outputs (tuple of arrays, or list of item outputs if their sizes
    differ) from the output of loop mode (items ended by _ENDMARK)."""
    import warnings

    import numpy as np

    items, cur, messages = [], [], []
    for line in text.splitlines():
        if line == _ENDMARK:
            items.append(" ".join(cur).split())
            cur = []
        elif line.startswith(("Warning", "Info   :")):
            messages.append(line.split(":", 1)[1].strip())
        else:
            cur.append(line)
    for message in dict.fromkeys(messages):
        warnings.warn(f"{command}: {message}", stacklevel=4)
    if len(items) != n:
        raise OrilibError(f"{command}: {len(items)} outputs for {n} items")

    types = [_type(t) for _, t in sig["out"]]
    sizes = [int(np.prod(shape)) for shape, _, _ in types]
    fixed = sum(s for s, (_, _, islist) in zip(sizes, types) if not islist)
    nlist = [i for i, (_, _, islist) in enumerate(types) if islist]

    def counts(qty):
        """Numbers of values of the outputs, for qty values."""
        res = [1] * len(types)
        if nlist:
            extra = qty - fixed
            if extra < 0 or extra % sizes[nlist[0]]:
                return None
            res[nlist[0]] = extra // sizes[nlist[0]]
        elif qty != fixed:
            return None
        return res

    for toks in items:
        if counts(len(toks)) is None:
            reason = " ".join(dict.fromkeys(messages)) or \
                f"unexpected output ({len(toks)} values, {fixed} expected)"
            raise OrilibError(f"{command}: {reason}")

    def convert(toks, cnts, lead):
        outs, col = [], 0
        table = np.array(toks, dtype=object).reshape(lead + (-1,)) if toks else \
            np.zeros(lead + (sum(s * c for s, c in zip(sizes, cnts)),), object)
        for (shape, kind, islist), size, cnt in zip(types, sizes, cnts):
            vals = table[..., col:col + size * cnt]
            col += size * cnt
            if kind == "f":
                vals = vals.astype(float)
            elif kind == "i":
                vals = vals.astype(float).astype(np.int64)
            else:
                vals = vals.astype(str)
            out = vals.reshape(lead + ((cnt,) if islist else ()) + shape)
            outs.append(out[()] if out.ndim == 0 else out)
        return outs

    lengths = {len(toks) for toks in items}
    if len(lengths) <= 1:  # same sizes: arrays of shape (batch + item)
        qty = lengths.pop() if lengths else fixed
        cnts = counts(qty) or [0 if i in nlist else 1 for i in range(len(types))]
        flat = [t for toks in items for t in toks]
        outs = convert(flat, cnts, (n,))
        return tuple(o.reshape(tuple(batch) + o.shape[1:]) if o.ndim else o for o in outs)

    # different sizes: list of item outputs
    res = []
    for toks in items:
        outs = convert(toks, counts(len(toks)), ())
        res.append(outs[0] if len(outs) == 1 else tuple(outs))
    return res


def _function(command):
    sig = _sig(command)

    def f(*args):
        return call(command, *args)

    f.__name__ = f.__qualname__ = command
    lines = [signature(command), ""]
    lines.append("Arguments: " + ", ".join(
        f"{n} ({_DESCRIPTIONS.get(t.rstrip('*'), t)})" for n, t in sig["in"]) + ".")
    lines.append("Returns: " + ", ".join(
        f"{n} ({_DESCRIPTIONS.get(t.rstrip('*'), t)}{', list' if t.endswith('*') else ''})"
        for n, t in sig["out"]) + ".")
    if command in DOCS:
        lines += ["", "C function: " + DOCS[command]["proto"], "", DOCS[command]["doc"]]
    lines += ["", "Arrays apply item by item (see the documentation of neper.orilib)."]
    f.__doc__ = "\n".join(lines)
    return f


_FUNCTIONS = {}


def __getattr__(name):
    if name.startswith("ol_"):
        if name not in _FUNCTIONS:
            _FUNCTIONS[name] = _function(name)
        return _FUNCTIONS[name]
    raise AttributeError(f"module 'neper.orilib' has no attribute '{name}'")


def __dir__():
    try:
        names = list(_commands())
    except (FileNotFoundError, OrilibError):
        names = []
    return sorted(list(globals()) + names)
