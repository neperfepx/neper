=========================================
Neper: Polycrystal Generation and Meshing
=========================================

.. role:: red

+-------------------------------------------------------------------------+------------------------------------------------+
|                                                                         |                                                |
|Neper is a software package for                                          |                                                |
|polycrystal generation                                                   |                                                |
|and meshing.            It can deal with 2D                              |                                                |
|and 3D polycrystals with very large numbers                              |                                                |
|of grains.  Its primary features are:                                    |                                                |
|                                                                         |                                                |
|- **Generation of polycrystals from                                      |.. image:: imgs/neper3.png                      |
|  (experimental) morphological properties**                              |    :width: 92%                                 |
|                                                                         |    :align: right                               |
|  - Distributions of grain sizes, shapes, etc.                           |                                                |
|  - Individual grain data such as obtained by                            |:download:`script <imgs/neper.sh>`              |
|    synchrotron X-ray diffraction (3DXRD, DCT, HEDM...):                 |                                                |
|    grain centroids/volumes or full polycrystal image.                   |                                                |
|                                                                         |                                                |
|  Standard approaches (regular,                                          |                                                |
|  Voronoi or Laguerre                                                    |                                                |
|  tessellations) are also available.                                     |                                                |
|                                                                         |                                                |
|- **[NEW]** **Generation of multiphase microstructures**                 |**[NEW] Neper now has a companion program,**    |
|                                                                         |`FEPX <http://fepx.info>`_.                     |
|                                                                         |                                                |
|                                                                         |Neper now works hand in hand with `FEPX,        |
|                                                                         |a new free / open-source finite element         |
|  Phases can be defined and assigned specific fractions and              |software package for polycrystal plasticity     |
|  morphological properties for the cells. This allows                    |<http://fepx.info>`_. Neper's output meshes can |
|  to model complex microstructures encountered in                        |be readily used by FEPX for simulation, and     |
|  steels, titanium alloys, etc.                                          |FEPX's output results can be processed by and   |
|                                                                         |visualized using Neper.                         |
|- **Generation of multiscale microstructures**                           |                                                |
|                                                                         |                                                |
|  Each cell of a primary tessellation is                                 |                                                |
|  partitioned into a new tessellation, and                               |                                                |
|  the process can be repeated any                                        |                                                |
|  number of times. This allows to model                                  |                                                |
|  complex microstructures such as bainitic                               |                                                |
|  steel, lamellar Ti-6Al-4V, etc.                                        |                                                |
|  See `examples                                                          |                                                |
|  <generation.html#multiscale-microstructures>`_.                        |                                                |
|                                                                         |                                                |
|- **Generation of periodic or                                            |                                                |
|  semi-periodic microstructures**                                        |                                                |
|                                                                         |                                                |
|  The tessellations (and their meshes) can                               |                                                |
|  be prescribed periodic or semi-periodic                                |                                                |
|  conditions to avoid free-surface effects                               |                                                |
|  or reduce the RVE size.                                                |                                                |
|                                                                         |                                                |
|- **Generation of polycrystals in                                        |                                                |
|  non-convex domains**                                                   |                                                |
|                                                                         |                                                |
|  Once generated, tessellations can be cut by a series of                |                                                |
|  geometric primitives,                                                  |                                                |
|  such as half-spaces, cylinders, elliptic cylinders or                  |                                                |
|  spheres, to generate non-convex domains.                               |                                                |
|  See `examples <generation.html#new-non-convex-domains>`_.              |                                                |
|                                                                         |                                                |
|- **Generation of uniform orientation                                    |                                                |
|  distributions**                                                        |                                                |
|                                                                         |                                                |
|  Orientations can be uniformly distributed (flat ODF) for               |                                                |
|  any number of orientations and any crystal symmetry.                   |                                                |
|  Some orientations can even be prescribed.                              |                                                |
|  See `examples                                                          |                                                |
|  <generation.html#new-crystal-orientation-distribution>`_.              |                                                |
|                                                                         |                                                |
|- **Meshing into good-quality elements**                                 |                                                |
|                                                                         |                                                |
|  Meshing involves specific methods called regularization, multimeshing  |                                                |
|  and remeshing. The mesh size can be uniform or defined on a            |                                                |
|  per-grain basis. Cohesive elements can be inserted at                  |                                                |
|  interfaces.                                                            |                                                |
|                                                                         |                                                |
|- **[NEW]** **Simulation-related capabilities**                          |                                                |
|                                                                         |                                                |
|  Simulation results (from FEPX) are arranged in a simulation            |                                                |
|  directory, and new results can be computed.                            |                                                |
|                                                                         |                                                |
|- **Visualization of the                                                 |                                                |
|  tessellations, meshes and simulation results**                         |                                                |
|                                                                         |                                                |
|  The tessellation, mesh and results to visualize can be set             |                                                |
|  in great detail for publication-quality rendering as                   |                                                |
|  PNG images.                                                            |                                                |
|  VTK files can also be generated for interactive visualization.         |                                                |
|                                                                         |                                                |
|Neper is built around four modules:                                      |                                                |
|generation, meshing, simulation and visualization.                       |                                                |
|All the input data are prescribed |non-interactively, using command      |                                                |
|lines and / or ASCII files. Neper is written in C/C++, and can be        |                                                |
|compiled (via CMake) and run on any Unix-like system (including MacOS).  |                                                |
|                                                                         |                                                |
+-------------------------------------------------------------------------+------------------------------------------------+

Contents
========

.. toctree::
   :maxdepth: 1

   generation
   meshing
   simulation
   visualization
   applications
   downloads
   community
   about

Author
======

- `Romain Quey <romain.quey@mines-stetienne.fr>`_
