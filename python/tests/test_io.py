# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

"""Tests of the readers and data files (require NumPy).

The values read are compared to those computed by Neper (statistics files).
The neper executable is NEPER_EXECUTABLE, or neper in the PATH."""

import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

import neper  # noqa: E402

try:
    import numpy as np
except ImportError:  # pragma: no cover
    np = None


def _have_neper():
    try:
        neper.executable()
        return True
    except FileNotFoundError:
        return False


@unittest.skipIf(np is None, "numpy not available")
class TestFiles(unittest.TestCase):
    """Readers on files written by hand (no run)."""

    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.dir = Path(self._tmp.name)

    def tearDown(self):
        self._tmp.cleanup()

    def test_stat(self):
        f = self.dir / "a.stcell"
        f.write_text("1 0.5\n2 0.25\n")
        arr = neper.read_stat(f)
        self.assertEqual(arr.shape, (2, 2))
        self.assertEqual(arr.dtype.kind, "f")
        d = neper.read_stat(f, "id,vol")
        np.testing.assert_array_equal(d["vol"], [0.5, 0.25])
        with self.assertRaises(ValueError):
            neper.read_stat(f, "vol")
        f.write_text("1 2\n3 4\n")
        self.assertEqual(neper.read_stat(f).dtype.kind, "i")
        f.write_text("1 2 3\n4\n")  # variable number of values (e.g. faces)
        rows = neper.read_stat(f)
        self.assertEqual([len(r) for r in rows], [3, 1])
        self.assertEqual(neper._io._split_keys("vol,body(1,2),x"), ["vol", "body(1,2)", "x"])

    def test_sim(self):
        sim = self.dir / "a.sim"
        (sim / "inputs").mkdir(parents=True)
        (sim / "inputs" / "simulation.msh").write_text("")
        d = sim / "results" / "nodes" / "disp"
        d.mkdir(parents=True)
        (d / "disp.def").write_text("*type vector\n")
        for step in range(3):
            (d / f"disp.step{step}").write_text(f"{step} 0 0\n{step} 1 0\n")
        s = neper.read(sim)
        self.assertEqual(s.entities, ["nodes"])
        self.assertEqual(s.results("nodes"), ["disp"])
        self.assertEqual(s.steps, [0, 1, 2])
        self.assertEqual(s.result_type("nodes", "disp"), "vector")
        self.assertEqual(s.read("nodes", "disp", 1).tolist(), [[1, 0, 0], [1, 1, 0]])
        self.assertEqual(s.read("nodes", "disp").shape, (3, 2, 3))
        self.assertEqual(s.result_type("nodes", "coo"), "vector")  # known name
        s.write("nodes", "val", [1.5, 2.5], step=1, expr="test")
        self.assertEqual(s.read("nodes", "val", 1).tolist(), [1.5, 2.5])
        self.assertEqual((s.result_type("nodes", "val"), s.result_expr("nodes", "val")),
                         ("real", "test"))
        with self.assertRaises(ValueError):  # 2 steps of values for 3 steps
            s.write("nodes", "val2", np.zeros((2, 2)))
        self.assertEqual(s.inputs["msh"].name, "simulation.msh")

    def test_tesr(self):
        # 3 x 2 voxels (x varying first), binary data in-file, orientations in
        # a raw file (binary, double precision), oridef in ASCII
        cells = np.array([1, 1, 2, 0, 2, 2], dtype="<i2")
        ori = np.arange(18, dtype=np.float64).reshape(6, 3)
        (self.dir / "o.raw").write_bytes(ori.tobytes())
        head = ("***tesr\n **format\n   2.3\n **general\n   2\n   3 2\n   0.5 0.25\n"
                "  *origin\n   1 2\n **cell\n   2\n  *id\n   7 9\n **data\n   binary16\n")
        tail = ("\n **oridata\n   euler-bunge\n  *file o.raw\n"
                " **oridef\n   ascii\n   1 1 1 0 1 1\n***end\n")
        f = self.dir / "a.tesr"
        f.write_bytes(head.encode() + cells.tobytes() + tail.encode())
        tesr = neper.read(f)
        self.assertEqual(tesr.vox_cell.shape, (3, 2))
        self.assertEqual(tesr.vox_cell[:, 0].tolist(), [1, 1, 2])
        self.assertEqual(tesr.vox_cell[:, 1].tolist(), [0, 2, 2])
        self.assertEqual(tesr.cell_id.tolist(), [7, 9])
        self.assertEqual(tesr.origin.tolist(), [1, 2])
        self.assertEqual(tesr.vox_orides, "euler-bunge:passive")
        self.assertEqual(tesr.vox_ori.shape, (3, 2, 3))
        self.assertEqual(tesr.vox_ori[1, 0].tolist(), [3, 4, 5])  # 2nd voxel
        self.assertEqual(tesr.vox_ori[0, 1].tolist(), [9, 10, 11])  # 4th voxel
        self.assertEqual(tesr.vox_oridef[:, 1].tolist(), [False, True, True])

    def test_odf(self):
        (self.dir / "a.dat").write_text("1 2\n3\n")
        (self.dir / "a.odf").write_text(
            "***odf\n **format\n   1.0\n **space\n  rodrigues\n  *crysym\n   cubic\n"
            "  *fr\n   fr20\n  *theta\n   5\n **eltdata\n   ascii\n   *file a.dat\n"
            " **nodedata\n   ascii\n   4 5\n***end\n")
        odf = neper.read(self.dir / "a.odf")
        self.assertEqual((odf.space, odf.crysym, odf.fr, odf.theta),
                         ("rodrigues", "cubic", "fr20", 5))
        self.assertEqual(odf.elt_data.tolist(), [1, 2, 3])
        self.assertEqual(odf.node_data.tolist(), [4, 5])

    def test_ori_convert(self):
        # 30 degrees about x (see the documentation, "Orientation Convention")
        ref = {"rodrigues": [0.267949192, 0, 0], "euler-bunge": [0, 30, 0],
               "euler-kocks": [270, 30, 90], "euler-roe": [270, 30, 90],
               "rotmat": [1, 0, 0, 0, 0.866025404, 0.5, 0, -0.5, 0.866025404],
               "axis-angle": [1, 0, 0, 30], "quaternion": [0.965925826, 0.258819045, 0, 0]}
        for a in ref:
            for b in ref:
                np.testing.assert_allclose(neper.ori_convert(ref[a], a, b), ref[b], atol=1e-8)
        # active: opposite rotation
        np.testing.assert_allclose(
            neper.ori_convert(ref["rodrigues"], "rodrigues", "euler-bunge:active"),
            [180, 30, 180], atol=1e-8)
        # arrays of any shape, and round trips
        rng = np.random.default_rng(0)
        q = rng.normal(size=(5, 7, 4))
        q = q / np.linalg.norm(q, axis=-1, keepdims=True)
        q[q[..., 0] < 0] *= -1
        for des in ref:
            back = neper.ori_convert(neper.ori_convert(q, "q", des), des, "quaternion")
            np.testing.assert_allclose(back, q, atol=1e-9)
        self.assertAlmostEqual(neper.ori_angle([0, 0, 0], ref["rodrigues"], "rodrigues"), 30,
                               places=6)
        with self.assertRaises(ValueError):
            neper.ori_convert([1, 2, 3], "foo", "rodrigues")
        with self.assertRaises(ValueError):
            neper.ori_convert([1, 2], "rodrigues", "quaternion")

    def test_orides(self):
        self.assertEqual(neper._io._orides("e"), "euler-bunge:passive")
        self.assertEqual(neper._io._orides("rodrigues", "all"), "rodrigues:active")
        self.assertEqual(neper._io._orides("rodrigues", "explicit"), "rodrigues:passive")
        self.assertEqual(neper._io._orides("q:active", "explicit"), "quaternion:passive")

    def test_datafile(self):
        df = neper.datafile([[1, 2.5], [3, 4]])
        self.assertTrue(str(df).startswith("file(") and str(df).endswith(")"))
        self.assertEqual(df.path.read_text(), "1 2.5\n3 4\n")
        df = neper.datafile(np.array([0.1, 0.2]), path=self.dir / "d.txt")
        self.assertEqual(df.path, (self.dir / "d.txt").resolve())
        self.assertEqual(df.path.read_text(), "0.1\n0.2\n")
        self.assertEqual(neper.datafile(["red", "blue"]).path.read_text(), "red\nblue\n")
        # 2D arrays are passed as data files, 1D arrays as lists
        self.assertTrue(neper._core._value(np.zeros((2, 3))).startswith("file("))
        self.assertTrue(neper._core._value([[1, 2], [3, 4]]).startswith("file("))
        self.assertEqual(neper._core._value(np.array([1, 2])), "1,2")
        self.assertEqual(neper._core._value(df), f"file({df.path})")


@unittest.skipIf(np is None, "numpy not available")
@unittest.skipUnless(_have_neper(), "neper executable not found")
class TestRead(unittest.TestCase):
    """Readers on files written by Neper, compared to Neper's statistics."""

    @classmethod
    def setUpClass(cls):
        cls._tmp = tempfile.TemporaryDirectory()
        cls.dir = Path(cls._tmp.name)
        cls._saved = neper.get_defaults()
        neper.set_defaults(rcfile="none", threads=2)

    @classmethod
    def tearDownClass(cls):
        neper.set_defaults(**cls._saved)
        cls._tmp.cleanup()

    def T(self, **kw):
        return neper.T(cwd=self.dir, **kw)

    def M(self, *args, **kw):
        return neper.M(*args, cwd=self.dir, **kw)

    def test_tess(self):
        res = self.T(n=10, id=1, o="t", statseed="x,y,z,w", statver="x,y,z",
                     statpoly="faces", statface="vers,edges", statcell="vol")
        tess = res.read("tess")
        self.assertIsInstance(tess, neper.Tess)
        self.assertEqual((tess.dim, tess.cell_qty, tess.poly_qty), (3, 10, 10))
        seeds = neper.read(res.file("stseed"))
        np.testing.assert_allclose(tess.seed_coo, seeds[:, :3], atol=1e-9)
        np.testing.assert_allclose(tess.seed_weight, seeds[:, 3], atol=1e-9)
        np.testing.assert_allclose(tess.ver_coo, neper.read(res.file("stver")), atol=1e-9)
        polys = neper.read_stat(res.file("stpoly"))
        for i in range(tess.poly_qty):
            self.assertEqual(sorted(np.abs(tess.poly_faces[i])), sorted(polys[i]))
        faces = neper.read_stat(res.file("stface"))
        for i in range(tess.face_qty):
            nv = len(tess.face_vers[i])
            self.assertEqual(list(tess.face_vers[i]), list(faces[i][:nv]))
            self.assertEqual(list(np.abs(tess.face_edges[i])), list(faces[i][nv:]))
        self.assertEqual(tess.dom_type, "cube")
        self.assertEqual(sorted(tess.dom_face_label), ["x0", "x1", "y0", "y1", "z0", "z1"])
        self.assertEqual(tess.cell_ori.shape, (10, 3))
        self.assertEqual(tess.cell_orides, "rodrigues:passive")
        # face equations: the vertices are on the faces
        for i in range(tess.face_qty):
            d, n = tess.face_eq[i, 0], tess.face_eq[i, 1:]
            coo = tess.ver_coo[tess.face_vers[i] - 1]
            np.testing.assert_allclose(coo @ n, d, atol=1e-6)

    def test_tess_2d_periodic_multiscale(self):
        tess = self.T(n=5, dim=2, o="t2").read()
        self.assertEqual((tess.dim, tess.face_qty, tess.poly_faces), (2, 5, None))
        tess = self.T(n=5, periodicity=1, o="tp").read()
        self.assertEqual(tess.type, "periodic")
        self.assertTrue(tess.periodic.all())
        self.assertEqual(tess.per_seed.shape[1], 5)
        self.assertEqual(tess.per_face.shape[1], 6)
        tess = self.T(n="2::3", o="tm").read()
        self.assertEqual((tess.cell_qty, tess.scale_qty), (6, 2))
        self.assertEqual(tess.cell_scaleid[:, 0].tolist(), [1, 1, 1, 2, 2, 2])

    def test_tesr(self):
        res = self.T(n=5, format="tesr", tesrsize="8:6:4", statcell="x,y,z,vol", o="r")
        tesr = res.read("tesr")
        self.assertEqual(tesr.vox_cell.shape, (8, 6, 4))
        stat = neper.read(res.file("stcell"))
        # cell centroids and volumes, from the voxels ([x, y, z] indexing)
        idx = np.indices(tesr.vox_cell.shape).reshape(3, -1).T
        coo = tesr.origin + (idx + 0.5) * tesr.vsize
        cells = tesr.vox_cell.reshape(-1)
        for c in range(1, tesr.cell_qty + 1):
            np.testing.assert_allclose(coo[cells == c].mean(axis=0), stat[c - 1, :3],
                                       atol=1e-9)
            self.assertAlmostEqual((cells == c).sum() * np.prod(tesr.vsize),
                                   stat[c - 1, 3], places=9)
        # ASCII format
        tesr2 = self.T(n=5, format="tesr", tesrsize="8:6:4", tesrformat="ascii",
                       o="r2").read()
        np.testing.assert_array_equal(tesr.vox_cell, tesr2.vox_cell)
        # 2D
        tesr3 = self.T(n=5, dim=2, format="tesr", tesrsize="7:5", o="r3").read()
        self.assertEqual(tesr3.vox_cell.shape, (7, 5))
        self.assertEqual(set(np.unique(tesr3.vox_cell)), {1, 2, 3, 4, 5})

    def test_mesh(self):
        tess = self.T(n=5, o="m").file("tess")
        res = self.M(tess, elttype="tet", order=1, statelt="x,y,z,vol", statnode="x,y,z",
                     nset="faces", faset="faces", o="m")
        mesh = res.read("msh")
        self.assertEqual(mesh.dim, 3)
        np.testing.assert_allclose(mesh.node_coo, neper.read(res.file("stnode")), atol=1e-9)
        tets = mesh.elts()
        self.assertEqual(tets.type, "tet")
        stat = neper.read(res.file("stelt"))
        self.assertEqual(len(stat), tets.elt_qty)
        coo = mesh.node_coo[tets.elt_nodes - 1]  # (n, 4, 3)
        np.testing.assert_allclose(coo.mean(axis=1), stat[:, :3], atol=1e-9)
        vol = np.abs(np.linalg.det(coo[:, 1:] - coo[:, :1])) / 6
        np.testing.assert_allclose(vol, stat[:, 3], atol=1e-9)
        self.assertEqual(set(tets.elt_elset), set(range(1, 6)))
        self.assertEqual(mesh.elset_ori.shape, (5, 3))
        self.assertEqual(sorted(mesh.nsets), ["x0", "x1", "y0", "y1", "z0", "z1"])
        np.testing.assert_allclose(mesh.node_coo[mesh.nsets["x0"] - 1, 0], 0, atol=1e-12)
        self.assertEqual(sorted(mesh.fasets), ["x0", "x1", "y0", "y1", "z0", "z1"])
        self.assertEqual(mesh.elts(2).type, "tri")
        # binary format
        mesh2 = self.M(tess, elttype="tet", order=1, nset="faces", faset="faces",
                       format="msh:binary", o="mb").read()
        self.assertTrue(mesh2.binary)
        np.testing.assert_allclose(mesh.node_coo, mesh2.node_coo, atol=1e-11)  # (ASCII: 12 digits)
        self.assertEqual(len(mesh.blocks), len(mesh2.blocks))
        for b1, b2 in zip(mesh.blocks, mesh2.blocks):
            for key in ("elt_id", "elt_nodes", "elt_elset", "elt_part"):
                np.testing.assert_array_equal(getattr(b1, key), getattr(b2, key))
        self.assertEqual(mesh.nsets.keys(), mesh2.nsets.keys())

    def test_mesh_hex_2d(self):
        tess = self.T(n=3, dim=2, o="q").file("tess")
        mesh = self.M(tess, elttype="quad", order=2, o="q").read()
        self.assertEqual(mesh.dim, 2)
        quads = mesh.elts()
        self.assertEqual((quads.type, quads.order, quads.elt_nodes.shape[1]), ("quad", 2, 8))

    def test_result_stat(self):
        res = self.T(n=10, statcell="vol,x", statseed="w", o="st")
        stat = res.stat("cell")
        self.assertEqual(list(stat), ["vol", "x"])
        self.assertAlmostEqual(stat["vol"].sum(), 1, places=9)
        self.assertEqual(len(res.stat("seed")["w"]), 10)
        with self.assertRaises(LookupError):
            res.stat("ver")

    def test_sim(self):
        # simulation directory made by hand: a mesh and node positions
        tess = self.T(n=5, o="s").file("tess")
        mesh = self.M(tess, elttype="tet", order=1, o="s").read()
        sim = self.dir / "s.sim"
        (sim / "inputs").mkdir(parents=True)
        (sim / "inputs" / "simulation.msh").write_bytes(mesh.path.read_bytes())
        s = neper.read_sim(sim)
        coo = np.stack([mesh.node_coo * [1, 1, 1 + 0.01 * i] for i in range(3)])
        s.write("nodes", "coo", coo)
        self.assertEqual(s.steps, [0, 1, 2])
        self.assertEqual(s.result_type("nodes", "coo"), "vector")
        self.assertEqual(s.entity_qty("nodes"), mesh.node_qty)
        with self.assertRaises(ValueError):  # wrong number of nodes
            s.write("nodes", "bad", np.zeros((3, 4)), step=0)
        # results computed by module -S from Python results
        elts = mesh.elts()
        s.write("elts", "grain", np.stack([elts.elt_elset] * 3))  # (all steps, for -S)
        self.assertEqual(s.result_type("elts", "grain"), "int")
        neper.S(sim, resnode="disp", reselt="twice:2*grain", cwd=self.dir)
        s.refresh()
        disp = s.read("nodes", "disp")
        self.assertEqual(disp.shape, (3, mesh.node_qty, 3))
        np.testing.assert_allclose(disp[2, :, 2], 0.02 * mesh.node_coo[:, 2], atol=1e-9)
        np.testing.assert_array_equal(s.read("elts", "twice", 0), 2 * elts.elt_elset)
        self.assertIn("disp", s.read_all(["nodes"])["nodes"])
        s.remove("elts", "twice")
        self.assertNotIn("twice", s.results("elts"))
        # visualization of a Python result
        img = neper.V(sim, dataeltcol="int:grain", imagesize="100:100", print="img",
                      cwd=self.dir)
        self.assertEqual(img.file().name, "img.png")

    def test_v_scale(self):
        tess = self.T(n=5, o="v").read()
        res = neper.V(self.dir / "v.tess", cwd=self.dir, imagesize="100:100",
                      datacellcol=f"real:{neper.datafile(tess.seed_coo[:, 2])}", print="img")
        self.assertEqual(res.file("png").name, "img.png")  # not the scale image
        self.assertIn("img-scale3.png", [f.name for f in res.files])
        self.assertEqual(res._repr_png_()[:8], b"\x89PNG\r\n\x1a\n")
        self.assertIsNone(self.T(n=2, o="nopng")._repr_png_())

    def test_ori_convert_neper(self):
        # conversions compared to those of Neper (-oridescriptor)
        ori = self.T(n=50, o="or").read().cell_ori
        for des in ("euler-bunge", "euler-kocks:active", "rotmat", "axis-angle", "quaternion"):
            tess = self.T(n=50, oridescriptor=des, o="or2").read()
            np.testing.assert_allclose(neper.ori_convert(ori, "rodrigues", tess.cell_orides),
                                       tess.cell_ori, atol=1e-6)

    def test_datafile_input(self):
        oris = np.array([[0.1, 0.2, 0.3], [0, 0, 0], [-0.1, 0.05, 0.2]])
        tess = self.T(n=3, ori=oris, o="o").read()
        np.testing.assert_allclose(tess.cell_ori, oris, atol=1e-9)
        seeds = np.array([[0.2, 0.2, 0.2], [0.8, 0.8, 0.8], [0.5, 0.1, 0.9]])
        tess = self.T(n=3, morphooptiini=f"coo:{neper.datafile(seeds)}", o="s").read()
        np.testing.assert_allclose(tess.seed_coo, seeds, atol=1e-9)


if __name__ == "__main__":
    unittest.main()
