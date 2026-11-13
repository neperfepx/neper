# Neper - Polycrystal Generation, Meshing and Post-Processing

## Overview

Neper is a software package for polycrystal generation, meshing and post-processing, written in
C and C++ with OpenMP. It deals with 2D and 3D polycrystals with very large numbers of grains.
Its primary features are:

- Generation of polycrystals as tessellations: Voronoi and Laguerre tessellations,
  tessellations from (experimental) morphological properties, such as cell size and sphericity
  distributions, and regular tessellations
- Generation of multiscale microstructures (e.g. lamellar or multiphase), of periodic or
  semi-periodic microstructures, and of polycrystals in non-convex or curved domains
- Generation of crystal orientations, including uniform orientation distributions
- Regularization of the tessellations (removal of the small edges) for high-quality meshing
- Meshing into tetrahedral or triangular elements, conformal hexahedral or quadrangular
  elements, or regular (grid) elements, with cohesive elements at interfaces if needed, and
  output to Gmsh, Abaqus, Z-set, VTK and FEPX formats
- Post-processing of simulation results (from FEPX), into new results and statistics
- Visualization of tessellations, meshes and simulation results as publication-quality images,
  pole figures and inverse pole figures
- A Python interface, which runs Neper and reads its files into NumPy arrays

Neper is built around four modules: tessellation (-T), meshing (-M), simulation post-processing
(-S) and visualization (-V). All input data are prescribed non-interactively, using command
lines and / or ASCII files, so that all treatments can be automated. Neper works with
[FEPX](https://fepx.info), whose meshes it generates and whose results it post-processes.

## Installation

Neper is available as a conda package from [conda-forge](https://conda-forge.org), for Linux and
macOS:

```
conda create -n neper -c conda-forge neper
```

Neper can also be built from source, which requires a C and C++ compiler, CMake and the Gmsh
library (4.9 or later), and preferably OpenMP; the images require POV-Ray (and, for pole
figures, Asymptote):

```
cd src
mkdir build && cd build
cmake ..
make -j
ctest
make userinstall                                       # or sudo make install
```

The installation instructions, including the build options, are in the
[documentation](https://neper.info/doc/installation.html) (`doc/installation.rst`).

## Documentation

The documentation, with a quick start and tutorials, is available at
<https://neper.info/doc/index.html> and in `doc/`.

## License

GNU General Public License, version 3, see [`COPYING`](COPYING); the documentation is under the
GNU Free Documentation License.
