# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

"""Tests of the Python interface (run with python -m unittest or pytest).

The neper executable is NEPER_EXECUTABLE, or neper in the PATH."""

import os
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

import neper  # noqa: E402
from neper import _core  # noqa: E402


class TestArguments(unittest.TestCase):
    """Conversion of Python arguments to command-line arguments (no run)."""

    def test_values(self):
        self.assertEqual(_core._value(True), "1")
        self.assertEqual(_core._value(False), "0")
        self.assertEqual(_core._value(10), "10")
        self.assertEqual(_core._value(0.5), "0.5")
        self.assertEqual(_core._value(["tess", "vtk"]), "tess,vtk")
        self.assertEqual(_core._value((1, 2.5)), "1,2.5")
        self.assertEqual(_core._value(Path("a/b.tess")), "a/b.tess")

    def test_options(self):
        self.assertEqual(
            _core._options({"n": 10, "id": 1, "reg": True, "o": None, "in_": "x"}),
            ["-n", "10", "-id", "1", "-reg", "1", "-in", "x"])

    def test_numpy(self):
        try:
            import numpy as np
        except ImportError:
            self.skipTest("numpy not available")
        self.assertEqual(_core._value(np.int64(3)), "3")
        self.assertEqual(_core._value(np.array([1, 2])), "1,2")
        self.assertEqual(_core._value(np.bool_(True)), "1")
        self.assertEqual(_core._value(np.float64(0.5)), "0.5")

    def test_errors(self):
        out = ("Info   : Reading input data...\n"
               "Error  :   - Unknown argument `foo'.\n")
        self.assertEqual(_core._error_message(out, -6), "Unknown argument `foo'.")
        out = ("Error  : Unknown option `-foo'.\n"
               "Error  : Bad arguments!\n\n")
        self.assertEqual(_core._error_message(out, -6),
                         "Unknown option `-foo'. Bad arguments!")
        out = ("Error  : A long message\n"
               "         on two lines.\n"
               "Info   : x\n")
        self.assertEqual(_core._error_message(out, 1), "A long message on two lines.")
        self.assertTrue(_core._error_message("", -6).startswith("neper terminated by SIGABRT"))

    def test_written_files(self):
        out = ("Info   :     [o] Writing file `a.tess'...\n"
               "Info   :     [o] Wrote file `a.tess'.\n"
               "Info   :     [o] Wrote file `b.msh'.\n")
        self.assertEqual(_core._written_files(out), ["a.tess", "b.msh"])

    def test_defaults(self):
        saved = neper.get_defaults()
        try:
            neper.set_defaults(verbose=True)
            self.assertTrue(neper.get_defaults()["verbose"])
            with self.assertRaises(TypeError):
                neper.set_defaults(foo=1)
        finally:
            neper.set_defaults(**saved)


def _have_neper():
    try:
        neper.executable()
        return True
    except FileNotFoundError:
        return False


@unittest.skipUnless(_have_neper(), "neper executable not found")
class TestRun(unittest.TestCase):
    """Runs of the neper executable."""

    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.dir = Path(self._tmp.name)
        self._saved = neper.get_defaults()
        neper.set_defaults(rcfile="none", threads=2)

    def tearDown(self):
        neper.set_defaults(**self._saved)
        self._tmp.cleanup()

    def test_version(self):
        self.assertRegex(neper.version(), r"^\d+\.\d+\.\d+")
        self.assertTrue(os.path.isdir(neper.data_dir()))

    def test_t(self):
        res = neper.T(n=10, id=1, format=["tess", "tesr"], tesrsize=10,
                      statcell="vol", cwd=self.dir)
        self.assertTrue(res.ok)
        self.assertEqual([f.name for f in res.files],
                         ["n10-id1.tess", "n10-id1.tesr", "n10-id1.stcell"])
        self.assertTrue(res.file("tess").is_file())
        self.assertEqual(res.file(".tess"), self.dir / "n10-id1.tess")
        with self.assertRaises(LookupError):
            res.file("msh")
        vols = [float(x) for x in res.file("stcell").read_text().split()]
        self.assertEqual(len(vols), 10)
        self.assertAlmostEqual(sum(vols), 1, places=6)

    def test_t_m(self):
        tess = neper.T(n=5, o="poly", cwd=self.dir).file("tess")
        res = neper.M(tess, elttype="hex", cwd=self.dir)
        self.assertEqual(res.file(), self.dir / "poly.msh")
        self.assertIn("$Elements", res.file().read_text())

    def test_run(self):
        res = neper.run("-T -n 3 -o ab", cwd=self.dir)
        self.assertEqual(res.file().name, "ab.tess")
        res = neper.run(["neper", "-T", "-n", 3, "-o", "c"], cwd=self.dir)
        self.assertEqual(res.file().name, "c.tess")

    def test_error(self):
        with self.assertRaises(neper.NeperError) as cm:
            neper.T(n=10, morpho="foo", cwd=self.dir)
        err = cm.exception
        self.assertIn("foo", err.message)
        self.assertNotEqual(err.returncode, 0)
        self.assertIn("Error", err.output)

        res = neper.T(n=10, morpho="foo", cwd=self.dir, check=False)
        self.assertFalse(res.ok)

    def test_unknown_option(self):
        # (requires the error message to be flushed before Neper aborts)
        with self.assertRaises(neper.NeperError) as cm:
            neper.T(n=10, foobar=1, cwd=self.dir)
        self.assertIn("-foobar", cm.exception.message)

    def test_missing_file(self):
        with self.assertRaises(neper.NeperError) as cm:
            neper.M("missing.tess", cwd=self.dir)
        self.assertIn("missing.tess", cm.exception.message)

    def test_rcfile(self):
        rc = self.dir / "rc"
        rc.write_text("neper -T -o fromrc\n")
        res = neper.T(n=3, rcfile=rc, cwd=self.dir)
        self.assertEqual(res.file().name, "fromrc.tess")

    def test_threads(self):
        res = neper.T(n=3, threads=1, cwd=self.dir)
        self.assertIn("Running on 1 thread", res.output)

    def test_executable(self):
        with self.assertRaises(FileNotFoundError):
            neper.T(n=3, cwd=self.dir, executable=self.dir / "nonexistent")


if __name__ == "__main__":
    unittest.main()
