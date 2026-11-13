# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

"""Tests of neper.orilib (require NumPy and the orilib program)."""

import sys
import unittest
import warnings
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

import neper  # noqa: E402
from neper import orilib  # noqa: E402

try:
    import numpy as np
except ImportError:  # pragma: no cover
    np = None

ROOT = Path(__file__).resolve().parents[2]


def _have_orilib():
    try:
        orilib.executable()
        return True
    except FileNotFoundError:
        return False


class TestDoc(unittest.TestCase):

    @unittest.skipUnless((ROOT / "dev" / "orilib").is_dir(), "source tree not available")
    def test_doc_up_to_date(self):
        # (regenerate with: python3 python/tools/orilib_doc.py)
        sys.path.insert(0, str(ROOT / "python" / "tools"))
        import orilib_doc

        current = (ROOT / "python" / "neper" / "_orilib_doc.py").read_text()
        self.assertEqual(orilib_doc.text(), current,
                         "_orilib_doc.py is out of date: run python3 python/tools/orilib_doc.py")


@unittest.skipUnless(_have_orilib(), "orilib program not found")
class TestSignature(unittest.TestCase):

    def test_signature(self):
        self.assertEqual(orilib.signature("ol_q_q_disori"),
                         "ol_q_q_disori (q1: q, q2: q, crysym: str) -> theta: theta")
        self.assertIn("ol_e_q", orilib.commands())
        self.assertIn("ol_e_q", dir(orilib))
        self.assertIn("C function: void ol_e_q (double* e, double* q)", orilib.ol_e_q.__doc__)
        with self.assertRaises(AttributeError):
            orilib.ol_foo
        with self.assertRaises(ValueError):  # not available in Python
            orilib.ol_set_filter


@unittest.skipIf(np is None, "numpy not available")
@unittest.skipUnless(_have_orilib(), "orilib program not found")
class TestCall(unittest.TestCase):

    def test_values(self):
        # 30 degrees about x (see the documentation, "Orientation Convention")
        q = orilib.ol_e_q([0, 30, 0])
        np.testing.assert_allclose(q, [0.965925826289068, 0.258819045102521, 0, 0], atol=1e-15)
        g = orilib.ol_e_g([0, 30, 0])
        self.assertEqual(g.shape, (3, 3))
        np.testing.assert_allclose(g, [[1, 0, 0], [0, 0.866025403784439, 0.5],
                                       [0, -0.5, 0.866025403784439]], atol=1e-15)
        self.assertEqual(orilib.ol_crysym_qty("cubic"), 24)
        self.assertEqual(orilib.ol_q_q_disori(q, q, "cubic"), 0)

    def test_arrays(self):
        rng = np.random.default_rng(0)
        e = rng.uniform(0, 90, (20, 3))
        q = orilib.ol_e_q(e)
        self.assertEqual(q.shape, (20, 4))
        # agreement with neper.ori_convert, and full precision
        np.testing.assert_allclose(q, neper.ori_convert(e, "euler-bunge", "quaternion"),
                                   atol=1e-14)
        np.testing.assert_allclose(orilib.ol_q_e(q), e, atol=1e-11)
        # batch shapes, broadcasting (with a single item, and a string per item)
        self.assertEqual(orilib.ol_e_g(e.reshape(4, 5, 3)).shape, (4, 5, 3, 3))
        d = orilib.ol_q_q_disori(q, q[0], "cubic")
        self.assertEqual(d.shape, (20,))
        self.assertEqual(d[0], 0)
        d2 = orilib.ol_q_q_disori(q[:2], q[0], ["cubic", "triclinic"])
        self.assertEqual(d2[0], 0)
        # equivalent orientations: zero disorientation
        qs = orilib.ol_q_crysym(q[:3], "cubic", 7)
        np.testing.assert_allclose(orilib.ol_q_q_disori(q[:3], qs, "cubic"), 0, atol=1e-6)
        # several outputs
        qd, status = orilib.ol_q_q_qdisori_update(q[0], q[1], "cubic", 0, "noupdate")
        self.assertEqual(qd.shape, (4,))
        # empty arrays
        self.assertEqual(orilib.ol_e_q(np.zeros((0, 3))).shape, (0, 4))

    def test_lists(self):
        # lists of same lengths: arrays
        self.assertEqual(orilib.ol_srand_e([1, 2], 3).shape, (2, 3, 3))
        q = orilib.ol_e_q([[10, 20, 30], [40, 50, 60]])
        self.assertEqual(orilib.ol_q_refsym_orthotropic(q).shape, (2, 4, 4))
        # lists of different lengths: list of arrays
        poles = orilib.ol_polef_polecrysym([[1, 0, 0], [1, 1, 2]], "cubic")
        self.assertEqual([p.shape for p in poles], [(3, 3), (12, 3)])

    def test_errors(self):
        with self.assertRaises(ValueError):  # wrong shape
            orilib.ol_e_q([1, 2])
        with self.assertRaises(TypeError):  # wrong number of arguments
            orilib.ol_e_q([1, 2, 3], [1, 2, 3])
        with self.assertRaises(orilib.OrilibError) as cm:
            orilib.ol_q_q_disori([1, 0, 0, 0], [1, 0, 0, 0], "foo")
        self.assertIn("Unknown crystal symmetry `foo'", cm.exception.message)
        with warnings.catch_warnings():
            warnings.simplefilter("ignore")
            with self.assertRaises(orilib.OrilibError):  # no output, after a warning
                orilib.ol_label_q("foo")

    def test_run(self):
        self.assertEqual(orilib.run("ol_e_q", "0 30 0\n").split(),
                         ["0.96592582628906831", "0.25881904510252074", "0", "0"])

    def test_all(self):
        # all functions run, on generic input data
        q = np.array([[0.9, 0.1, 0.2, 0.3], [0.5, -0.5, 0.5, 0.5]])
        q = q / np.linalg.norm(q, axis=1, keepdims=True)
        data = {"q": q, "g": orilib.ol_q_g(q), "e": [[10, 20, 30], [100, 50, 200]],
                "R": [[0.1, 0.2, 0.3], [-0.2, 0.1, 0.05]], "rtheta": [[0, 0, 1, 30], [1, 0, 0, 10]],
                "r": [[0, 0, 1], [0.6, 0.8, 0]], "vect": [[0, 0, 1], [0.6, 0.8, 0]],
                "p": [[0.1, 0.2], [0.3, -0.2]], "m": [[0, 0, 1, 1, 0, 0], [1, 1, 0, 0, 0, 1]],
                "pole": [[1, 0, 0], [1, 1, 1]], "hpole": [[1, 0, -1, 0], [0, 0, 0, 1]],
                "theta": [30, 10], "real3": [[0.2, 0.3, 0.5], [0.1, 0.1, 0.8]],
                "real3x3": [np.eye(3) * 0.1, np.eye(3) * 0.2]}
        strings = {"label": "Goss", "des": "rotmat", "flag": "update"}
        ints = {"i": 2, "qty": 3, "seed": 1, "prevnb": 1}
        reals = {"thetamax": 10.0, "alphaq": 0.5}
        failed = []
        for command in orilib.commands():
            if command == "ol_q_label":  # (requires a special orientation)
                continue
            args = []
            for name, typ in orilib._commands()[command]["in"]:
                if typ == "str":
                    args.append(strings.get(name, "cubic"))
                elif typ == "int":
                    args.append(ints.get(name, 1))
                elif typ == "real":
                    args.append(reals.get(name, 0.3))
                else:
                    args.append(data[typ])
            try:
                with warnings.catch_warnings():
                    warnings.simplefilter("ignore")
                    orilib.call(command, *args)
            except Exception as e:  # noqa: BLE001
                failed.append(f"{command}: {e}")
        self.assertEqual(failed, [])


if __name__ == "__main__":
    unittest.main()
