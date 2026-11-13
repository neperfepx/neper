# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

"""Conversions between orientation descriptors, following Neper's
conventions (see the documentation, "Rotations and Orientations")."""

from ._io import _np, _orides

__all__ = ["ori_convert", "ori_angle"]

_SIZE = {"rodrigues": 3, "euler-bunge": 3, "euler-kocks": 3, "euler-roe": 3, "rotmat": 9,
         "axis-angle": 4, "quaternion": 4}


def _split(des):
    name, conv = _orides(des).split(":")
    if name not in _SIZE:
        raise ValueError(f"unknown orientation descriptor `{name}' (expected one of "
                         + ", ".join(_SIZE) + ")")
    return name, conv


def ori_convert(values, des_from, des_to="quaternion"):
    """Convert orientations from descriptor *des_from* to descriptor
    *des_to*, e.g. ``ori_convert(tess.cell_ori, tess.cell_orides,
    "euler-bunge")``.

    The descriptors are those of Neper (``rodrigues``, ``euler-bunge``,
    ``euler-kocks``, ``euler-roe``, ``rotmat``, ``axis-angle`` and
    ``quaternion``), optionally followed by the convention (``:passive``,
    the default, or ``:active``).  *values* is an array of shape ``(..., k)``,
    with ``k`` the number of parameters of the descriptor (angles in
    degrees; a rotation matrix is given by its 9 components, row by row),
    and the returned array has shape ``(..., k')``.  The values are those
    written by Neper: Euler angles in [0, 360) x [0, 180] x [0, 360),
    rotation angles in [0, 180] and quaternions of non-negative first
    component."""
    np = _np()
    name1, conv1 = _split(des_from)
    name2, conv2 = _split(des_to)
    values = np.asarray(values, dtype=float)
    if values.shape[-1] != _SIZE[name1]:
        raise ValueError(f"{values.shape[-1]} values per orientation for descriptor "
                         f"{name1} ({_SIZE[name1]} expected)")
    q = _to_q(values, name1)
    if conv1 != conv2:  # opposite rotation
        q = q * np.array([1, -1, -1, -1])
    return _from_q(q, name2)


def ori_angle(values1, values2, des="quaternion"):
    """Rotation angle (in degrees) between orientations *values1* and
    *values2* (arrays of shape ``(..., k)``, in descriptor *des*), not
    accounting for crystal symmetry."""
    np = _np()
    q1 = ori_convert(values1, des, "quaternion")
    q2 = ori_convert(values2, des, "quaternion")
    dot = np.abs((q1 * q2).sum(axis=-1))
    return 2 * np.degrees(np.arccos(np.clip(dot, 0, 1)))


def _q_normalize(q):
    np = _np()
    q = q / np.linalg.norm(q, axis=-1, keepdims=True)
    return np.where(q[..., :1] < 0, -q, q)


def _q_from_axis_angle(axis, angle):
    np = _np()
    axis = axis / np.linalg.norm(axis, axis=-1, keepdims=True)
    half = np.radians(angle)[..., None] / 2
    return np.concatenate([np.cos(half), np.sin(half) * axis], axis=-1)


def _to_q(v, name):
    """Quaternion (passive convention preserved) from descriptor values."""
    np = _np()
    if name == "quaternion":
        return _q_normalize(v)
    if name == "rodrigues":
        return _q_normalize(np.concatenate([np.ones(v.shape[:-1] + (1,)), v], axis=-1))
    if name == "axis-angle":
        return _q_normalize(_q_from_axis_angle(v[..., :3], v[..., 3]))
    if name == "rotmat":
        return _q_from_g(v.reshape(v.shape[:-1] + (3, 3)))
    # Euler angles, converted to Bunge's
    e = v.copy()
    if name == "euler-kocks":
        e[..., 0] = v[..., 0] + 90
        e[..., 2] = 90 - v[..., 2]
    elif name == "euler-roe":
        e[..., 0] = v[..., 0] + 90
        e[..., 2] = v[..., 2] - 90
    return _q_from_g(_g_from_bunge(e))


def _from_q(q, name):
    np = _np()
    q = _q_normalize(q)
    if name == "quaternion":
        return q
    if name == "rodrigues":
        with np.errstate(divide="ignore", invalid="ignore"):
            return q[..., 1:] / q[..., :1]
    if name == "axis-angle":
        angle = 2 * np.degrees(np.arccos(np.clip(q[..., 0], -1, 1)))
        norm = np.linalg.norm(q[..., 1:], axis=-1, keepdims=True)
        axis = np.where(norm > 1e-15, q[..., 1:] / np.where(norm > 0, norm, 1),
                        np.array([1.0, 0, 0]))
        return np.concatenate([axis, angle[..., None]], axis=-1)
    g = _g_from_q(q)
    if name == "rotmat":
        return g.reshape(g.shape[:-2] + (9,))
    e = _bunge_from_g(g)
    if name == "euler-kocks":
        e[..., 0] = e[..., 0] - 90
        e[..., 2] = 90 - e[..., 2]
    elif name == "euler-roe":
        e[..., 0] = e[..., 0] - 90
        e[..., 2] = e[..., 2] + 90
    e[..., 0] %= 360
    e[..., 2] %= 360
    return e


def _g_from_q(q):
    """Rotation matrix g (passive: sample to crystal) of a quaternion."""
    np = _np()
    w, x, y, z = (q[..., i] for i in range(4))
    g = np.empty(q.shape[:-1] + (3, 3))
    g[..., 0, 0] = 1 - 2 * (y * y + z * z)
    g[..., 0, 1] = 2 * (x * y + w * z)
    g[..., 0, 2] = 2 * (x * z - w * y)
    g[..., 1, 0] = 2 * (x * y - w * z)
    g[..., 1, 1] = 1 - 2 * (x * x + z * z)
    g[..., 1, 2] = 2 * (y * z + w * x)
    g[..., 2, 0] = 2 * (x * z + w * y)
    g[..., 2, 1] = 2 * (y * z - w * x)
    g[..., 2, 2] = 1 - 2 * (x * x + y * y)
    return g


def _q_from_g(g):
    """Quaternion of a rotation matrix (inverse of _g_from_q), robust
    (Shepperd's method)."""
    np = _np()
    g = np.asarray(g, dtype=float)
    # g is the transpose of the "active" matrix R = g^T
    r = np.swapaxes(g, -1, -2)
    tr = r[..., 0, 0] + r[..., 1, 1] + r[..., 2, 2]
    cand = np.stack([tr, r[..., 0, 0], r[..., 1, 1], r[..., 2, 2]], axis=-1)
    k = np.argmax(cand, axis=-1)
    q = np.empty(g.shape[:-2] + (4,))
    # w largest
    s = np.sqrt(np.maximum(1 + tr, 1e-300)) * 2
    qw = np.stack([s / 4, (r[..., 2, 1] - r[..., 1, 2]) / s, (r[..., 0, 2] - r[..., 2, 0]) / s,
                   (r[..., 1, 0] - r[..., 0, 1]) / s], axis=-1)
    s = np.sqrt(np.maximum(1 + r[..., 0, 0] - r[..., 1, 1] - r[..., 2, 2], 1e-300)) * 2
    qx = np.stack([(r[..., 2, 1] - r[..., 1, 2]) / s, s / 4, (r[..., 0, 1] + r[..., 1, 0]) / s,
                   (r[..., 0, 2] + r[..., 2, 0]) / s], axis=-1)
    s = np.sqrt(np.maximum(1 + r[..., 1, 1] - r[..., 0, 0] - r[..., 2, 2], 1e-300)) * 2
    qy = np.stack([(r[..., 0, 2] - r[..., 2, 0]) / s, (r[..., 0, 1] + r[..., 1, 0]) / s, s / 4,
                   (r[..., 1, 2] + r[..., 2, 1]) / s], axis=-1)
    s = np.sqrt(np.maximum(1 + r[..., 2, 2] - r[..., 0, 0] - r[..., 1, 1], 1e-300)) * 2
    qz = np.stack([(r[..., 1, 0] - r[..., 0, 1]) / s, (r[..., 0, 2] + r[..., 2, 0]) / s,
                   (r[..., 1, 2] + r[..., 2, 1]) / s, s / 4], axis=-1)
    for i, qi in enumerate((qw, qx, qy, qz)):
        q = np.where((k == i)[..., None], qi, q)
    return _q_normalize(q)


def _g_from_bunge(e):
    np = _np()
    p1, P, p2 = (np.radians(e[..., i]) for i in range(3))
    c1, s1, c, s, c2, s2 = np.cos(p1), np.sin(p1), np.cos(P), np.sin(P), np.cos(p2), np.sin(p2)
    g = np.empty(e.shape[:-1] + (3, 3))
    g[..., 0, 0] = c1 * c2 - s1 * s2 * c
    g[..., 0, 1] = s1 * c2 + c1 * s2 * c
    g[..., 0, 2] = s2 * s
    g[..., 1, 0] = -c1 * s2 - s1 * c2 * c
    g[..., 1, 1] = -s1 * s2 + c1 * c2 * c
    g[..., 1, 2] = c2 * s
    g[..., 2, 0] = s1 * s
    g[..., 2, 1] = -c1 * s
    g[..., 2, 2] = c
    return g


def _bunge_from_g(g):
    np = _np()
    P = np.arccos(np.clip(g[..., 2, 2], -1, 1))
    sing = np.abs(np.sin(P)) < 1e-9
    p1 = np.where(sing, np.arctan2(g[..., 0, 1], g[..., 0, 0]),
                  np.arctan2(g[..., 2, 0], -g[..., 2, 1]))
    p2 = np.where(sing, 0.0, np.arctan2(g[..., 0, 2], g[..., 1, 2]))
    e = np.degrees(np.stack([p1, P, p2], axis=-1))
    e[..., 0] %= 360
    e[..., 2] %= 360
    return e
