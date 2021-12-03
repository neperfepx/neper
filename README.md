<p align="right">
    <a href="https://github.com/rquey/neper/releases"><img src="https://badge.fury.io/gh/rquey%2Fneper.svg" alt="Neper version"></a>
</p>

# Neper: Polycrystal Generation and Meshing

![](imgs/home-neper.png)

Neper is a software package for polycrystal generation and meshing.

_See also Neper's companion program, [FEPX](https://fepx.info), a finite element software package for polycrystal plasticity._

Neper's primary features are:

### Generation of polycrystals from (experimental) morphological properties

  - Distributions of grain sizes, shapes, etc.
  - Individual grain data such as obtained by synchrotron X-ray diffraction (3DXRD, DCT, HEDM…): grain centroids/volumes or full polycrystal image.

  Standard approaches (regular, Voronoi or Laguerre tessellations) are also available.

### Generation of multiphase microstructures

  Phases can be defined and assigned specific fractions and morphological properties for the cells. This allows to model complex microstructures encountered in steels, titanium alloys, etc.

### Generation of multiscale microstructures

  Each cell of a primary tessellation is partitioned into a new tessellation, and the process can be repeated any number of times. This allows to model complex microstructures such as bainitic steel, lamellar Ti-6Al-4V, etc. See examples.

### Generation of periodic or semi-periodic microstructures

  The tessellations (and their meshes) can be prescribed periodic or semi-periodic conditions to avoid free-surface effects or reduce the RVE size.

### Generation of polycrystals in non-convex domains

  Once generated, tessellations can be cut by a series of geometric primitives, such as half-spaces, cylinders, elliptic cylinders or spheres, to generate non-convex domains. See examples.

### Generation of uniform orientation distributions

 Orientations can be uniformly distributed (flat ODF) for any number of orientations and any crystal symmetry. Some orientations can even be prescribed. See examples.

### Meshing into good-quality elements

  Meshing involves specific methods called regularization, multimeshing and remeshing. The mesh size can be uniform or defined on a per-grain basis. Cohesive elements can be inserted at interfaces.

### Simulation-related capabilities

  Simulation results (from FEPX) are arranged in a simulation directory, and new results can be computed.

### Visualization of the tessellations, meshes and simulation results

  The tessellation, mesh and results to visualize can be set in great detail for publication-quality rendering as PNG images. VTK files can also be generated for interactive visualization.

Neper is built around four modules: generation, meshing, simulation and visualization. All the input data are prescribed non-interactively, using command lines and / or ASCII files. Neper is written in C/C++, and can be compiled (via CMake) and run on any Unix-like system (including MacOS).
