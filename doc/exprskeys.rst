.. _expressions_and_keys:

Expressions and Keys
====================

.. _mathematical_and_logical_expressions:

Mathematical and Logical Expressions
------------------------------------

Neper handles mathematical expressions thanks to the :program:`muparser` library. The expression must contain no space, tabulation or new-line characters, and match the following syntax. [#muparser_doc]_

.. _functions:

Functions
~~~~~~~~~

The following table gives an overview of the functions supported by the default implementation. It lists the function names, the number of arguments and a brief description.

=============== =========================================================
**Name**        **Description**
:data:`sin`     sine function
:data:`cos`     cosine function
:data:`tan`     tangens function
:data:`asin`    arcus sine function
:data:`acos`    arcus cosine function
:data:`atan`    arcus tangens function
:data:`sinh`    hyperbolic sine function
:data:`cosh`    hyperbolic cosine
:data:`tanh`    hyperbolic tangens function
:data:`asinh`   hyperbolic arcus sine function
:data:`acosh`   hyperbolic arcus tangens function
:data:`atanh`   hyperbolic arcur tangens function
:data:`log2`    logarithm to the base 2
:data:`log10`   logarithm to the base 10
:data:`log`     logarithm to the base 10
:data:`ln`      logarithm to base :math:`e` (2.71828...)
:data:`exp`     e raised to the power of x
:data:`sqrt`    square root of a value
:data:`sign`    sign function: -1 if :math:`x<0`; 1 if :math:`x>0`
:data:`rint`    round to nearest integer
:data:`abs`     absolute value
:data:`min`     min of all arguments
:data:`max`     max of all arguments
:data:`sum`     sum of all arguments
:data:`avg`     mean value of all arguments
=============== =========================================================

.. _binary_operators:

Binary Operators
~~~~~~~~~~~~~~~~

The following table lists the default binary operators supported by the parser.

============ ============================ =================
**Operator** **Description**              **Priority**
:data:`&&`   logical and                  1
:data:`||`   logical or                   2
:data:`<=`   less or equal                4
:data:`>=`   greater or equal             4
:data:`\!=`  not equal                    4
:data:`==`   equal                        4
:data:`>`    greater than                 4
:data:`<`    less than                    4
:data:`+`    addition                     5
:data:`-`    subtraction                  5
:data:`*`    multiplication               6
:data:`/`    division                     6
:data:`^`    raise x to the power of y    7
============ ============================ =================

.. _ternary_operators:

Ternary Operators
~~~~~~~~~~~~~~~~~

The parser has built in support for the if-then-else operator. It uses lazy evaluation in order to make sure only the necessary branch of the expression is evaluated.

==================== ====================================================
**Operator**         **Description**
:data:`?:`           if-then-else operator, following the C/C++ syntax: :data:`(\<test\>)?\<value_if_true\>:\<value_if_false\>`.
==================== ====================================================

.. _statistical_distributions:

Statistical Distributions
~~~~~~~~~~~~~~~~~~~~~~~~~

The following table lists the statistical distributions.  Custom endpoints (not indicated) can also be added as arguments, as described in the following.

============================================= ==================== ======================================
**Operator**                                  **Description**      **Information**
:data:`normal(<mean>,<sigma>)`                normal
:data:`lognormal(<mean>,<sigma>)`             lognormal
:data:`dirac(<mean>)`                         Dirac
:data:`beta(<x>,<y>)`                         beta function        :math:`x>0`, :math:`y>0`
:data:`lorentzian(<mean>,<sigma>)`            Lorentzian
:data:`studentst(<mean>,<sigma>)`             Student's t
:data:`weibull(k,<sigma>)`                    Weibull              :math:`k > 0` represents the shape
:data:`breitwigner(<mean>,<sigma>[,<gamma>])` Breit-Wigner         :math:`<gamma> \geq 0`, default 1
:data:`expnormal(<mean>,<sigma>[,<gamma>])`   exp-normal           :math:`<gamma> > 0`, default :math:`<sigma>`
:data:`moffat(<mean>,<sigma>[,<gamma>])`      Moffat               :math:`<gamma> > 0`, default 1
:data:`pearson7(<mean>,<sigma>[,<gamma>])`    Pearson type VII     default :math:`<gamma> = 1.5`
:data:`pseudovoigt(<mean>,<sigma>[,<gamma>])` Pseudo-Voigt         :math:`<gamma> \in [0,\,1]`, default 0.5
:data:`skewnormal(<mean>,<sigma>[,<gamma>])`  skewed normal        default :math:`<gamma> = <sigma>`
:data:`custom(<file_name>)`                   custom
============================================= ==================== ======================================

:data:`<mean>` represents the mean (or centre), and :data:`<sigma>` represents the standard deviation (or scale, :math:`> 0`). :data:`<gamma>` depends on the distribution function (see the above table). For all distributions, custom endpoints can be added as last arguments, as :data:`<from_value>,\<to_value\>`, where :data:`<from_value>` is the lower endpoint and :data:`<to_value>` is the upper endpoint. The parameter keywords do not need to be provided, but, when they are, the parameters can be given in any order, as in :data:`moffat(gamma=1,from=0,to=1,sigma=0.1,mean=0.5)`.  Endpoints are considered inclusive by default, but exclusive endpoints can be specified using :data:`fromexclusive=\<from_value\>` and :data:`toexclusive=\<to_value\>` (:data:`frominclusive=\<from_value\>` and :data:`toinclusive=\<to_value\>` can be used for inclusive endpoints).  String completion is available for the keywords.  Finally, a sum of distributions of increasing averages can be provided, as in :data:`0.3*lognormal(0.5,0.1)+0.7*normal(1,0.1)`.

.. attention:: When :data:`from` and/or :data:`to` are used, they should preferably be so that the distribution retains the same mean; otherwise, the distribution is shifted after truncation to match the specified mean.

In the case of the :data:`custom` distribution, the numerical distribution must be provided in the file. The file must contain the x and y values of the distribution on successive lines.  The x values must be provided in ascending order and form a regular grid.  The distribution must contain at least 3 points and does not need to integrate to 1.

.. _tessellation_keys:

Tessellation Keys
-----------------

Available keys for a tessellation itself are provided below.

=============== =================================== ==================
**Key**         **Descriptor**                      **Apply to**
:data:`dim`     dimension                           tess
:data:`vernb`   number of vertices                  tess
:data:`edgenb`  number of edges                     tess
:data:`facenb`  number of faces                     tess
:data:`polynb`  number of polyhedra                 tess
:data:`cellnb`  number of cells                     tess
:data:`x`       x coordinate                        tess
:data:`y`       y coordinate                        tess
:data:`z`       z coordinate                        tess
:data:`coo`     x, y and z coordinates              tess
:data:`area`    surface area                        tess
:data:`vol`     volume                              tess
:data:`size`    size (surface area/volume in 2D/3D) tess
:data:`step`    simulation step                     tess
=============== =================================== ==================

Available keys for tessellation seeds, vertices, edges, faces, polyhedra, crystals and cell groups are provided below.  Also note that the keys apply to *cells* if they are tagged to apply to *polyhedra* and the tessellation is 3D and *faces* and the tessellation is 2D, and that keys apply to *crystals* if they apply to *cells*.  You may also replace, in the tessellation keys themselves, :data:`poly` by :data:`cell` if the tessellation is 3D and :data:`face` by :data:`cell` if the tessellation is 2D (it applies only in rare cases).  For example, for a 2D tessellation, you may use :data:`-statcell ncells` instead of :data:`-statface nfaces`. Keys specific to cells are defined accordingly in the following but also apply to *polys* is the tessellation is 3D and *faces* is the tessellations is 2D.

To turn a key value into a value relative to the mean over all entities (e.g. the relative cell size), append the key expression with the :data:`:rel` modifier.  To turn a key value into a value which holds for a unit cell size, append the key expression with the :data:`:uc` modifier.  To use as a reference only the *body* entities (see below), append :data:`b` to the modifiers.

================================= =================================================================================================== =========================================
**Key**                           **Descriptor**                                                                                      **Apply to**
:data:`id`                        identifier                                                                                          seed, ver, edge, face, poly, group
:data:`x`                         x coordinate                                                                                        seed, ver, edge, face, poly
:data:`y`                         y coordinate                                                                                        seed, ver, edge, face, poly
:data:`z`                         z coordinate                                                                                        seed, ver, edge, face, poly
:data:`coo`                       x, y and z coordinates                                                                              seed, ver, edge, face, poly
:data:`xmin`                      minimum x coordinate                                                                                edge, face, poly
:data:`ymin`                      minimum y coordinate                                                                                edge, face, poly
:data:`zmin`                      minimum z coordinate                                                                                edge, face, poly
:data:`xmax`                      maximum x coordinate                                                                                edge, face, poly
:data:`ymax`                      maximum y coordinate                                                                                edge, face, poly
:data:`zmax`                      maximum z coordinate                                                                                edge, face, poly
:data:`w`                         weight (width for a lamellar tessellation)                                                          seed, cell
:data:`body[<expr>]`              body level                                                                                          ver, edge, face, poly
:data:`state`                     state                                                                                               ver, edge, face, poly
:data:`domtype`                   type of domain (0 if on a domain vertex, 1 if on a domain edge and 2 if on a domain face)           ver, edge, face
:data:`domface`                   domain face (-1 if undefined)                                                                       face
:data:`domedge`                   domain edge (-1 if undefined)                                                                       edge
:data:`domver`                    domain vertex (-1 if undefined)                                                                     ver
:data:`scale`                     scale                                                                                               ver, edge, face, poly, cell [#multiscale_entity]_
:data:`length`                    length                                                                                              edge
:data:`area`                      surface area                                                                                        face, poly, group
:data:`vol`                       volume                                                                                              poly, group
:data:`size`                      size (surface area/volume in 2D/3D)                                                                 cell, group
:data:`diameq`                    equivalent diameter [#equivalent_diameter]_                                                         face, poly
:data:`avdiameq`                  average equivalent diameter [#equivalent_diameter]_                                                 face, poly
:data:`radeq`                     equivalent radius (half of the eq. diameter)                                                        face, poly
:data:`avradeq`                   average equivalent radius (half of the eq. diameter)                                                face, poly
:data:`sphericity`                sphericity [#sphericity]_                                                                           poly
:data:`circularity`               circularity [#circularity]_                                                                         face
:data:`convexity`                 convexity [#convexity]_                                                                             face (only for a 2D tessellation), poly
:data:`dihangleav`                average dihedral angle                                                                              face, poly
:data:`dihanglemin`               minimum dihedral angle                                                                              face, poly
:data:`dihanglemax`               maximum dihedral angle                                                                              face, poly
:data:`dihangles`                 dihedral angles                                                                                     face, poly
:data:`ff`                        flatness fault (in degrees)                                                                         face
:data:`theta`                     disorientation angle (in degrees)                                                                   edge (in 2D), face (in 3D)
:data:`cyl`                       cylinder polygonization [#cyl]_                                                                     edge
:data:`vernb`                     number of vertices                                                                                  edge, face, poly
:data:`vers`                      vertices                                                                                            edge, face, poly
:data:`edgenb`                    number of edges                                                                                     ver, face, poly
:data:`edges`                     edges                                                                                               ver, face, poly
:data:`facenb`                    number of faces                                                                                     ver, edge, poly
:data:`faces`                     faces                                                                                               ver, edge, poly
:data:`polynb`                    number of polyhedra                                                                                 ver, edge, face
:data:`polys`                     polyhedra                                                                                           ver, edge, face
:data:`nfacenb`                   number of neighboring faces                                                                         face
:data:`nfaces`                    neighboring faces                                                                                   face
:data:`nfacenb_samedomain`        number of neighboring faces of the same domain (parent cell of a multiscale tessellation)           face (in 2D)
:data:`nfaces_samedomain`         neighboring faces of the same domain (parent cell of a multiscale tessellation)                     face (in 2D)
:data:`npolynb`                   number of neighboring polyhedra                                                                     poly
:data:`npolys`                    neighboring polyhedra                                                                               poly
:data:`npolys_unsort`             neighboring polyhedra, unsorted list                                                                poly
:data:`npolynb_samedomain`        number of neighboring polyhedra of the same domain (parent cell of a multiscale tessellation)       poly
:data:`npolys_samedomain`         neighboring polyhedra of the same domain (parent cell of a multiscale tessellation)                 poly
:data:`vercoos`                   vertex coordinates                                                                                  face, poly
:data:`faceareas`                 face surface areas                                                                                  poly
:data:`faceeqs`                   face equations [#face_equation]_                                                                    poly
:data:`nseednb`                   number of neighboring seeds                                                                         poly
:data:`nseeds`                    neighboring seeds [#nseeds]_                                                                        poly
:data:`scaleid(<scale_nb>)`       identifier of the corresponding cell at scale :data:`<scale_nb>`                                    cell
:data:`lam`                       lamella width id [#lam]_                                                                            cell
:data:`mode`                      mode [#mode]_                                                                                       cell
:data:`group`                     group                                                                                               cell
:data:`per`                       periodic (1 if periodic, 0 otherwise)                                                               ver, edge, face (in 3D)
:data:`fiber(...)`                1 if in orientation fiber and 0 otherwise, see :ref:`orientation_fibers`                            poly
:data:`<orientation_descriptor>`  :ref:`orientation descriptor <rotation_and_orientation_descriptors>`                                face (in 2D), poly (in 3D)
:data:`step`                      simulation step                                                                                     ver, edge, face, poly
================================= =================================================================================================== =========================================

  Variables consisting of several values (:data:`vers`, etc.) are not available for sorting
  (option :option:`-sort`).

  For a cell, the :data:`body` variable is defined as follows:

  - In the general case (:data:`body`, no argument provided), it is an integer equal to :data:`0` if the cell is at the domain boundary, i.e. if it shares at least one face with it (edge in 2D), and is equal to :data:`1` or higher otherwise.  This is determined as follows: if a cell is surrounded by cells with :data:`body` values equal to or higher than :data:`n`, its :data:`body` value is equal to :data:`n + 1`.  Therefore, :data:`body` tends to increase with the distance to the domain boundary and can be used to define cells that may suffer from boundary effects.

  - In the case where an expression is provided as argument (:data:`body(<expr>)`), the expression is a logical expression that defines the boundary to consider, from the domain face (edge in 2D) labels (for a cube, :data:`x0`, :data:`x1`, :data:`y0`, :data:`y1`, :data:`z0` and :data:`z1`).  For example, :data:`body(z0||z1)` considers only the :data:`z0` and :data:`z1` domain faces as the boundary, and the more exotic :data:`body(x1&&y0||z1)` considers only the intersection between the :data:`x1` and :data:`y0` domain faces, and the :data:`z1` domain face as the boundary.

  For entities of lower dimension than cells (vertices, edges and faces), :data:`body` is equal to the maximum :data:`body` value of the cells they belong to.

.. _raster_tessellation_keys:

Raster Tessellation Keys
------------------------

Available keys for raster tessellation itself are provided below.

===================== ============================================ ======================
**Key**               **Descriptor**                               **Apply to**
:data:`dim`           dimension                                    tesr
:data:`voxnbx`        number of voxels in direction x              tesr
:data:`voxnby`        number of voxels in direction y              tesr
:data:`voxnbz`        number of voxels in direction z              tesr
:data:`voxnb`         number of voxels in total                    tesr
:data:`originx`       origin x coordinate                          tesr
:data:`originy`       origin y coordinate                          tesr
:data:`originz`       origin z coordinate                          tesr
:data:`voxsizex`      voxel size in direction x                    tesr
:data:`voxsizey`      voxel size in direction y                    tesr
:data:`voxsizez`      voxel size in direction z                    tesr
:data:`rastersizex`   raster size in direction x                   tesr
:data:`rastersizey`   raster size in direction y                   tesr
:data:`rastersizez`   raster size in direction z                   tesr
:data:`rastersize`    raster size (surface area/volume in 2D/3D)   tesr
:data:`area`          surface area                                 tesr
:data:`vol`           volume                                       tesr
:data:`size`          size (surface area/volume in 2D/3D)          tesr
:data:`x`             x coordinate                                 tesr
:data:`y`             y coordinate                                 tesr
:data:`z`             z coordinate                                 tesr
:data:`coo`           x, y and z coordinates                       tesr
:data:`step`          simulation step                              tesr
===================== ============================================ ======================

Available keys for raster tessellation seeds, cells, cell groups and voxels are provided below.  Mathematical and logical expressions based on these keys can also be used.  To turn a key value into a value relative to the mean over all entities (e.g.the relative cell size), append the key expression with the :data:`:rel` modifier.  To turn a key value into a value which holds for a unit cell size, append the key expression with the :data:`:uc` modifier.

General
~~~~~~~

============================ ======================================================================= ====================================
**Key**                      **Descriptor**                                                          **Applies to**
:data:`id`                   identifier                                                              seed, cell, group, voxel
:data:`cell`                 cell                                                                    voxel
:data:`oridef`               orientation is defined                                                  voxel
:data:`w`                    Laguerre weight                                                         seed
:data:`step`                 simulation step                                                         tesr
============================ ======================================================================= ====================================

Geometry
~~~~~~~~

============================ ======================================================================= ====================================
**Key**                      **Descriptor**                                                          **Applies to**
:data:`x`                    x coordinate                                                            seed, cell, voxel
:data:`y`                    y coordinate                                                            seed, cell, voxel
:data:`z`                    z coordinate                                                            seed, cell, voxel
:data:`coo`                  x, y and z coordinates                                                  seed, cell, voxel
:data:`vx`                   x coordinate (in voxel)                                                 voxel
:data:`vy`                   y coordinate (in voxel)                                                 voxel
:data:`vz`                   z coordinate (in voxel)                                                 voxel
:data:`vcoo`                 x, y and z coordinates (in voxel)                                       voxel
:data:`vxmin`                minimum x coordinate (in voxel)                                         cell
:data:`vymin`                minimum y coordinate (in voxel)                                         cell
:data:`vzmin`                minimum z coordinate (in voxel)                                         cell
:data:`vxmax`                maximum x coordinate (in voxel)                                         cell
:data:`vymax`                maximum y coordinate (in voxel)                                         cell
:data:`vzmax`                maximum z coordinate (in voxel)                                         cell
:data:`domvxmin`             domain minimum x coordinate (in voxel), always 1                        domain
:data:`domvymin`             domain minimum y coordinate (in voxel), always 1                        domain
:data:`domvzmin`             domain minimum z coordinate (in voxel), always 1                        domain
:data:`domvxmax`             domain maximum x coordinate (in voxel)                                  domain
:data:`domvymax`             domain maximum y coordinate (in voxel)                                  domain
:data:`domvzmax`             domain maximum z coordinate (in voxel)                                  domain
:data:`area`                 surface area                                                            cell, group (in 2D)
:data:`vol`                  volume                                                                  cell, group (in 3D)
:data:`size`                 size (surface area/volume in 2D/3D)                                     cell, group
:data:`voxnb`                number of voxels                                                        cell
:data:`areafrac`             surface area fraction                                                   group (in 2D)
:data:`volfrac`              volume fraction                                                         group (in 3D)
:data:`sizefrac`             size fraction (surface area/volume fraction in 2D/3D)                   group
:data:`diameq`               equivalent diameter [#equivalent_diameter]_                             cell
:data:`radeq`                equivalent radius                                                       cell
:data:`convexity`            convexity [#convexity]_                                                 cell
============================ ======================================================================= ====================================

Orientation
~~~~~~~~~~~
================================ ======================================================================= ====================================
**Key**                          **Descriptor**                                                          **Applies to**
:data:`<orientation_descriptor>` :ref:`orientation descriptor <rotation_and_orientation_descriptors>`    voxel, cell
:data:`gos`                      grain orientation spread [#gos]_                                        cell
:data:`oridisanisoangles`        orientation distribution anisotropy / principal angles [#JMPS2015]_     cell
:data:`oridisanisoaxes`          orientation distribution anisotropy / principal axes [#JMPS2015]_       cell
:data:`oridisanisofact`          orientation distribution anisotropy factor [#JMPS2015]_                 cell
:data:`oridisanisodeltas`        orientation distribution anisotropy / principal delta angles [#deltas]_ cell
================================ ======================================================================= ====================================

.. _tessellation_optimization_keys:

Tessellation Optimization Keys
------------------------------

.. _time_keys:

Time Keys
~~~~~~~~~

The available keys for option :option:`-morphooptilogtime` are provided below. Use :data:`iter(<factor>)`, where :data:`factor` is an integer reduction factor, to log values only at specific iteration numbers.

============================= ============================================================
**Key**                       **Descriptor**
:data:`iter`                  iteration number
:data:`varupdateqty`          number of updated variables
:data:`seedupdateqty`         number of updated seeds
:data:`seedupdatelist`        list of updated seeds
:data:`cellupdateqty`         number of updated cells
:data:`cellupdatelist`        list of updated cells
:data:`var`                   time for variable update
:data:`seed`                  time for seed update
:data:`cell_init`             time for cell update initialization
:data:`cell_kdtree`           time for cell update kdtree computation
:data:`cell_shift`            time for cell update shift computation
:data:`cell_neigh`            time for cell update neighbor computation
:data:`cell_cell`             time for cell update cell computation
:data:`cell_other`            time for cell update others
:data:`cell_total`            total time for cell update
:data:`val`                   time for (objective function) value update
:data:`val_init`              time for (objective function) value update / initialization
:data:`val_penalty`           time for (objective function) value update / penalty computation
:data:`val_val`               time for (objective function) value update / value computation
:data:`val_val_cellval`       time for (objective function) value update / value computation / cell values
:data:`val_val_comp`          time for (objective function) value update / value computation / computation
:data:`val_comp`              time for (objective function) value update / computation
:data:`total`                 total time
:data:`cumtotal`              cumulative total time
============================= ============================================================

.. _variable_keys:

Variable Keys
~~~~~~~~~~~~~

The available keys for option :option:`-morphooptilogvar` are provided below.  Use :data:`iter(<factor>)`, where :data:`factor` is an integer reduction factor, to log values only at specific iteration numbers.

=========================== ======================== =====================
**Key**                     **Descriptor**           **Apply to**
:data:`iter`                iteration number         n/a
:data:`id`                  identifier               seed
:data:`x`                   x coordinate             seed
:data:`y`                   y coordinate             seed
:data:`z`                   z coordinate             seed
:data:`w`                   weight                   seed
=========================== ======================== =====================

.. _objective_function_value_keys:

Objective Function Value Keys
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The available keys for option :option:`-morphooptilogval` are provided below.  Use :data:`iter(<factor>)`, where :data:`factor` is an integer reduction factor, to log values only at specific iteration numbers.

======================= ==========================================================================
**Key**                 **Descriptor**
:data:`iter`            iteration number
:data:`val`             value
:data:`valmin`          minimal value
:data:`val0`            value, without smoothing
:data:`valmin0`         minimal value, without smoothing
:data:`val(<i>)`        :data:`i` th subvalue
:data:`val0(<i>)`       :data:`i` th subvalue, without smoothing
:data:`eps`             error on the objective function (see :option:`-morphooptistop`)
:data:`reps`            relative error on the objective function (see :option:`-morphooptistop`)
:data:`loop`            optimization loop
:data:`plateaulength`   current plateau length [#plateau]_
======================= ==========================================================================

.. _statistical_distribution_keys:

Statistical Distribution Keys
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The available keys for option :option:`-morphooptilogdis` are provided below. PDF stands for *probability density function* and CDF stands for *cumulative probability density function*.  Use :data:`iter(<factor>)`, where :data:`factor` is a reduction factor, to log values only at specific iteration numbers.

================ ==========================
**Key**          **Descriptor**
:data:`iter`     iteration number
:data:`x`        x coordinate
:data:`tarpdf`   target PDF
:data:`tarcdf`   target CDF
:data:`curpdf`   current PDF
:data:`curcdf`   current CDF
:data:`tarpdf0`  target PDF, not smoothed
:data:`tarcdf0`  target CDF, not smoothed
:data:`curcdf0`  current CDF, not smoothed
================ ==========================

.. _raster_tessellation_voxel_keys:

Raster Tessellation Voxel Keys
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The available keys for option :option:`-morphooptilogtesr` are provided below.  Values are written for each voxel used to compute the objective function.  Use :data:`iter(<factor>)`, where :data:`factor` is a reduction factor, to log values only at specific iteration numbers.

======================= =========================
**Key**                 **Descriptor**
:data:`iter`            iteration number
:data:`id`              cell identifier	
:data:`x`               x coordinate
:data:`y`               y coordinate
:data:`z`               z coordinate
:data:`dist`            distance to the cell
======================= =========================

.. _orientation_optimization_keys:

Orientation Optimization Keys
-----------------------------

Variable Keys
~~~~~~~~~~~~~

The available keys for option :option:`-orioptilogvar` are provided below.  For all orientation descriptors but :data:`quaternion`, the returned orientation are located in the fundamental region. Use :data:`iter(<factor>)`, where :data:`factor` is an integer reduction factor, to log values only at specific iteration numbers.

========================== ============================================= ===================
**Key**                    **Descriptor**                                **Apply to**
:data:`iter`               iteration number                              n/a
:data:`id`                 identifier                                    seed
:data:`rodrigues`          Rodrigues vector                              seed
:data:`euler-bunge`        Euler angles (Bunge convention)               seed
:data:`euler-kocks`        Euler angles (Kocks convention)               seed
:data:`euler-roe`          Euler angles (Roe convention)                 seed
:data:`rotmat`             Rotation matrix                               seed
:data:`axis-angle`         rotation axis / angle pair                    seed
:data:`quaternion`         quaternion                                    seed
========================== ============================================= ===================

.. _mesh_keys:

Mesh Keys
---------

Available keys for a mesh itself are provided below.  "co" stands for "cohesive".

======================= ================================================= ==============================
**Key**                 **Descriptor**                                    **Apply to**
:data:`eltnb`           element number                                    {0-3}D,co mesh
:data:`nodenb`          node number                                       {0-3}D mesh
:data:`elsetnb`         elset number                                      {0-3}D,co mesh
:data:`partnb`          partition number                                  highest-dimension mesh
:data:`x`               x coordinate                                      {0-3}D mesh
:data:`y`               y coordinate                                      {0-3}D mesh
:data:`z`               z coordinate                                      {0-3}D mesh
:data:`coo`             x, y and z coordinates                            {0-3}D mesh
:data:`length`          length                                            1D mesh
:data:`area`            surface area                                      2D mesh
:data:`vol`             volume                                            3D mesh
:data:`size`            size (length/area/volume in 1D/2D/3D)             {1-3}D mesh
:data:`step`            simulation step                                   {0-3}D,co mesh
======================= ================================================= ==============================

Available keys for mesh node, elements and element sets (of all dimensions) and points are provided below. "co" stands for "cohesive".

================================================= ===================================================================== ===================================================================
**Key**                                           **Descriptor**                                                        **Apply to**
:data:`id`                                        identifier                                                            node, {0-3}D,co elt, {0-3}D,co elset
:data:`x`                                         x coordinate                                                          node, {0-3}D,co elt, {0-3}D elset
:data:`y`                                         y coordinate                                                          node, {0-3}D,co elt, {0-3}D elset
:data:`z`                                         z coordinate                                                          node, {0-3}D,co elt, {0-3}D elset
:data:`coo`                                       x, y and z coordinates                                                node, {0-3}D,co elt, {0-3}D elset
:data:`dim`                                       lowest parent elt dimension                                           node
:data:`elset0d`                                   0D elset                                                              0D elt
:data:`elset1d`                                   1D elset                                                              1D elt
:data:`elset2d`                                   2D elset                                                              2D elt
:data:`elset3d`                                   3D elset                                                              3D elt
:data:`elsetco`                                   Cohesive elset                                                        co elt
:data:`part`                                      partition                                                             {0-3}D elt, node
:data:`group`                                     group                                                                 {0-3}D elt, {0-3}D elset
:data:`scaleid(<scale_nb>)`                       identifier of the corresponding tess cell at scale :data:`<scale_nb>` 2D elset, 3D elset
:data:`scale`                                     scale                                                                 {0-2}D elset [#multiscale_entity_mesh]_
:data:`cyl`                                       cylinder polygonization [#cyl]_                                       1D elt, 1D elset
:data:`vol`                                       volume                                                                3D elt, 3D elset
:data:`area`                                      surface area                                                          2D elt
:data:`diameq`                                    equivalent diameter                                                   {2,3}D elt, {2,3}D elset
:data:`radeq`                                     equivalent radius                                                     {2,3}D elt, {2,3}D elset
:data:`length`                                    average edge length                                                   {0-3}D elt, 1D elset
:data:`lengths`                                   edge lengths                                                          2D elt, 3D elt
:data:`elsetvol`                                  elset volume                                                          3D elt
:data:`elsetarea`                                 elset area                                                            2D elt
:data:`elsetlength`                               elset length                                                          1D elt
:data:`rr`                                        radius ratio                                                          3D elt
:data:`rrav`                                      average radius ratio                                                  3D elset
:data:`rrmin`                                     min radius ratio                                                      3D elset
:data:`rrmax`                                     max radius ratio                                                      3D elset
:data:`Osize`                                     Osize                                                                 3D elset
:data:`eltnb`                                     number of elements                                                    {0-3}D,co elset
:data:`elts`                                      elements                                                              {0-3}D,co elset
:data:`nodenb`                                    number of nodes                                                       {0-3}D,co elset
:data:`nodes`                                     nodes                                                                 {0-3}D,co elset
:data:`body`                                      body level                                                            {0-3}D elt, {0-3}D elset
:data:`elsetbody`                                 body level, relative to the elset boundary                            {1-3}D elt
:data:`domtype`                                   type of domain [#domtype]_                                            {0-2}D elt, {0-2}D elset
:data:`2dmeshp`                                   closest point of the 2D mesh                                          node, 3D elt
:data:`2dmeshd`                                   distance to :data:`2dmeshp`                                           node, 3D elt
:data:`2dmeshv`                                   vector to :data:`2dmeshp`                                             node, 3D elt
:data:`2dmeshn`                                   outgoing normal vector at :data:`2dmeshp`                             node, 3D elt
:data:`per`                                       periodic (1 if periodic, 0 otherwise)                                 {0,1}D elt, 2D elt (in 3D), {0,1}D elset, 2D elset (in 3D)
:data:`col_rodrigues`                             color in Rodrigues vector convention [#col_rodrigues]_                node
:data:`col_stdtriangle`                           color in IPF convention, cubic symmetry [#col_stdtriangle]_           node
:data:`col_stdtriangle_hexagonal`                 color in IPF convention, hexagonal symmetry [#col_stdtriangle]_       node
:data:`fiber(...)` [#fiber]_                      1 if in orientation fiber and 0 otherwise                             3D elt, 3D elset
:data:`theta`                                     disorientation angle (in degrees)                                     1D elt and elset (in 2D), 2D elt and elset (in 3D)
:data:`gos`                                       grain orientation spread [#gos]_                                      {2,3}D elset
:data:`anisogos`                                  grain orientation spread estimated from the orientation distribution  [#gos]_            {2,3}D elset
:data:`<orientation_descriptor>`                  :ref:`orientation descriptor <rotation_and_orientation_descriptors>`  2D elt (in 2D), 2D elset (in 2D), 3D elt (in 3D), 3D elset (in 3D)
:data:`step`                                      simulation step                                                       {0-3}D,co mesh
================================================= ===================================================================== ===================================================================

Variables beginning with :data:`2dmesh` are only available for statistics (options beginning with :data:`-stat` of module -M); for elements, they apply to the centroids.

.. _point_keys:

Point Keys
----------

Available keys for points are provided below.

========================== ======================================================== ====================== ======================
**Key**                    **Descriptor**                                           **Apply to**           **Require**
:data:`id`                 identifier                                               point
:data:`x`                  x coordinate                                             point
:data:`y`                  y coordinate                                             point
:data:`z`                  z coordinate                                             point
:data:`cell`               cell                                                     point                  tessellation
:data:`elt`                containing element                                       point                  mesh
:data:`elset`              containing elset                                         point                  mesh
:data:`2dmeshp`            coordinates of the closest point of the 2D mesh          point                  3D mesh
:data:`2dmeshd`            distance to :data:`2dmeshp`                              point                  3D mesh
:data:`2dmeshv`            vector to :data:`2dmeshp`                                point                  3D mesh
:data:`2dmeshn`            outgoing normal vector of the 2D mesh at :data:`2dmeshp` point                  3D mesh
========================== ======================================================== ====================== ======================

.. _simulation_results:

Simulation Results
------------------

A result of a :ref:`simulation_directory` can be invoked simply from its name.  A component of a vectorial or tensorial result can be invoked by prefixing the component to the name, as in :data:`coo1`, :data:`stress11`, etc.  For a symmetrical tensor (for which only 6 values are stored), :data:`t`, both :data:`t\<i\>\<j\>` and :data:`t\<j\>\<i\>` are valid.  The type of a result of the simulation directory is determined automatically. Tessellation results can be obtained from the cell results, by averaging or other statistical treatments.  Similarly, elset and mesh results can be obtained from the element results, by averaging or other statistical treatments.

Available results / keys for nodes are the following:

========================================== ================================================================ ==================================
**Key**                                    **Descriptor**                                                   **Apply to**
:data:`disp`                               displacement (computed from positions)                           node
========================================== ================================================================ ==================================

Available results / keys for elements sets are the following:

========================================== ================================================================ ==================================
**Key**                                    **Descriptor**                                                   **Apply to**
:data:`ori`                                average orientation                                              elset, mesh
:data:`gos`                                grain orientation spread [#gos]_                                 elset
:data:`anisogos`                           grain orientation spread computed from :data:`oridisanisoangles` elset
:data:`oridisanisoangles`                  orientation distribution principal angles                        elset, mesh
:data:`oridisanisoaxes`                    orientation distribution principal axes                          elset, mesh
:data:`oridisanisofact`                    orientation distribution factor                                  elset, mesh
:data:`odf(<var>=<value>,...)`             ODF defined at elements of orientation space (see also below)    tess, tesr, mesh, cell, elt, elset
:data:`odfn(<var>=<value>,...)`            ODF defined at nodes of orientation space (see also below)       tess, tesr, mesh
========================================== ================================================================ ==================================

The ODF (:data:`odf` or :data:`odfn`) of a tessellation or mesh is computed over orientation space (provided using :option:`-orispace`) from the orientations of the (tessellation) cells or (mesh) elsets.  The (optional) parameters are:

- :data:`theta`: the standard deviation of the kernel (in degrees);
- :data:`weight`: the weight of a cell or elset, which can be a real value or an expression based on the :ref:`tessellation_keys` (for cells) or :ref:`mesh_keys` (for elsets) --  by default, the volumes of the cells or elsets are used;
- :data:`cutoff`: the cut-off factor used to compute the ODF, which can be :data:`all` (for no cut-off) or any positive real value (default :data:`5`).


For a cell, element or elset, :data:`odf` returns the value of the ODF of the tessellation or mesh at the corresponding orientation (and simulation step).

.. _rotations_and_orientations:

Rotations and Orientations
--------------------------

.. _rotation_and_orientation_descriptors:

Rotation and Orientation Descriptors
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Rotations and orientations can be described using the following
descriptors.

========================== ======================================== =============================
Key                        Descriptor                               Number of parameters
:data:`rodrigues`          Rodrigues vector                         3
:data:`euler-bunge`        Euler angles (Bunge convention)          3
:data:`euler-kocks`        Euler angles (Kocks convention)          3
:data:`euler-roe`          Euler angles (Roe convention)            3
:data:`rotmat`             rotation matrix                          9
:data:`axis-angle`         rotation axis / angle pair               4
:data:`quaternion`         quaternion                               4
========================== ======================================== =============================

The convention can be added to the descriptor, either :data:`active` or :data:`passive`, as in :data:`rodrigues:active`.  When no convention is provided, :data:`active` is assumed.

Some options can take parameter values as argument, in which case the orientation must be expressed as :data:`<descriptor>(<parameter1>,<parameters2>,...)`. An example is :data:`rodrigues(0.1,0.2,0.3)`.

.. _ideal_orientations:

Ideal Orientations
~~~~~~~~~~~~~~~~~~

Keys are available for ideal orientations (lowercased is accepted):

======================== ====================================
**Key**                  **Miller indices**
:data:`Cube`             :math:`(0\,0\,1)[1\,0\,0]`
:data:`Goss`             :math:`(0\,1\,1)[1\,0\,0]`
:data:`U`                :math:`(1\,0\,1)[\overline{1}\,0\,1]`
:data:`45NDCube`         :math:`(0\,0\,1)[1\,\overline{1}\,0]`
:data:`S1`               :math:`(1\,2\,3)[6\,3\,\overline{4}]`
:data:`S2`               :math:`(\overline{1}\,2\,3)[6\,\overline{3}\,4]`
:data:`S3`               :math:`(1\,\overline{2}\,3)[6\,\overline{3}\,\overline{4}]`
:data:`S4`               :math:`(\overline{1}\,\overline{2}\,3)[6\,3\,4]`
:data:`Brass1`           :math:`(1\,1\,0)[1\,\overline{1}\,2]`
:data:`Brass2`           :math:`(\overline{1}\,1\,0)[1\,1\,\overline{2}]`
:data:`Copper1`          :math:`(1\,1\,2)[1\,1\,\overline{1}]`
:data:`Copper2`          :math:`(\overline{1}\,1\,2)[1\,\overline{1}\,1]`
======================== ====================================

When loading orientations from an external file, use :data:`file(<file_name>[,des=<descriptor>])` where the orientation descriptor is among those listed above and is :data:`rodrigues:active` by default.

.. _orientation_fibers:

Orientation Fibers
~~~~~~~~~~~~~~~~~~

Orientation fibers are defined by a crystal direction being parallel to a sample direction.  Depending on the context, an angular tolerance or distribution with respect to the theoretical fiber can also be defined:

- :data:`fiber(<dirc_x>,<dirc_y>,<dirc_z>,<dirs_x>,<dirs_y>,<dirs_z>)`, where (:data:`<dirc_x>`, :data:`<dirc_y>`, :data:`<dirc_z>`) is the crystal direction and (:data:`<dirs_x>`, :data:`<dirs_y>`, :data:`<dirs_z>`) is the sample direction, corresponds to an ideal orientation fiber;
- :data:`fiber(<dirc_x>,<dirc_y>,<dirc_z>,<dirs_x>,<dirs_y>,<dirs_z>,<theta>)`, where :data:`<theta>` is an angle expressed in degrees, corresponds to an orientation fiber with the angular tolerance :data:`<theta>` from the ideal fiber;
- :data:`fiber(<dirc_x>,<dirc_y>,<dirc_z>,<dirs_x>,<dirs_y>,<dirs_z>):normal(<var>=<val>)`, where :data:`<var>` can be :data:`theta` or :data:`thetam`) and :data:`<val>` is the value, corresponds to an orientation fiber with a normal (Gaussian) disorientation normal to the ideal fiber.

.. _crystal_symmetries:

Crystal Symmetries
------------------

Crystal symmetries can be described using the following descriptors.

======================== ============================================ ========================
**Key**                  **Descriptor**                               **Number of operators**
:data:`triclinic`        triclinic (Laue group :math:`\overline{1}`)  24
:data:`cubic`            cubic				              24
:data:`hexagonal`        hexagonal				      1
:data:`-1`               Laue group :math:`\overline{1}`	      1
:data:`2/m`              Laue group :math:`2/m`		              2
:data:`mmm`              Laue group :math:`mmm`		              4
:data:`4/m`              Laue group :math:`4/m`		              4
:data:`4/mmm`            Laue group :math:`4/mmm`		      8
:data:`-3`               Laue group :math:`\overline{3}`	      3
:data:`-3m`              Laue group :math:`\overline{3}m`	      6
:data:`6/m`              Laue group :math:`6/m`		              6
:data:`6/mmm`            Laue group :math:`6/mmm`		      12
:data:`m-3`              Laue group :math:`m\overline{3}`	      12
:data:`m-3m`             Laue group :math:`m\overline{3}m`            24
======================== ============================================ ========================

.. _colors_and_color_maps:

Colors and Color Maps
---------------------

.. _colors:

Colors
~~~~~~

The available colors are provided below, with their corresponding RGB channel values (ranging from 0 to 255). Any other color can be defined from the RGB channel values, under format :data:`<R_value>:\<G_value\>:\<B_value\>`.

============================= ===================
**Key**                       **RGB value**
:data:`black`                 (0, 0, 0)
:data:`red`                   (255, 0, 0)
:data:`green`                 (0, 255, 0)
:data:`blue`                  (0, 0, 255)
:data:`yellow`                (255, 255, 0)
:data:`magenta`               (255, 0, 255)
:data:`cyan`                  (0, 255, 255)
:data:`white`                 (255, 255, 255)
:data:`maroon`                (128, 0, 0)
:data:`navy`                  (0, 0, 128)
:data:`chartreuse`            (127, 255, 0)
:data:`springgreen`           (0, 255, 127)
:data:`olive`                 (128, 128, 0)
:data:`purple`                (128, 0, 128)
:data:`teal`                  (0, 128, 128)
:data:`gray`                  (128, 128, 128)
:data:`deepskyblue`           (0, 191, 255)
:data:`lawngreen`             (124, 252, 0)
:data:`darkgray`              (64, 64, 64)
:data:`orangered`             (255, 69, 0)
:data:`silver`                (192, 192, 192)
:data:`snow`                  (255, 250, 250)
:data:`darkred`               (139, 0, 0)
:data:`darkblue`              (0, 0, 139)
:data:`darkorange`            (255, 140, 0)
:data:`azure`                 (240, 255, 255)
:data:`ghostwhite`            (248, 248, 255)
:data:`ivory`                 (255, 255, 240)
:data:`mediumblue`            (0, 0, 205)
:data:`lightpink`             (255, 182, 193)
:data:`mintcream`             (245, 255, 250)
:data:`indigo`                (75, 0, 130)
:data:`lightcoral`            (240, 128, 128)
:data:`pink`                  (255, 192, 203)
:data:`coral`                 (255, 127, 80)
:data:`salmon`                (250, 128, 114)
:data:`floralwhite`           (255, 250, 240)
:data:`aquamarine`            (127, 255, 212)
:data:`lemonchiffon`          (255, 250, 205)
:data:`gold`                  (255, 215, 0)
:data:`darkgreen`             (0, 100, 0)
:data:`orange`                (255, 165, 0)
:data:`aliceblue`             (240, 248, 255)
:data:`lightcyan`             (224, 255, 255)
:data:`lightyellow`           (255, 255, 224)
:data:`darkmagenta`           (139, 0, 139)
:data:`darkcyan`              (0, 139, 139)
:data:`peru`                  (205, 133, 63)
:data:`steelblue`             (70, 130, 180)
:data:`lavenderblush`         (255, 240, 245)
:data:`seashell`              (255, 245, 238)
:data:`mediumspringgreen`     (0, 250, 154)
:data:`darkslateblue`         (72, 61, 139)
:data:`darkgoldenrod`         (184, 134, 11)
:data:`lightsalmon`           (255, 160, 122)
:data:`bisque`                (255, 228, 196)
:data:`lightskyblue`          (135, 206, 250)
:data:`lightgoldenrodyellow`  (250, 250, 210)
:data:`honeydew`              (240, 255, 240)
:data:`cornsilk`              (255, 248, 220)
:data:`peachpuff`             (255, 218, 185)
:data:`whitesmoke`            (245, 245, 245)
:data:`tomato`                (255, 99, 71)
:data:`slategray`             (112, 128, 144)
:data:`hotpink`               (255, 105, 180)
:data:`oldlace`               (253, 245, 230)
:data:`blanchedalmond`        (255, 235, 205)
:data:`darkkhaki`             (189, 183, 107)
:data:`moccasin`              (255, 228, 181)
:data:`darkturquoise`         (0, 206, 209)
:data:`mediumseagreen`        (60, 179, 113)
:data:`mediumvioletred`       (199, 21, 133)
:data:`violet`                (238, 130, 238)
:data:`greenyellow`           (173, 255, 47)
:data:`papayawhip`            (255, 239, 213)
:data:`darkseagreen`          (143, 188, 143)
:data:`rosybrown`             (188, 143, 143)
:data:`deeppink`              (255, 20, 147)
:data:`saddlebrown`           (139, 69, 19)
:data:`darkviolet`            (148, 0, 211)
:data:`dodgerblue`            (30, 144, 255)
:data:`lightslategray`        (119, 136, 153)
:data:`burlywood`             (222, 184, 135)
:data:`navajowhite`           (255, 222, 173)
:data:`linen`                 (250, 240, 230)
:data:`mediumslateblue`       (123, 104, 238)
:data:`turquoise`             (64, 224, 208)
:data:`skyblue`               (135, 206, 235)
:data:`mediumturquoise`       (72, 209, 204)
:data:`beige`                 (245, 245, 220)
:data:`mistyrose`             (255, 228, 225)
:data:`tan`                   (210, 180, 140)
:data:`antiquewhite`          (250, 235, 215)
:data:`thistle`               (216, 191, 216)
:data:`limegreen`             (50, 205, 50)
:data:`darksalmon`            (233, 150, 122)
:data:`lightsteelblue`        (176, 196, 222)
:data:`royalblue`             (65, 105, 225)
:data:`palegreen`             (152, 251, 152)
:data:`crimson`               (220, 20, 60)
:data:`wheat`                 (245, 222, 179)
:data:`mediumorchid`          (186, 85, 211)
:data:`lavender`              (230, 230, 250)
:data:`khaki`                 (240, 230, 140)
:data:`lightgreen`            (144, 238, 144)
:data:`paleturquoise`         (175, 238, 238)
:data:`darkslategray`         (47, 79, 79)
:data:`darkorchid`            (153, 50, 204)
:data:`seagreen`              (46, 139, 87)
:data:`yellowgreen`           (154, 205, 50)
:data:`blueviolet`            (138, 43, 226)
:data:`palevioletred`         (219, 112, 147)
:data:`olivedrab`             (107, 142, 35)
:data:`mediumpurple`          (147, 112, 219)
:data:`sandybrown`            (244, 164, 96)
:data:`darkolivegreen`        (85, 107, 47)
:data:`mediumaquamarine`      (102, 205, 170)
:data:`slateblue`             (106, 90, 205)
:data:`palegoldenrod`         (238, 232, 170)
:data:`forestgreen`           (34, 139, 34)
:data:`midnightblue`          (25, 25, 112)
:data:`lightseagreen`         (32, 178, 170)
:data:`lightgray`             (211, 211, 211)
:data:`orchid`                (218, 112, 214)
:data:`cornflowerblue`        (100, 149, 237)
:data:`sienna`                (160, 82, 45)
:data:`firebrick`             (178, 34, 34)
:data:`powderblue`            (176, 224, 230)
:data:`indianred`             (205, 92, 92)
:data:`dimgray`               (105, 105, 105)
:data:`lightblue`             (173, 216, 230)
:data:`chocolate`             (210, 105, 30)
:data:`brown`                 (165, 42, 42)
:data:`goldenrod`             (218, 165, 32)
:data:`gainsboro`             (220, 220, 220)
:data:`plum`                  (221, 160, 221)
:data:`cadetblue`             (95, 158, 160)
============================= ===================

.. _color_maps:

Color Maps
~~~~~~~~~~

.. _color_map_for_integer_values:

Color Map for Integer Values
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The *color map* or *palette* used to represent integer values is defined from the above color list, by excluding colors of brightness below 0.2 and above 0.8.  The brightness is defined as the average of the channel values divided by 255.  The resulting list of colors is: \(1) :data:`red`, (2) :data:`green`, (3) :data:`blue`, (4) :data:`yellow`, (5) :data:`magenta`, (6) :data:`cyan`, (7) :data:`chartreuse`, (8) :data:`springgreen`, (9) :data:`olive`, (10) :data:`purple`, (11) :data:`teal`, (12) :data:`gray`, (13) :data:`deepskyblue`, (14) :data:`lawngreen`, (15) :data:`darkgray`, (16) :data:`orangered`, (17) :data:`silver`, (18) :data:`darkorange`, (19) :data:`mediumblue`, (20) :data:`indigo`, (21) :data:`lightcoral`, (22) :data:`coral`, (23) :data:`salmon`, (24) :data:`aquamarine`, (25) :data:`gold`, (26) :data:`orange`, (27) :data:`darkmagenta`, (28) :data:`darkcyan`, (29) :data:`peru`, (30) :data:`steelblue`, (31) :data:`mediumspringgreen`, (32) :data:`darkslateblue`, (33) :data:`darkgoldenrod`, (34) :data:`lightsalmon`, (35) :data:`lightskyblue`, (36) :data:`tomato`, (37) :data:`slategray`, (38) :data:`hotpink`, (39) :data:`darkkhaki`, (40) :data:`darkturquoise`, (41) :data:`mediumseagreen`, (42) :data:`mediumvioletred`, (43) :data:`violet`, (44) :data:`greenyellow`, (45) :data:`darkseagreen`, (46) :data:`rosybrown`, (47) :data:`deeppink`, (48) :data:`saddlebrown`, (49) :data:`darkviolet`, (50) :data:`dodgerblue`, (51) :data:`lightslategray`, (52) :data:`burlywood`, (53) :data:`mediumslateblue`, (54) :data:`turquoise`, (55) :data:`skyblue`, (56) :data:`mediumturquoise`, (57) :data:`tan`, (58) :data:`limegreen`, (59) :data:`darksalmon`, (60) :data:`lightsteelblue`, (61) :data:`royalblue`, (62) :data:`palegreen`, (63) :data:`crimson`, (64) :data:`mediumorchid`, (65) :data:`khaki`, (66) :data:`lightgreen`, (67) :data:`darkslategray`, (68) :data:`darkorchid`, (69) :data:`seagreen`, (70) :data:`yellowgreen`, (71) :data:`blueviolet`, (72) :data:`palevioletred`, (73) :data:`olivedrab`, (74) :data:`mediumpurple`, (75) :data:`sandybrown`, (76) :data:`darkolivegreen`, (77) :data:`mediumaquamarine`, (78) :data:`slateblue`, (79) :data:`forestgreen`, (80) :data:`midnightblue`, (81) :data:`lightseagreen`, (82) :data:`orchid`, (83) :data:`cornflowerblue`, (84) :data:`sienna`, (85) :data:`firebrick`, (86) :data:`indianred`, (87) :data:`dimgray`, (88) :data:`chocolate`, (89) :data:`brown`, (90) :data:`goldenrod`, (91) :data:`plum` and (92) :data:`cadetblue`.

.. _color_map_for_real_values:

Color Maps for Real Values
^^^^^^^^^^^^^^^^^^^^^^^^^^

The color *map* used to represent real values is smooth and obtained by interpolation between nominal colors. `Tinycolormap <https://github.com/yuki-koyama/tinycolormap>`_ is used to generate standard color maps, and the default is :data:`viridis`. The color maps are

================= ===================================================
**Key**           **Color bar**
viridis           .. image:: imgs/color-maps-real/viridis.png
cividis           .. image:: imgs/color-maps-real/cividis.png
magma             .. image:: imgs/color-maps-real/magma.png
inferno           .. image:: imgs/color-maps-real/inferno.png
plasma            .. image:: imgs/color-maps-real/plasma.png
parula            .. image:: imgs/color-maps-real/parula.png
heat              .. image:: imgs/color-maps-real/heat.png
hot               .. image:: imgs/color-maps-real/hot.png
jet               .. image:: imgs/color-maps-real/jet.png
gray              .. image:: imgs/color-maps-real/gray.png
github            .. image:: imgs/color-maps-real/github.png
================= ===================================================

Alternatively, a custom color map can be provided as :data:`custom(<color1>,<color2>,...)`.  Neper's legacy color map (version :math:`< 4`) is :data:`custom(blue,cyan,yellow,green)` and can also be obtained using :data:`legacy`:

================= ===================================================
legacy            .. image:: imgs/color-maps-real/legacy.png
================= ===================================================

Finally, it is possible to gradually fade the start of a color map, to make it starts with white.  This can be done using the :data:`fade` modifier, following the syntax :data:`<colormap>:fade[(threshold)]`.  The threshold ranges from 0 to 1 and is equal to 0.1 by default.  Fading is applied linearly from 0 (full fading) to the threshold (no fading).  Examples are show below:

================= ===================================================
viridis:fade      .. image:: imgs/color-maps-real/viridis-fade.png
viridis:fade(0.2) .. image:: imgs/color-maps-real/viridis-fade0p2.png
================= ===================================================

.. [#muparser_doc] Taken from the :data:`muparser` `documentation <http://beltoforion.de/article.php?a=muparser>`_

.. [#multiscale_entity] Applies only to a 3D tessellation and relevant for multiscale tessellations.  The scale of an entity (vertex, edge or face) is the scale at which the entity was created, and it ranges from 0 (for domain entities) to the number of scales of the tessellation (for the last created entities).  The scale of a cell is determined based on the number of subcells of its parent cell.

.. [#multiscale_entity_mesh] Applies only to a 3D mesh and relevant for meshes of multiscale tessellations.  The scale of an elset is equal to the scale of its corresponding tessellation entity  [#multiscale_entity]_.

.. [#equivalent_diameter] Equivalent diameter = diameter of the circle of equivalent area/volume in 2D/3D

.. [#sphericity] Sphericity of a polyhedron = ratio of the surface area of the sphere of equivalent volume to the surface area of the polyhedron

.. [#circularity] Circularity of a polygon = ratio of the perimeter of the circle of equivalent area to the perimeter of the polygon

.. [#convexity] Convexity of a polyhedron (face) = ratio of the volume (surface area) of the polyhedron (face) to the volume (surface area) of the convex hull of the polyhedron (face)

.. [#gos] Grain orientation spread = average disorientation (angle) with respect to the average orientation

.. [#face_equation] A face equation is specified by the parameters :math:`d`, :math:`a`, :math:`b` and :math:`c`, with the equation being: :math:`a x + b y + c z = d`.  The vector :math:`(a, b, c)` is pointing outwards of the polyhedron.

.. [#nseeds] If a polyhedron has no neighbor on a face, a negative value is returned.

.. [#lam] In the case of a lamellar tessellation with several lamella widths, :data:`lam` stands for the actual lamella width of the cell (starting from 1).

.. [#mode] In the case of a multimodal tessellation (e.g.in terms of cell size), :data:`mode` stands for the actual mode (starting from 1).  Otherwise, it is equal to 1.

.. [#JMPS2015] R Quey, JH Driver, PR Dawson, Intra-grain orientation distributions in hot-deformed aluminium: orientation dependence and relation to deformation mechanisms, Journal of the Mechanics and Physics of Solids, vol. 84, pp. 506-524, 2015.

.. [#deltas] For Gaussian distributions, they are equal to 1.349 times the values of :data:`oridisanisoangles`.

.. [#plateau] Number of iterations during which the objective function does not decrease

.. [#domtype] 0 if on a domain vertex, 1 if on a domain edge and 2 if on a domain face

.. [#col_rodrigues] Applies to a mesh of Rodrigues space

.. [#col_stdtriangle] Applies to a mesh of the stereographic triangle

.. [#cyl] 1 if polygonize the circular part of a cylindrical domain and 0 otherwise.
.. [#fiber] See :ref:`orientation_fibers`.
