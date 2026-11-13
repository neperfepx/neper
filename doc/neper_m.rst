.. |translate| replace:: :data:`translate(<dist_x>,<dist_y>,<dist_z>)`: translate by distances :data:`<dist_x>`, :data:`<dist_y>` and :data:`<dist_z>` along :data:`x`, :data:`y` and :data:`z`, respectively
.. |rotate| replace:: :data:`rotate(<axis_x>,<axis_y>,<axis_z>,<angle>)`: rotate about the center, by a given axis and angle (angle expressed in degrees)
.. |scale| replace:: :data:`scale(<fact_x>,<fact_y>,<fact_z>)`: scale by :data:`<fact_x>`, :data:`<fact_y>` and :data:`<fact_z>` along :data:`x`, :data:`y` and :data:`z`, respectively

.. index::
   single: -M

.. _neper_m:

Meshing Module (-M)
===================

Module -M is the module for meshing scalar and raster tessellations.  Two meshing strategies are available. `Free (or unstructured) meshing` creates a conforming mesh into tetrahedral or hexahedral elements (triangular or quadrangular in 2D).  `Grid (or structured) meshing` generates a non-conforming mesh into hexahedral elements arranged on a grid (quadrangular in 2D). Free meshing is carried out so that the elements have sizes as close as possible to the target value, and show high quality, that is, with shapes as close to equilateral as possible.  The input file is a :ref:`tess_file` or a :ref:`tesr_file`, as provided by :ref:`neper_t`.  Standard tessellations, multiscale tessellations, and periodic (or semi-periodic) tessellations are supported.  Free meshing of raster tessellations works for 2D tessellations only.  The output mesh can be written as a :ref:`mesh_file`, which can be readily used by FEPX, or in other formats.

The target element size of the mesh can be specified through the element *characteristic length* (:data:`cl`).  It corresponds to the length of a 1D element, the length of the edge of a triangular or quadrangular element (2D), and the length of an edge of a tetrahedral or hexahedral element (3D). A *relative characteristic length* (:data:`rcl`) can be used instead, whose value is relative to the average cell size and provides a medium number of elements.  It is also possible to specify :data:`cl` or :data:`rcl` values on a per-cell basis, or different values along the three space directions.

For free meshing, mesh quality is ensured to the greatest extent possible using several approaches:

- Optimized meshing rules: the mesh properties are controlled by size parameters (options :option:`-cl`, :data:`-rcl`, etc.) and size gradient parameters (options :option:`-pl` and :option:`-pl{edge,face,poly}`);

- Multimeshing: each tessellation face and volume is meshed separately from the others, with several meshing algorithms, until a target mesh quality is reached.  This is controlled by options :data:`-meshqual*`, :option:`-mesh2dalgo` and :option:`-mesh3dalgo`.

Note that, in general, tessellation *regularization* is also necessary to ensure good-quality meshing (see :ref:`neper_t`, option :option:`-reg`).

Remeshing can also be applied to generate a new, good-quality mesh from a mesh containing poor-quality elements.  The variables defined on the parent mesh can be transported to the child mesh (options :data:`-transport*`).

For mapped meshing, mesh cleaning options enable the merging of duplicate nodes, or the duplication of nodes that would create a singular behavior in simulations (options :option:`-dupnodemerge` and :option:`-singnodedup`).

Mesh partitioning allows for the division of the mesh nodes and elements into several sets while minimizing the interfaces between them [#partitioning]_, as needed for parallel simulations with FEPX.  Partitioning can return any number of partitions (option :option:`-part`).

In the output mesh, the individual entities of the tessellations (the vertices, edges, faces and polyhedra) can be described by element sets (option :option:`-dim`).  Node sets of the vertices, edges and faces located on the boundary of the tessellation are also provided for prescribing the boundary conditions (option :option:`-nset`).  The surface element sets are also provided (option :option:`-faset`).  Element sets other than those corresponding to the tessellation cells can be defined (option :option:`-elset`). The mesh order can be 1 or 2 (option :option:`-order`, default :data:`2`).  Statistical data can be obtained on the meshes (options :data:`-stat*`).

The methods implemented for meshing are described in [CMAME2011]_.

Here is what a typical run of module -M looks like:

.. code-block:: console

  $ neper -M n10-id1.tess

  ========================    N   e   p   e   r    =======================
  Info   : A software package for polycrystal generation and meshing.
  Info   : Version 5.0.1
  Info   : Built with: gsl|libgmsh|muparser|opengjk|openmp|libscotch
           (full)
  Info   : Running on 8 threads.
  Info   : <https://neper.info>
  Info   : Copyright (C) 2003-2026, and GNU GPL'd, by Romain Quey, CNRS.
  Info   : No initialization file found (`/home/rquey/.neperrc').
  Info   : ---------------------------------------------------------------
  Info   : MODULE  -M loaded with arguments:
  Info   : [ini file] (none)
  Info   : [com line] n10-id1.tess
  Info   : ---------------------------------------------------------------
  Info   : Reading input data...
  Info   :   - Reading arguments...
  Info   : Loading input data...
  Info   :   - Loading tessellation...
  Info   :     [i] Parsing file `n10-id1.tess'...
  Info   :     [i] Parsed file `n10-id1.tess'.
  Info   : Meshing...
  Info   :   - Preparing... (cl = 0.2321) 100%
  Info   :   - 0D meshing... 100%
  Info   :   - 1D meshing... 100%
  Info   :   - 2D meshing... 100% (0.76|0.93/96%| 4%| 0%)
  Info   :   - Fixing 2D-mesh pinches...
  Info   :   - 3D meshing... 100% (0.92|0.94/100%| 0%| 0%)
  Info   : Creating fasets...
  Info   : Switching mesh to order 2...
  Info   : Creating nsets...
  Info   : Writing mesh results...
  Info   :   - Preparing mesh...
  Info   :   - Mesh properties:
  Info   :     > Node number:     1264
  Info   :     > Elt  number:      716
  Info   :     > Mesh volume:    1.000
  Info   :   - Writing mesh...
  Info   :     [o] Writing file `n10-id1.msh'...
  Info   :     [o] Wrote file `n10-id1.msh'.
  Info   : Elapsed time: 0.211 secs.
  ========================================================================

Input Data
----------

In normal use, the input data is a tessellation file, a raster tessellation file or a mesh file.

.. option:: <file_name1>[:<transformation>],<file_name2>...

  Specify the input file, which can be:

  - a :ref:`tess_file`;

  - a :ref:`tesr_file`;

  - a :ref:`mesh_file`, for remeshing (in which case a tessellation file should preferably be loaded too), with an optional transformation:

    - :data:`<file_name>`: nodal coordinates to load from a :ref:`position_file`.

  .. note:: To transform a raster tessellation (crop, etc.), use :ref:`neper_t`'s option :option:`-transform`.

  **Default value**: -.

It is also possible to load a mesh to be considered as the output mesh (in contrast to loading it as an input file).  If the file contains meshes of dimensions lower than the tessellation's dimension, they are used, and only higher-dimension meshes are computed. Use option :option:`-o` to avoid overwriting the file.

.. option:: -loadmesh <file_name>

  Load a mesh from a :ref:`mesh_file`.

  If the tessellation is not loaded, the node sets of the mesh file are kept (instead of being created, see :option:`-nset`), and the missing edge and vertex node sets, if any, are obtained from the intersections of the face node sets (in 3D) or edge node sets (in 2D).

  **Default value**: -.

Finally, it is possible to load a set of points.

.. option:: -loadpoint <file_name>

  Load points from a file formatted as a :ref:`position_file`.

  .. important::

    These points are used only for statistics, in option :option:`-statpoint`, and are *not* used for tessellation.

  **Default value**: -.

Meshing Options
---------------

.. option:: -elttype <elt_type>

  Specify the type of elements, which can be:

  - :data:`tri`: triangular elements;
  - :data:`quad`: quadrangular elements;
  - :data:`tet`: tetrahedral elements;
  - :data:`hex`: hexahedral elements;
  - :data:`quadgrid`: quadrangular elements, arranged on a grid;
  - :data:`quad9grid`: 9-node quadrangular elements, arranged on a grid (requires :option:`-order` :data:`2`);
  - :data:`hexgrid`: hexahedral elements, arranged on a grid.

  In 2D, :data:`tet`, :data:`hex` and :data:`hexgrid` are treated as :data:`tri`, :data:`quad` and :data:`quadgrid`, respectively.

  :data:`tri`, :data:`quad`, :data:`tet` and :data:`hex` provide meshes that conform to the cell boundaries, while :data:`quadgrid`, :data:`quad9grid` and :data:`hexgrid` provide grid (structured) meshes, which do not.

  :data:`hex` is obtained by meshing into tetrahedral elements and then splitting each tetrahedral element into 4 hexahedral elements, from its edge centres, face centres and centre (as done by Gmsh, with :data:`Mesh.SubdivisionAlgorithm = 2`).  Similarly, each triangular element is split into 3 quadrangular elements and each 1D element into 2 1D elements, so that the mesh is conformal.  The element quality is lower than the one of the tetrahedral elements (a regular tetrahedral element provides hexahedral elements of scaled Jacobian 0.71).  In 2D, :data:`quad` proceeds as Gmsh's full-quad recombination: each face is meshed into triangular elements of twice the element size, which are recombined into quadrangular elements (Blossom algorithm), and all elements are then split (each quadrangular element into 4 and each triangular element into 3) and smoothed.  The element size options (such as :option:`-cl` and :data:`-rcl`) apply to the resulting elements (the element length being the mean edge length), and :data:`-rcl` is defined as for :data:`quadgrid` and :data:`hexgrid`.  :option:`-interface` is not available with :data:`quad` and :data:`hex`.

  Raster tessellations can be meshed with :data:`quadgrid`, :data:`quad9grid` and :data:`hexgrid`, and, in 2D, with :data:`tri` and :data:`quad`.

  **Default value**: :data:`tet` in 3D and :data:`tri` in 2D.

.. index::
   single: -cl

.. index::
   single: -rcl

.. option:: -cl or -rcl <length>

  Specify the absolute or relative characteristic lengths of the elements.  :data:`-rcl` is defined relative to the average cell size.  The value applies to all entities (polyhedra, faces, edges and vertices), unless specified otherwise for some of them using :data:`-[r]cl{poly,face,edge,ver}`.  The argument can be:

  - a real value;
  - an expression based on the :ref:`tessellation_keys` or :ref:`raster_tessellation_keys`, which allows one to define individual values. An example is :data:`-rcl "(body>0)?\<val1\>:\<val2\>"`, to mesh interior cells with :data:`rcl=\<val1\>` and boundary cells with :data:`rcl=\<val2\>`;
  - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  **Default value**: :data:`-rcl 1` (which corresponds to averages of about 100 elements per cell in 3D and 64 in 2D).

.. option:: -dim <dimension1>,<dimension2>,...[:msh]

  Specify the mesh dimension(s), which can be:

  - an integer value between :data:`0` and :data:`3`;
  - :data:`inputdim`: the dimension of the input data;
  - :data:`all`: all (:data:`0` to :data:`inputdim`);
  - :data:`none`: none.

  .. attention::

    The :file:`.msh` file does not depend on this option and will always contain the meshes of all dimensions, unless :data:`:msh` is used.

  .. note::

    If a mesh dimension of 3 is requested while the input data is 2D, the 2D mesh is extruded into a 3D mesh (tetrahedral elements).  The extrusion length is equal to the typical (in-plane) element characteristic length.

  With :data:`-format geof`, use :data:`1,inputdim` to get the 1D mesh written as lisets.

  **Default value**: :data:`inputdim`.

.. option:: -order <order>

  Specify the mesh order, which can be:

  - :data:`1`: linear-interpolation elements (2-node linear elements, 3-node triangular elements, 4-node quadrangular elements, 4-node tetrahedral elements and 8-node hexahedral elements);

  - :data:`2`: quadratic-interpolation elements (3-node linear elements, 6-node triangular elements, 8-node or 9-node quadrangular elements, 10-node tetrahedral elements and 20-node hexahedral elements).

  A mesh loaded using :option:`-loadmesh` is switched to the specified order (in the case of :data:`1`, the second-order nodes are removed).

  **Default value**: :data:`2`.

.. option:: -cl{ver,edge,face,poly} or -rcl{ver,edge,face,poly} <characteristic_length> (secondary options)

  Specify the absolute or relative characteristic lengths of the elements at the vertex, edge, face or polyhedron level (compared to options :option:`-cl` and :data:`-rcl` applying to all entities), which can be:

  - :data:`default`: the same value as the parent, upper-dimension entity (or the :option:`-cl` or :data:`-rcl` value, for a polyhedron);
  - a real value;
  - an expression based on the :ref:`tessellation_keys`, which allows one to define individual values.  An example is :data:`-rclface "(domface==<id>)?<val>:default"`, to get a finer mesh at domain face :data:`<id>`;

  - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  The element characteristic lengths inside a face increase from those at its boundary (its edge mesh) to the :data:`-[r]clface` value, at a rate defined by option :option:`-pl` (or :data:`-plface`).  Similarly, in 3D, the element characteristic lengths inside a cell increase from those at its boundary (its face meshes) to the :data:`-[r]cl` or :data:`-[r]clpoly` value, at a rate defined by option :option:`-pl` (or :data:`-plpoly`).  For example, :data:`-rcl 1 -rclface 0.3` provides a fine mesh at the interfaces and a coarser mesh inside the cells, and :data:`-rcl 1 -rclface 0.6 -rcledge 0.2` also provides a fine mesh at the edges and a coarser mesh inside the faces.

  **Default value**: -.

.. option:: -pl <progression_factor> (secondary option)

  Specify the progression factor for the element characteristic lengths, which corresponds to the ratio between the lengths of two adjacent elements where the element characteristic length increases (:math:`\geq 1`), i.e., from the vertices to the interiors of the edges, and from the boundaries to the interiors of the faces and cells: the characteristic length at a distance :math:`d` from a boundary node of characteristic length :math:`cl_b` is :math:`cl_b + \ln (pl)\,d` (up to the edge, face or cell value).  It also defines the characteristic length of a vertex whose shortest edge is short, of length :math:`L`, which is :math:`pl \, L` (if lower than the target value).

  The value applies to all entities (polyhedra, faces and edges), unless specified otherwise for some of them using :data:`-pl{poly,face,edge}`.

  **Default value**: :data:`2`.

.. option:: -pl{edge,face,poly} <progression_factor> (secondary options)

  Specify the progression factors at the edge, face or polyhedron level (compared to option :option:`-pl` applying to all entities), which can be:

  - :data:`default`: the :option:`-pl` value;
  - a real value (:math:`\geq 1`);
  - an expression based on the :ref:`tessellation_keys`, which allows one to define individual values.  An example is :data:`-plpoly "(body>0)?3:default"`, to get a faster increase of the element characteristic length inside the interior cells;
  - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  :data:`-pledge` applies to the 1D meshing of the edges (from the vertices), :data:`-plface` to the increase of the element characteristic length from the face boundaries to the face interiors, and :data:`-plpoly` to the increase from the polyhedron boundaries to the polyhedron interiors (see :data:`-[r]cl{ver,edge,face,poly}`).

  **Default value**: :data:`default`.

.. option:: -clratio <val_x>:<val_y>:<val_z> (secondary option)

  Specify the ratios between the :data:`cl` values along the :data:`x`, :data:`y` and :data:`z` directions. For example, :data:`2:1:1` leads to elements twice as long in the :data:`x` direction as in the :data:`y` and :data:`z` directions.

  **Default value**: :data:`1:1:1`.

The following options define the multimeshing algorithm (for 2D and 3D free meshing). `Multimeshing` consists of using several meshing algorithms concurrently, for each face or polyhedron, until a minimum, target mesh quality is reached. The mesh quality factor, :math:`O`, accounts for both the element sizes and aspect ratios. It is given by :math:`O = O_{dis}^\alpha \times O_{size}^{1-\alpha}`, where :math:`O_{dis}` and :math:`O_{size}` range from 0 (poor quality) to 1 (high quality), and :math:`\alpha` is a factor equal to 0.8.  Therefore, :math:`O` also ranges from 0 (poor quality) to 1 (high quality) [CMAME2011]_.  :math:`O_{size}` is computed with respect to the target element characteristic lengths given by the mesh size map (see options :option:`-cl` and :option:`-pl`), i.e., for the ratio of the length of each element to the target value at its center (rather than to a uniform value, as in [CMAME2011]_).  The minimum quality value can be modified using option :option:`-meshqualmin`.  The values of :math:`O` and :math:`O_{dis}` can be modified using options :option:`-meshqualexpr` and :option:`-meshqualdisexpr`.  The value of the target mesh quality significantly influences meshing speed and output mesh quality. A value of :data:`0` provides the fastest meshing while a value of :data:`1` provides the best-quality meshing. The default value provides an effective balance. 2D and 3D meshing is performed using Gmsh, and mesh optimization using Gmsh and Netgen (options :option:`-mesh2dalgo` and :option:`-mesh3dalgo`).

.. option:: -meshqualmin <quality_value>

  Specify the minimum target value of mesh quality, :math:`O`, as defined by option :option:`-meshqualexpr`.

  **Default value**: :data:`0.9`.

.. option:: -meshqualexpr <quality_expression> (secondary option)

  Specify the expression of mesh quality, :math:`O`, as a function of :math:`O_{dis}` and :math:`O_{size}`.

  **Default value**: :data:`Odis^0.8*Osize^0.2`.

.. option:: -meshqualdisexpr <quality_expression> (secondary option)

  Specify the expression of the mesh element distortion parameter, :math:`O_{dis}`, as a function of the element distortion parameter :math:`dis`.

  **Default value**: :data:`dis^(exp((dis^0.1)/(dis^0.1-1)))`.

.. option:: -mesh2dalgo <algo1>,<algo2>,... (secondary option)

  Specify the 2D meshing algorithm(s), which can be:

  - :data:`mead`: MeshAdapt;
  - :data:`dela`: Delaunay;
  - :data:`fron`: Frontal.

  According to *multimeshing*, the algorithms are used concurrently until the quality criterion is reached.

  **Default value**: :data:`mead,dela,fron`.

.. option:: -mesh3dalgo <mesh_algo1>:<opti_algo1>,<mesh_algo2>:<opti_algo2>,... (secondary option)

  Specify the 3D meshing and optimization algorithms.  The meshing algorithms can be:

  - :data:`dela`: Delaunay (Gmsh).

  The optimization algorithms can be:

  - :data:`gmsh`: Gmsh;
  - :data:`netg`: Netgen;
  - :data:`gmne`: Gmsh + Netgen;
  - :data:`none`: none.

  According to *multimeshing*, the algorithms are used concurrently until the quality criterion is reached.

  The :data:`netg` meshing algorithm (Netgen) is no longer available.

  **Default value**: :data:`dela:gmsh,dela:gmne,dela:netg`.

.. option:: -meshedge <edge_expr> (secondary option)

  Specify the edges to mesh, which can be :data:`all` or an expression based on the variables described in :ref:`tessellation_keys`.

  **Default value**: :data:`all`.

.. option:: -meshface <face_expr> (secondary option)

  Specify the faces to mesh, which can be :data:`all` or an expression based on the variables described in :ref:`tessellation_keys`.

  **Default value**: :data:`all`.

.. option:: -meshpoly <poly_expr> (secondary option)

  Specify the polyhedra to mesh, which can be :data:`all` or an expression based on the variables described in :ref:`tessellation_keys`.

  **Default value**: :data:`all`.

.. option:: -interface <interface_type> (secondary option)

  Specify the type of interface meshing, which can be:

  - :data:`continuous`: continuous mesh at interfaces with shared nodes between neighbor element sets (which are associated with the tessellation cells);
  - :data:`discontinuous`: discontinuous mesh at interfaces with distinct nodes for the neighbor element sets;
  - :data:`cohesive`: cohesive elements at interfaces joining the neighbor element sets.

  For a periodic (or semi-periodic) tessellation, an interface between a cell and the periodic image of another cell is described at its master location only: the nodes of the periodic image are duplicated there and tied by periodicity to the nodes of the other cell at the slave location, and the cohesive elements are defined there.

  In the case of a multiscale tessellation, it is possible to provide a different value for each scale using the :data:`::` separator (if fewer values than tessellation scales are provided, the last provided value is used for all higher scales).  The value of a scale applies to the interfaces created at that scale (cohesive elements are defined only at the :data:`cohesive` interfaces), and neighbor element sets joined by :data:`continuous` interfaces share their nodes, so that a :data:`discontinuous` or :data:`cohesive` interface is closed where it meets :data:`continuous` interfaces only.

  **Default value**: :data:`continuous`.

.. option:: -mesh2dpinchfix <logical> (secondary option)

  Correct 2D-mesh pinches after 2D meshing (recommended).

  **Default value**: :data:`1`.

Raster Tessellation Meshing Options
-----------------------------------

Direct 2D raster tessellation meshing involves interface reconstruction, interface mesh smoothing and then remeshing. The following options enable the control of interface smoothing.

.. option:: -tesrsmooth <smoothing> (secondary option)

  Specify the method for smoothing the interface meshes reconstructed from a raster tessellation, which can be:

  - :data:`laplacian`: Laplacian smoothing is an iterative method that modifies the coordinates of a node of a mesh using the coordinates of the connected nodes.  At iteration :math:`i`, the position of a node, :math:`X_i`, is calculated from its previous position, :math:`X_{i-1}`, and the position of the barycenter of the neighboring nodes (weighted barycenter considering the inverse of the distance between the node and the neighbor), :math:`X^n_{i-1}`, as follows: :math:`X_i=(1-A)\,X_{i-1} + A\,X^n_{i-1}`.  :math:`A \in \left[0,\,1\right]` is an adjustable parameter (see option :option:`-tesrsmoothfact`). The number of iterations is set by option :option:`-tesrsmoothitermax`.  There is no stopping criterion, so that the maximum number of iterations is always reached.
  - :data:`none`: none.

  **Default value**: :data:`laplacian`.

.. option:: -tesrsmoothfact <fact> (secondary option)

  Specify the factor used for the interface mesh smoothing (:math:`A` in option :option:`-tesrsmooth`).

  **Default value**: :data:`0.5`.

.. option:: -tesrsmoothitermax <iter_number> (secondary option)

  Specify the number of iterations used for interface mesh smoothing.

  **Default value**: :data:`5`.

Mesh Cleaning Options
---------------------

The following options are specific to mapped meshing of raster tessellations containing voids.

.. option:: -singnodedup <logical> (secondary option)

  Duplicate singular nodes.  A singular node belongs to several elements that share only this node or an edge, which provides a singular behavior [#singular]_.

  Duplicated nodes can be merged back using option :option:`-dupnodemerge`.

  **Default value**: :data:`0`.

.. option:: -dupnodemerge <distance> (secondary option)

  Merge duplicate nodes.  The argument is the distance between nodes below which two nodes are merged.

  Note that Neper does not generate meshes with duplicate nodes, except when :option:`-singnodedup` or :option:`-interface` are used.

  **Default value**: -.

Transformation Options
----------------------

.. option:: -transform <transformation1>,<transformation2>,...

  Apply successive transformations to a mesh, which can be:

  - |translate|;
  - |rotate|;
  - |scale|;
  - :data:`smooth(<A>,<itermax>,<type>)`: smooth the interfaces of a mesh by Laplacian smoothing (use only if you know what you are doing).  Laplacian smoothing is an iterative method that modifies the coordinates of a node using the coordinates of the neighboring nodes.  At iteration :math:`i`, the position of a node, :math:`X_i`, is calculated from its previous position, :math:`X_{i-1}`, and the position of the barycenter of the neighboring nodes (weighted barycenter considering the inverse of the distance between the node and the neighbor), :math:`X^n_{i-1}`, as follows: :math:`X_i=(1-A)\,X_{i-1} + A\,X^n_{i-1};\,0\leq A \leq 1`.  :data:`<itermax>` iterations are applied. :data:`<type>` defines the nodes used for smoothing and can be :data:`all` for all nodes or :data:`interior` for interior nodes.
  - :data:`explode(<fact>)`: produce a roughly "exploded" mesh for which elsets are apart from each other (or cohesive elements become thick).  To be used in conjunction with :data:`-interface {discontinuous,cohesive}`.  :data:`<fact>` is a factor controlling the distance between elsets.
  - :data:`slice(<d>,<a>,<b>,<c>)`: slice a 3D mesh by the (oriented) plane of equation :math:`a\,x+b\,y+c\,z=d` (yielding a 2D mesh).
  - :data:`node(<file_name>)`: override node positions with ones defined in a :ref:`data_file`.
  - :data:`ori(<file_name>[,des=<descriptor>])`: override orientations with ones defined in a :ref:`data_file` written using a specific descriptor (see :ref:`rotations_and_orientations`, default :data:`rodrigues`).  The file may contain orientations for elsets or for elements, and this is automatically determined from the number of lines (in the case of element orientations, elset orientations are updated accordingly, by averaging).

  **Default value**: -.

.. index::
   single: -sortnode
   single: -sortelt

.. option:: -sort{node,elt} <sort_expression> (secondary option)

  Sort nodes or elements (typically to facilitate data post-processing) according to a mathematical expression based on the variables described in :ref:`mesh_keys`, in ascending order.

  **Default value**: -.

Mesh Partitioning Options
-------------------------

Mesh partitioning is achieved using the `libscotch <https://www.labri.fr/perso/pelegrin/scotch>`_ library. The principle of mesh partitioning is to create partitions of the same size, in terms of nodes and elements, while minimizing the interfaces between them. This attempts to distribute an equal load to all computation units and minimizes communications between them, and therefore minimizes the total computation time of simulations.  Partitioning is applied to the higher-dimension mesh and does not change node and element numbering.  This is done using option :option:`-part`.

.. option:: -part <part_number>

  Specify the number of partitions, which can be any integer value.

  **Default value**: -.

Field Transport Options
-----------------------

.. option:: -transport <entity_type1>:<data_type1>:<file_name1>,<data_type2>:<file_name2>,...

  Transport data from a parent mesh to a child mesh.  The parent mesh is the input mesh and the child mesh is the result mesh (created by remeshing or loaded with :option:`-loadmesh`).  :data:`<entity_type>` can be :data:`node` or :data:`elt`, and :data:`<data_type>` can be :data:`integer\<X\>` or :data:`real\<X\>`, where :data:`<X>` is the data dimensionality, and :data:`<file_name>` is the file containing the parent data.

  Nodal data are transported by interpolation using the shape functions (2D only).  Elemental data are transported as specified by :option:`-transporteltmethod`.

  **Default value**: -.

.. option:: -transporteltmethod <method> (secondary option)

  Specify the method to transport elemental data from the parent mesh to the child mesh. For each element of the child mesh, its center, :math:`c`, is considered.  The method can be:

  - :data:`distance`: choose, for each element of the child mesh (of center :math:`c`), the element of the parent mesh whose center is the closest to :math:`c`;
  - :data:`location`: choose the element of the parent mesh that :math:`c` belongs to.

  **Default value**: :data:`distance`.

Output Options
--------------

.. option:: -o <file_name>

  Specify the output file name.

  **Default value**: input file's base name.

.. option:: -format <format1>,<format2>,...

  Specify the format(s) of the output file(s), which can be:

  - :data:`msh[:<encoding>]`: native (Gmsh-style) mesh file, where :data:`encoding` can be :data:`ascii` or :data:`binary` (default :data:`ascii`);
  - :data:`sim`: simulation directory;
  - :data:`vtk`: VTK mesh file;
  - :data:`inp`: Abaqus input file;
  - :data:`geof`: Z-set mesh file;
  - :data:`ori`: orientation file (optional FEPX input);
  - :data:`phase`: group file (optional FEPX input);
  - :data:`bcs`: boundary condition file (optional FEPX input);
  - :data:`per`: periodicity file (see also option :option:`-performat`);
  - :data:`tess`: tessellation file, where the tessellation is reconstructed from the mesh;
  - :data:`etree`: element-orientation k-d tree file (for development only);
  - :data:`ntree`: node-orientation k-d tree file (for development only).

  **Default value**: :data:`msh`.

.. option:: -nset <nset1>,<nset2>,...

  Specify the node sets to provide, which can be:

  - :data:`faces`: domain faces;
  - :data:`edges`: domain edges;
  - :data:`vertices`: domain vertices;
  - :data:`facebodies`: interior of the domain faces;
  - :data:`edgebodies`: interior of the domain edges;
  - :data:`all`: all of the above node sets;
  - :data:`<dom_entity_label>`: label of a domain entity.

    - For a cubic domain, they are :data:`{x-z}{0,1}`: domain faces, :data:`{x-z}{0,1}{x-z}{0,1}`: domain edges, and :data:`{x-z}{0,1}{x-z}{0,1}{x-z}{0,1}`: domain vertices.
    - For a cylindrical domain, they are :data:`z{0,1}` for the :data:`z` faces and :data:`cyl` for the curved face (or :data:`f{1,2,...}` for the faces of a faceted cylinder).
    - For an hourglass domain, they are :data:`z{0,1}` for the :data:`z` faces and :data:`hourglass` for the curved face.
    - For other domains, they are :data:`f{1,2,...}` for the faces.

    The labels of edges and vertices are derived by concatenating those of their parent faces. :data:`body` can be appended to obtain only the interior nodes.

  - :data:`<nset_label>:\<nset_definition\>`, where :data:`<nset_label>` is a custom nset label and :data:`<nset_definition>` defines the nodes belonging to the nset, which can be:

    - an expression based on the variables provided in :ref:`mesh_keys` (for nodes), for example :data:`bottom:z<1e-6`;
    - :data:`min(<expr>)` or :data:`max(<expr>)`: the nodes for which the expression is minimal or maximal (to a relative tolerance of :math:`10^{-6}`), for example :data:`bottom:min(z)`;
    - :data:`closest(<x>,<y>,<z>)`: the node closest to a point, for example :data:`center:closest(0.5,0.5,0.5)`.

  - :data:`none`: none.

  The node sets of the domain faces, edges and vertices that are periodic (see :option:`-periodicity` in module :ref:`neper_t`), which are not boundaries, are not provided (a fully periodic mesh has no node sets).

  **Default value**: :data:`faces,edges,vertices` in 3D and :data:`edges,vertices` in 2D.

.. option:: -faset <faset1>,<faset2>,...

  Specify the element surface meshes (edge meshes in 2D) to output, which can be:

  - :data:`faces`: all domain faces (except the periodic ones, which are not boundaries);
  - :data:`<dom_face_label>`: individual faces (see option :option:`-nset`);
  - :data:`internal`: internal faces (edges in 2D) as created by :data:`-interface discontinuous`;
  - :data:`none`: none.

  **Default value**: :data:`faces`.

.. option:: -elset <elset1>,<elset2>,... (secondary option)

  Specify the element sets to output, which can be:

  - :data:`default`: default element sets (those corresponding to the input tessellation cells);
  - :data:`<elset_label>:\<elset_definition\>`, where :data:`<elset_label>` is a custom elset label and :data:`<elset_definition>` is an expression defining the elements belonging to the elset, based on the variables provided in :ref:`mesh_keys`.

  **Default value**: :data:`default`.

.. option:: -performat <format>

  Specify the format of the :file:`.per` output file, which can be:

  - :data:`msh`;
  - :data:`plain`: plain format;
  - :data:`geof`: Z-set format.

  **Default value**: :data:`msh`.

Post-Processing Options
-----------------------

In all :data:`-stat` options, a key can also be any mathematical expression based on the keys, for example :data:`x+y+z` or :data:`vol>0.1`.

The following options provide general statistics on the highest-dimension mesh (:data:`mesh`), 0D mesh (:data:`mesh0d`), 1D mesh (:data:`mesh1d`), 2D mesh (:data:`mesh2d`), 3D mesh (:data:`mesh3d`) and cohesive-element mesh (:data:`meshco`).

.. option:: -stat{mesh,mesh{0-3}d,meshco} <key1>,<key2>,...

  Provide statistics on the mesh. Available keys are described in :ref:`mesh_keys`.

  **Default value**: -.

  **File extension**: :file:`.stmesh`, :file:`.stmesh{0-3}d`, :file:`.stmeshco`.

The following options provide statistics on the nodes (:data:`node`), highest-dimension elements (:data:`elt`) and element sets (:data:`elset`), 0D elements (:data:`elt0d`) and element sets (:data:`elset0d`), 1D elements (:data:`elt1d`) and element sets (:data:`elset1d`), 2D elements (:data:`elt2d`) and element sets (:data:`elset2d`), 3D elements (:data:`elt3d`) and element sets (:data:`elset3d`), cohesive elements (:data:`eltco`) and element sets (:data:`elsetco`), and points (:data:`point`).

.. option:: -stat{node,elt,elt{0-3}d,elset,elset{0-3}d,eltco,elsetco} <key1>,<key2>,...

  Provide statistics on the nodes, elements or elsets.  Available keys are described in :ref:`mesh_keys`.

  **Default value**: -.

  **File extension**: :file:`.stnode`, :file:`.stelt`, :file:`.stelt{0-3}d`, :file:`.stelset`, :file:`.stelset{0-3}d`, :file:`.steltco`, :file:`.stelsetco`.

.. option:: -statpoint <key1>,<key2>,...

  Provide statistics on points.  The points must be loaded with option :option:`-loadpoint`. Available keys are described in :ref:`point_keys`.

  **Default value**: -.

  **File extension**: :file:`.stpoint`.

Advanced Options
----------------

These advanced options set running conditions for the meshing libraries (2D and 3D meshing).

.. option:: -mesh2dclreps <tolerance> (secondary option)

  Specify the relative tolerance on the average ratio of the element characteristic lengths of each face to their target values (given by the mesh size map, see options :option:`-cl` and :option:`-pl`), which can be:

  - a real value;
  - an expression based on the variables defined in :ref:`tessellation_keys`;
  - :data:`<file_name>`: values to load from a :ref:`data_file`.

  As Gmsh's 2D mesher does not exactly provide elements of the target values, the mesh size map passed to it is scaled iteratively, and so this is a CPU-sensitive option.

  **Default value**: :data:`0.02`.

.. option:: -mesh3dclreps <tolerance> (secondary option)

  Same as :option:`-mesh2dclreps`, but for the cells (3D meshing).  As Gmsh's 3D mesher generally provides elements larger than the target values, this is a highly CPU-sensitive option.

  **Default value**: :data:`0.02`.

.. index::
   single: -mesh2dmaxtime
   single: -mesh3dmaxtime

.. option:: -mesh{2,3}dmaxtime <time> (secondary option)

  Specify the maximum processing time allowed to the meshing library for meshing a tessellation face or polyhedron (in seconds).

  **Default value**: :data:`10000`.

.. index::
   single: -mesh2drmaxtime
   single: -mesh3drmaxtime

.. option:: -mesh{2,3}drmaxtime <relative_time> (secondary option)

  Specify a factor used to determine the maximum processing time allowed to the meshing library for meshing a tessellation face or polyhedron.  This option is similar to :data:`-mesh2dmaxtime`, but the actual maximum time is the product of the maximum processing time of the previous meshing operations by the value provided in argument.

  **Default value**: :data:`100`.

.. option:: -mesh{2,3}diter <iter_number> (secondary option)

  Specify the maximum number of meshing attempts for a particular face or polyhedron (in the rare case of failure).

  **Default value**: :data:`3`.

Output Files
------------

Mesh
~~~~

  - :file:`.msh`: mesh file as described in :ref:`mesh_file`;
  - :file:`.vtk`: VTK file;
  - :file:`.inp`: Abaqus input file;
  - :file:`.geof`: Z-set file;
  - :file:`.ori`: orientation file (optional FEPX file).  It contains the :data:`$ElsetOrientations` and :data:`$ElementOrientations` sections of the :file:`.msh` file;
  - :file:`.phase`: group file (optional FEPX file).  It contains the :data:`$Groups` section of the :file:`.msh` file;
  - :file:`.bcs`: boundary conditions file (optional FEPX file);
  - :file:`.per`: periodicity file;
  - :file:`.etree`: element-orientation k-d tree file;
  - :file:`.ntree`: node-orientation k-d tree file.

Interfaces
~~~~~~~~~~

- File :file:`.intl`.  For each 2D interface, the file provides (one 2D interface per line) the labels of the two element sets of the interface.

Statistics
~~~~~~~~~~

Statistics files are provided for nodes, elements, element sets and points.  They are formatted with one entity per line.  Each line contains the data specified to the corresponding :data:`-stat` option and described in :ref:`mesh_keys`.

  - :file:`.stnode`: node statistics file;
  - :file:`.stmesh`: (highest-dimension) mesh statistics file;
  - :file:`.stelt`: (highest-dimension) element statistics file;
  - :file:`.stelset`: (highest-dimension) element set statistics file;
  - :file:`.stmesh0d`: 0D mesh statistics file;
  - :file:`.stmesh1d`: 1D mesh statistics file;
  - :file:`.stmesh2d`: 2D mesh statistics file;
  - :file:`.stmesh3d`: 3D mesh statistics file;
  - :file:`.stmeshco`: cohesive-element mesh statistics file;
  - :file:`.stelt0d`: 0D element statistics file;
  - :file:`.stelt1d`: 1D element statistics file;
  - :file:`.stelt2d`: 2D element statistics file;
  - :file:`.stelt3d`: 3D element statistics file;
  - :file:`.stelset0d`: 0D elset statistics file;
  - :file:`.stelset1d`: 1D elset statistics file;
  - :file:`.stelset2d`: 2D elset statistics file;
  - :file:`.stelset3d`: 3D elset statistics file;
  - :file:`.stpoint`: point statistics file.

.. rubric:: Footnotes

.. [#partitioning] Each partition being assigned to a computation core in the finite element calculation, the minimization of the interfaces between the partitions is done in terms of the number of necessary communications between computation cores.

.. [#singular] In mechanics, it corresponds to imposing a common displacement while the point can carry no stress, and, in heat transfer, it corresponds to imposing a given temperature at a particular location shared by two bodies while the location can carry no heat flux.  When such a node is duplicated, each body has its own node and the singular behavior disappears.
