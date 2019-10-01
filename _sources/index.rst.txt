=========================================
Neper: polycrystal generation and meshing
=========================================

.. role:: red

+-------------------------------------------------------------------+----------------------------------+
|                                                                   |                                  |
|Neper is a software package for                                    |                                  |
|polycrystal generation                                             |                                  |
|and meshing.            It can deal with 2D                        |                                  |
|and 3D polycrystals with very large numbers                        |                                  |
|of grains.  Its main features are:                                 |                                  |
|                                                                   |                                  |
|- **Generation of polycrystals from                                |.. image:: imgs/neper2.png        |
|  (experimental) morphological properties**                        |    :width: 85%                   |
|                                                                   |    :align: center                |
|  - Grain size distribution, grain shape                           |                                  |
|    distribution, etc.                                             |:download:`script <imgs/neper.sh>`|
|  - Grain-by-grain data as obtained for                            |                                  |
|    example in synchrotron X-ray diffraction                       |                                  |
|    (3DXRD, DCT, HEDM, ...)                                        |                                  |
|    and such as grain centroids/volumes or a                       |                                  |
|    full polycrystal image.                                        |                                  |
|  Standard approaches (regular                                     |                                  |
|  tessellations and Voronoi or Laguerre                            |                                  |
|  tessellations) are also available.                               |                                  |
|  See `examples                                                    |                                  |
|  <generation.html#tessellations-from-morphological-properties>`_. |                                  |
|                                                                   |                                  |
|- **Generation of multiscale                                       |                                  |
|  microstructures**                                                |                                  |
|                                                                   |                                  |
|  Each cell of a primary tessellation is                           |                                  |
|  partitioned into a new tessellation, and                         |                                  |
|  the process can be repeated any                                  |                                  |
|  number of times. This can be used to mode                        |                                  |
|  complex microstructures such as bainitic                         |                                  |
|  steel, lamellar Ti-6Al-4V, etc.                                  |                                  |
|  See `examples                                                    |                                  |
|  <generation.html#multiscale-microstructures>`_.                  |                                  |
|                                                                   |                                  |
|- **Generation of periodic or                                      |                                  |
|  semi-periodic microstructures**                                  |                                  |
|                                                                   |                                  |
|  The tessellations (and their meshes) can                         |                                  |
|  be prescribed periodic or semi-periodic                          |                                  |
|  conditions to avoid free-surface effects                         |                                  |
|  or reduce the RVE size.                                          |                                  |
|  See `examples                                                    |                                  |
|  <generation.html#periodic-and-semi-periodic-tessellations>`_.    |                                  |
|                                                                   |                                  |
|- **NEW (version 3.4+):** **Generation of polycrystals in          |                                  |
|  non-convex domains**                                             |                                  |
|                                                                   |                                  |
|  Once generated, tessellations can be cut by a series of          |                                  |
|  geometric primitives,                                            |                                  |
|  such as half-spaces, cylinders, elliptic cylinders or spheres, to|                                  |
|  generate non-convex domains.                                     |                                  |
|  See `examples <generation.html#new-non-convex-domains>`_.        |                                  |
|                                                                   |                                  |
|- **NEW (version 3.3+):** **Generation of uniform orientation      |                                  |
|  distributions**                                                  |                                  |
|                                                                   |                                  |
|  Orientations can be uniformly distributed (flat ODF) for         |                                  |
|  any number of orientations and any crystal symmetry.             |                                  |
|  Some orientations can even be prescribed.                        |                                  |
|  See `examples                                                    |                                  |
|  <generation.html#new-crystal-orientation-distribution>`_.        |                                  |
|                                                                   |                                  |
|- **Meshing into good-quality elements**                           |                                  |
|                                                                   |                                  |
|  Meshing involves specific methods                                |                                  |
|  called regularization, multimeshing                              |                                  |
|  and remeshing. The mesh                                          |                                  |
|  size can be uniform                                              |                                  |
|  or defined on a per-grain basis.                                 |                                  |
|                                                                   |                                  |
|- **Meshing with cohesive                                          |                                  |
|  elements at interfaces**                                         |                                  |
|                                                                   |                                  |
|- **Visualization and analysis of the                              |                                  |
|  tessellations and  meshes**                                      |                                  |
|                                                                   |                                  |
|  The scene can be set in great detail                             |                                  |
|  and images are generated with                                    |                                  |
|  publication-quality rendering                                    |                                  |
|  (see the image on the right).                                    |                                  |
|  Several metrics are available on the                             |                                  |
|  morphological and topological                                    |                                  |
|  properties.                                                      |                                  |
|                                                                   |                                  |
|Neper is built around three modules:                               |                                  |
|generation, meshing and visualization.                             |                                  |
|All the input data are prescribed                                  |                                  |
|non-interactively, using command lines                             |                                  |
|and / or ASCII files. Neper can be                                 |                                  |
|compiled and run on any Unix-like                                  |                                  |
|system.                                                            |                                  |
+-------------------------------------------------------------------+----------------------------------+

Contents
========

.. toctree::
   :maxdepth: 1

   generation
   meshing
   visualization
   applications
   downloads
   community
   about

Author
======

- `Romain Quey <romain.quey@mines-stetienne.fr>`_
