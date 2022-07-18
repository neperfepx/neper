.. |translate| replace:: :data:`translate(<dist_x>,<dist_y>,<dist_z>)`: translate by distances :data:`<dist_x>`, :data:`<dist_y>` and :data:`<dist_z>` along :data:`x`, :data:`y` and :data:`z`, respectively
.. |rotate| replace:: :data:`rotate(<axis_x>,<axis_y>,<axis_z>,<angle>)`: rotate about the center and by an axis/angle pair (angle expressed in degrees)
.. |scale| replace:: :data:`scale(<fact_x>,<fact_y>,<fact_z>)`: scale by :data:`<fact_x>`, :data:`<fact_y>` and :data:`<fact_z>` along :data:`x`, :data:`y` and :data:`z`, respectively
.. |crop| replace:: :data:`crop(<xmin>,<xmax>,<ymin>,<ymax>,<zmin>,<zmax>)`: crop a region of a raster tessellation, where :data:`<xmin>`, :data:`<xmax>`, :data:`<ymin>`, :data:`<ymax>`, :data:`<zmin>` and :data:`<zmax>` are the minimum and maximum positions along :data:`x`, :data:`y` and :data:`z`, respectively. For 2D raster tessellations, the :data:`z` values can be omitted
.. |rasterscale| replace:: :data:`rasterscale(<fact>)` or :data:`rasterscale(<fact_x>,<fact_y>,<fact_z>)`: : scale the number of voxels of a raster tessellation, where :data:`factor` is the scaling factor that applies in the three directions, and :data:`<fact_x>`, :data:`<fact_y>` and :data:`<fact_z>` are the scaling factors along :data:`x`, :data:`y` and :data:`z`, respectively

.. index::
   single: -T

.. _neper_t:

Tessellation Module (-T)
========================

Module -T is the module for generating *tessellations* and *multiscale tessellations* of a finite *domain* of space, in 2D or 3D.  The domain is generally convex, although non-convex shapes can also be obtained.  Periodicity and semi-periodicity conditions can be prescribed.  Module -T also enables the *regularization* of the tessellations for meshing with high quality elements. The tessellations are provided in scalar (vectorial) or raster formats.  The scalar format described the tessellation cells using sets of vertices, edges and faces, while the raster format uses a regular raster of voxels (similarly to an EBSD map). Module -T also generates crystal orientations for the cells.

*Tessellations* can be generated from various types of morphological cell properties (option :option:`-morpho`).  Several predefined properties are available, such as those obtained by grain growth in metals (which are described by cell size and sphericity (circularity, in 2D) distributions).  Custom properties can be specified using various metrics, including the size and sphericity (circularity, in 2D), the centroid or even the actual shape (using a raster tessellation), in terms of distributions or individual cell values.  Standard analytical distributions are included, and custom numerical distributions can be read from a file.  Global morphological properties, such as a cell aspect ratio or a columnar axis, can also be specified.  The generated *tessellations* are *general convex-cell tessellations* represented as Laguerre (or Voronoi) tessellations whose seed attributes are set by optimization to obtain the desired cell properties [#convex]_.  Of course, it is also possible to generate standard tessellations (e.g. Poisson-Voronoi or regular tessellations).  Cell *groups* can be defined to represent, for example, the different phases of a multiphased polycrystalline material (option :option:`-group`).

*Multiscale tessellations* are characterized by the subdivision of the cells of a primary tessellation into secondary tessellations (and so on) and are obtained by combining into one, using :data:`::`, the option arguments that apply at the successive scales.  The same value can be used for defining the tessellations at a given scale, or different values can be loaded using :data:`msfile(<file_name>)`, where :data:`<file_name>` is a :ref:`multiscale cell file <multiscale_cell_file>`). So, all capabilities available for generating a standard (single-scale) tessellations are available for generating the tessellations at the different scales of a multiscale tessellation.

The *domain* of space in which the tessellation is created can be of any convex shape. In 3D, cuboidal, cylindrical and spherical shapes (and a few other, exotic shapes) are directly supported while other morphologies can be defined from a set of planes (option :option:`-domain`).  Non convex domain shapes can be obtained by cutting the tessellation by different geometrical primitives once generated (option :data:`-transform cut` [#cut]_).  Periodicity or semi-periodicity conditions can be applied to the tessellation (option :option:`-periodicity`).

*Crystal orientations* can be randomly distributed (according to a uniform distribution function), either in the 3D space or along a specific orientation fiber, or uniformly distributed (also according to a uniform distribution function, option :option:`-ori`).  Uniform crystal orientation distributions ensure that all possible crystal orientations are equally represented (no orientation clustering).  Crystal orientations can be written according to different descriptors (option :option:`-oridescriptor`).  It is also possible to define an analytical orientation spread for the cells (option :option:`-orispread`).

*Regularization* can be applied to the tessellations and consists of removing their small edges and faces (option :option:`-regularization`) which would otherwise be detrimental to generating meshes with high quality elements with :ref:`module -M <neper_m>`).

*Output files* describe the tessellation either at the :ref:`scalar format (.tess) <tess_file>` or at the :ref:`raster format (.tesr) <tesr_file>`. A *raster tessellation* also has all required fields to describe data obtained by 2D or 3D orientation mapping methods (such as EBSD).   Tessellation files are input files of the :ref:`neper_m` and the :ref:`neper_v`, and can also be exported as a :ref:`simulation_directory`, which is input to the :ref:`neper_s`, for post-processing.  Third-party software file formats are also available.


The methods implemented for tessellation generation are described in [CMAME2011]_, [CMAME2018]_ and [JAC2018]_.

Here is what a typical run of module -T looks like:

.. code-block:: console

  $ neper -T -n 10 -reg 1

  ========================    N   e   p   e   r    =======================
  Info   : A software package for polycrystal generation and meshing.
  Info   : Version 4.0.0
  Info   : Built with: gsl|muparser|opengjk|openmp|nlopt|libscotch (full)
  Info   : Running on 8 threads.
  Info   : <https://neper.info>
  Info   : Copyright (C) 2003-2020, and GNU GPL'd, by Romain Quey.
  Info   : No initialization file found (`/home/rquey/.neperrc').
  Info   : ---------------------------------------------------------------
  Info   : MODULE  -T loaded with arguments:
  Info   : [ini file] (none)
  Info   : [com line] -n 10 -reg 1
  Info   : ---------------------------------------------------------------
  Info   : Reading input data...
  Info   : Creating domain...
  Info   : Creating tessellation...
  Info   :   - Setting seeds...
  Info   :   - Generating crystal orientations...
  Info   :   - Running tessellation...
  Info   : Regularizing tessellation...
  Info   :   - loop 2/2: 100% del=14
  Info   : Writing results...
  Info   :     [o] Writing file :data:`n10-id1.tess'...
  Info   :     [o] Wrote file :data:`n10-id1.tess'.
  Info   : Elapsed time: 0.036 secs.
  ========================================================================

Arguments
---------

Input Data
~~~~~~~~~~

.. option:: -n <cell_number>

  Specify the number of cells of the tessellation, which can be:

  - an integer value or any expression based on the :ref:`tessellation_keys`;
  - :data:`from_morpho` to set the value from the morphology (option :option:`-morpho`).

  **Default value**: -.

.. option:: -id <identifier>

  Specify the identifier of the tessellation, which can be an integer value or any expression based on the :ref:`tessellation_keys`.

  The identifier is used as seed of the random number generator to compute the (initial) seed positions.

  **Default value**: :data:`1`.

.. option:: -dim <dimension>

  Specify the dimension of the tessellation, which can be :data:`2` or :data:`3`.

  **Default value**: :data:`3`.

.. option:: -domain <domain_morphology[:transformation]>

  Specify the domain morphology and, optionally, a transformation.

  The domain morphology can be:

  - :data:`cube(<size_x>,<size_y>,<size_z>)`: a cuboidal shape (in 3D), or :data:`square(<size_x>,<size_y>)`: a rectangular shape (in 2D);
  - :data:`cylinder(<height>,<diameter>[,<facet_nb>])`: a cylindrical shape;
  - :data:`sphere(<diameter>[,<facet_nb>])`: a spherical shape (in 3D), or :data:`circle(<diameter>[,<facet_nb>])`: a circular shape (in 2D);

    :data:`facet_nb` is the number of facets used to described a curved surface.

  - :data:`planes(<file_name>)`: an arbitrary (non-degenerated) convex 3D shape, where :data:`<file_name>` contains the total number of planes and then, for each plane, the 4 parameters of its equation: :data:`d`, :data:`a`, :data:`b` and :data:`c,` successively for an equation of the form :math:`a\,x+b\,y+c\,z=d`, and where the plane normal :math:`(a,\,b,\,c)` is an outgoing vector of the domain.  For the unit cube, the file can be as follows::

      6
      0 -1  0  0
      0  0 -1  0
      0  0  0 -1
      1  1  0  0
      1  0  1  0
      1  0  0  1

  - :data:`cell(<file_name>,<cell_id>)`: a tessellation cell, where :data:`<file_name>` is the tessellation file and :data:`<cell_id>` is the cell identifier;

  - :data:`rodrigues(<crysym>)`: a Rodrigues space fundamental region, where :data:`<crysym>` is the :ref:`Crystal Symmetry <crystal_symmetries>`;

  - :data:`euler-bunge(<size_x>,<size_y>,<size_z>)`: the Euler space (Bunge convention), where :data:`<size_x>`, :data:`<size_y>` and :data:`<size_z>` are the space dimensions (in degrees or radians [#euler-bunge]_);

  - :data:`stdtriangle(<segment_nb>)`: a standard stereographic triangle, where :data:`<segment_nb>` is the number of segments used to describe the [011]--[111] line.

  The transformation can be:

  - |translate|;
  - |rotate|;
  - :data:`split(<dir>)`: splitting the domain in half along direction :data:`<dir>` (:data:`x`, :data:`y` or :data:`z`), which can be used to apply symmetries.

  For a 2D tessellation, the axis parameters can be omitted in :data:`rotate` (default :data:`z`), and the :math:`z` component can be omitted in :data:`scale` (n/a) and :data:`translate` (default :data:`0`).

  An example is :data:`sphere(1,1,0,0):translate(-0.5-0.5-0.5):scale(0.512)`.

  **Default value**: :data:`cube(1,1,1)` in 3D and :data:`square(1,1)` in 2D.

.. option:: -periodicity <periodicity>

  Specify the periodicity conditions that apply to the domain (and therefore to the tessellation), which can be:

  - :data:`0` or :data:`none`: no periodicity;
  - :data:`1` or :data:`all`: full periodicity;
  - a list of periodicity directions, among :data:`x`, :data:`y` and :data:`z` and combined with :data:`,`: semi-periodicity.

  **Default value**: :data:`0`.

Is it also possible to load a tessellation or a raster tessellation from a file:

.. option:: -loadtess <tess_file>

  Load a tessellation from a :ref:`tess_file`.

  **Default value**: -.

.. option:: -loadtesr <tesr_file>[:<transformation1>][:<transformation2>:...]

  Load a raster tessellation from a :ref:`tesr_file` and, optionally, apply transformations.

  The transformations can be:

  - |crop|

  - |rasterscale|. For 2D raster tessellations, the :math:`z` value can be omitted.

  **Default value**: -*

Finally, it is possible to load a set of points.

.. option:: -loadpoint <point_file>

  Load points from a file formatted as a :ref:`position_file`.

  .. important::

    These points are used only for statistics, in option :option:`-statpoint`, and are *not* used for tessellation.

  **Default value**: -*

Morphology Options
~~~~~~~~~~~~~~~~~~

These options can be used to set the cell morphology.

.. _morpho_voronoi:

.. option:: -morpho <morphology>

  Specify morphological properties of the cells.  This can be done in different ways:

  **Special morphological properties** can be (mutually-exclusive):

  - :data:`voronoi`: standard Poisson-Voronoi tessellation;

  - :data:`graingrowth` or :data:`gg`: grain-growth morphology, which corresponds to a wider grain size distribution and higher grain sphericities than in a Voronoi tessellation.  It actually is an alias for :data:`diameq:lognormal(1,0.35),1-sphericity:lognormal(0.145,0.03)` in 3D and :data:`diameq:lognormal(1,0.42),1-circularity:lognormal(0.100,0.03)` in 2D, which are described below.  The :data:`graingrowth(<mean>)` and :data:`gg(<mean>)` variants can be used to provide an absolute mean grain size, :data:`<mean>` (in which case :data:`-n from_morpho` must be used, as described below).

  - :data:`centroidal`: a centroidal tessellation [#centroidal]_.  It actually is an alias for :data:`centroid:seed`, which is described below.

  - :data:`cube(<N>)` or :data:`square(<N>)`: regular tessellations into cubic or square cells, where :data:`<N>` is the number of cells along a direction, or :data:`cube(<N1>,<N2>,<N3>)` / :data:`square(<N1>,<N2>)` for a regular tessellation into cubic / square cells, where :data:`<N1>`, :data:`<N2>` and :data:`<N3>` are the number of cells along the three directions;

  - :data:`tocta(<N>)`: regular tessellation into truncated octahedra, where :data:`<N>` is the number of cells along a direction;

  - :data:`lamellar(w=<width>[,v=<normal>][,pos=<pos>][,reps=<reps>])`: lamellar morphology, where :data:`<width>` is the absolute lamella width or a series of absolute lamella widths combined with :data:`:`, and :data:`<normal>` is the lamella plane normal and can be:

    - :data:`random`: randomly-distributed normals taken from a uniform distribution (the default);
    - :data:`(<dir_x>,<dir_y>,<dir_z>)`: a specific direction of space, (:data:`dir_x`, :data:`dir_y`, :data:`dir_z`);
    - :data:`crysdir(<crysdir_x>,<crysdir_y>,<crysdir_z>)`: a specific direction of the parent crystal, (:data:`<crysdir_x>`, :data:`<crysdir_y>`, :data:`<crysdir_z>`).

    :data:`pos` is the position of the first lamella and can be:

    - :data:`random`: random position (the default);
    - :data:`optimal`: optimal position, i.e. so that lamellae at the start *and* end of the domain are of lengths as close as possible to nominal (along direction :data:`<dir>`);
    - :data:`start`: first lamella starting full-width from the start point of the domain (along direction  :data:`<dir>`);
    - :data:`half`: first lamella starting half-width from the start point of the domain (along direction  :data:`<dir>`).
    - :data:`<factor>`: lamella starting with a width equal to :data:`<factor>` times the nominal width (between :data:`0` and :data:`1`), from the start point of the domain (along direction  :data:`<dir>`).

    :data:`reps` is a relative tolerance on the width of the last lamella (default :data:`1e-2`) - a lamella is allowed to be larger than nominal, within the specified relative tolerance, to avoid the occurence of unrealistically thin lamella.

    In the case of a multiscale tessellation, a :ref:`multiscale cell file <multiscale_cell_file>` can be provided as value of :data:`w`, :data:`v`, and :data:`pos`.

  **Custom morphological properties** can be defined using :data:`<property>:\<value\>`. The properties and the possible corresponding values can be:

  - :data:`size`: the size (volume in 3D and area in 2D) [#size_diameq]_;

  - :data:`diameq`: the equivalent diameter [#size_diameq]_;

  - :data:`sphericity`: the sphericity, and :data:`1-sphericity`: 1 :math:`-` the sphericity (or :data:`circularity` and :data:`1-circularity`). [#sphericity-circularity]_ [#1-sphericity]_

    All of :data:`size`, :data:`diameq` and :data:`sphericity` (and their variants) can be defined by :ref:`statistical distributions <statistical_distributions>` or cell by cell.  If the number of cells is defined using option :option:`-n`, the :data:`size` or :data:`diameq` distribution is scaled to get the specified number of cells.  At the opposite, if :data:`-n from_morpho` is used, the number of cells is determined from the :data:`size` or :data:`diameq` distribution.  An interval of possible values can also be provided using :data:`interval(<min>,<max>)`.  Cell-by-cell values can be provided using :data:`file(<file_name>)`, where :data:`<file_name>` contains the cell values.  A unique (numeral) value to be assigned to all cells can also be provided directly.

  - :data:`centroid` for the centroid;

  - :data:`centroidtol` for the centroid with a tolerance (see below for the format; centroids with a tolerance more than 1000 times as high as the minimum tolerance are simply disregarded);

  - :data:`centroidsize` for combined centroid and size, and :data:`centroiddiameq` for combined centroid and equivalent diameter.

    All of the :data:`centroid*` properties must be defined cell by cell, and provided using :data:`file(<file_name>)`.  For :data:`centroid`, the file must be a :ref:`position file <position_file>` while, for more complete properties, the additional data should be provided on the following columns.

  - :data:`tesr` for cells of a raster tessellation.  It must be defined by a raster tessellation, provided using :data:`file(<file_name>)`.  If :option:`-n` is set to :data:`from_morpho`, the number of cells is set to the number of cells of the raster tessellation.

  **Global cell properties** can be defined as follows (mutually-exclusive):

  - :data:`columnar(<dir>)` for a columnar morphology along direction :data:`<dir>`, where :data:`<dir>` can be :data:`x`, :data:`y` or :data:`z`;

  - :data:`bamboo(<dir>)` for a bamboo morphology along direction :data:`<dir>`, where :data:`<dir>` can be :data:`x`, :data:`y` or :data:`z`;

  - :data:`aspratio(<r_x>,<r_y>,<r_z>)`, where :data:`r_x`, :data:`r_y` and :data:`r_z` represent relative length along the :data:`x`, :data:`y` and :data:`z` directions. For a 2D tessellation, :data:`r3` can be omitted.  When provided, other properties, such as the equivalent diameter or the sphericity (circularity, in 2D), are considered to apply to the cells as if they had no aspect ratio.

  **A tessellation file** (:file:`.tess`) can be loaded using :data:`file(<file_name>)`.

  To specify several properties, combine them with :data:`,` (centroids and sizes / equivalent diameters should be seen as one property and specified with :data:`centroidsize` / :data:`centroiddiameq`).

  **Default value**: :data:`voronoi`.


.. option:: -morphooptiini <seed_attributes>

  Specify the initial positions and/or weights of the seeds.

  The general form of the argument is :data:`coo:\<coo_definition\>,weight:\<weight_definition\>`.  Different values of :data:`<coo_definition>` and :data:`<weight_definition>` are available, depending on the value of option :option:`-morpho`:

  - :data:`<coo_definition>` can be:

    - :data:`random`: random positions;
    - :data:`packing`: positions set by (rough) dense sphere packing using the weights as sphere radii;
    - :data:`centroid`: cell centroids;
    - :data:`LLLFP2011`: Lyckegaard et al.'s method [LLLFP2011]_;
    - :data:`file(<file_name>)`: values to load from a :ref:`position_file` or a :ref:`tess_file` (only the seed coordinates are considered).

    The default depends on the value of option :option:`-morpho`: for :data:`voronoi`, it is :data:`random`, for a cell-size statistical distribution, it is :data:`none`, and for cell-based coordinate values (including :data:`-morpho tesr`), it is :data:`centroid`.

  - :data:`<weight_definition>` can be:

    - a real value or an expression based on the :ref:`tessellation_keys`, especially :data:`radeq`, :data:`diameq`, :data:`avradeq` and :data:`avdiameq`;
    - :data:`file(<file_name>)`: values from a :ref:`data_file` or a :ref:`tess_file` (only the seed weights are considered).

    The default depends on the value of option :data:`-morpho`: for :data:`voronoi`, it is :data:`0`, for a cell-size statistical distribution, it is :data:`avradeq`, and for cell-based size values (including :data:`-morpho tesr`) , it is :data:`radeq`.

  Alternatively, :data:`file(<file_name>)` can be used to load the seed coordinates and weights from a unique :ref:`tess_file` (thereby replicating the tessellation).

  **Default value**: :data:`default`.

.. option:: -morphooptiobjective <objective_function> (secondary option)

  Specify the objective function.  The general form of the argument is :data:`<prop1>:<objective_function1>,<prop2>:<objective_function2>,...`, where :data:`<prop#>` are properties as defined in option :option:`-morpho`, and :data:`<objective_function#>` are their objective functions.  An objective function depends on the property and its value.

  **Properties defined by a statistical distribution** (which can be :data:`size`, :data:`diameq`, :data:`sphericity` or :data:`1-sphericity` (or :data:`circularity` and :data:`1-circularity`)) can take values of:

  - :data:`chi2`: Chi-square test;
  - :data:`ks`: Kolmogorov-Smirnov test;
  - :data:`kuiper` (Kuiper's test);
  - :data:`cmv` (Cramér-von Mises test);
  - :data:`ad` (Anderson-Darling test);
  - :data:`FL2` (:math:`L^2\text{-norm}` on :math:`F`);
  - :data:`FL2w` (weighted :math:`L^2`-norm on :math:`F`) [CMAME2018]_;
  - :data:`FL2wu` (weighted :math:`L^2`-norm on :math:`F` corresponding to :data:`FL2w` for a unimodal distribution).

  The default value is :data:`FL2w`.

  **The** :data:`centroid` **property** can take values of a Minkowski distance between the seeds and centroids is used, and can be :data:`L1`, :data:`L2` or :data:`Linf`.

  **The** :data:`tesr` **property** accepts an objective function which includes several factors.

    First, preprocessing operations to the raster tessellation can be applied using :data:`transform(<operation1>,<operation2>,...)`, where :data:`<operation#>` can be:

      - :data:`scale` to scale the tessellation to correct for a global cell elongation;
      - :data:`rasterscale` to scale the raster itself to correct for a global voxel elongation (which may result from operation :data:`scale`).

    Second, control points can be defined using :data:`pts(<def1>,<def2>,...)`, where :data:`<def#>` can be:

      - :data:`region=\<region\>`, where :data:`region` can be :data:`surf` for surface voxels or :data:`all` for all voxels;
      - :data:`res=\<res\>`, where :data:`<res>` is the resolution, i.e. the average number of control points along a direction of a grain.

    Third, the expression of the objective function *per se* can be specified using :data:`val(<expr>)`, where :data:`expr` can be (mutually exclusive):

      - :data:`bounddist`: minimize the distance between the raster tessellation and tessellation cell boundaries;
      - :data:`intervol`: maximize the volume of intersection between the raster tessellation and tessellation (both provide similar results).

    To define the objective function, combine the above factors using :data:`+`.  The default value is :data:`pts(region=surf,res=5)+val(bounddist)`.  A penalty is automatically added to the objective function in the case where cells are empty (including when :data:`-transform cut` is used).

  **The** :data:`general` **variable**, finally, can be used to specify how the different components of the objective function are combined into the objective function (in the case where several properties are specified), using the syntax :data:`general:\<norm\>`, where :data:`norm` can be :data:`L1`, :data:`L2` or :data:`Linf`; the default is :data:`L2`.

  Examples are :data:`diameq:FL2,1-sphericity:FL2`, :data:`centroid:L1` or :data:`tesr:pts(region=all,res=5)+val(intervol)`.

  **Default value**: :data:`default`.

.. option:: -morphooptidof <dof1>,<dof2>,... (secondary option)

  Specify the degrees of freedom, which can be :data:`x`, :data:`y` and :data:`z` for the 3 coordinates, and :data:`w` for the weights.

  **Default value**: :data:`x,y,z,w`.

.. option:: -morphooptistop <var1>=<val1>,<var2>=<val2>,... (secondary option)

  Specify the stopping criteria of the optimization process, which can be:

  - :data:`eps`: absolute error on the value of the objective function evaluated on a number of degrees of freedom basis (:data:`nlopt_eps` or :data:`nlopt_reps` are the NLopt iteration-based values);
  - :data:`reps`: relative error on the value of the objective function evaluated on a number of degrees of freedom basis (:data:`nlopt_eps` or :data:`nlopt_reps` are the NLopt iteration-based values);
  - :data:`xeps`: absolute error on the components of the solution vector;
  - :data:`xreps`: relative error on the components of the solution vector;
  - :data:`<val>`: value of the objective function;
  - :data:`itermax`: a maximum number of iterations;
  - :data:`time`: maximum computation time;
  - :data:`loopmax`: maximum number of iteration loops (see option :option:`-morphooptialgomaxiter`).

  Optimization stops as soon as one stopping criterion is verified.

  Optimization can also be stopped anytime using the :command:`Ctrl+C` command.

  **Default value**: :data:`eps=1e-6` (:data:`val=1e-4,iter=10000` for :data:`-morpho centroidal`).

.. option:: -morphooptialgo <algorithm1>,<algorithm2>,... (secondary option)

  Specify the optimization algorithm, which can be:

    - :data:`subplex`: Subplex (recommend);
    - :data:`praxis`: Praxis (recommended, except for high numbers of seeds, where it becomes highly memory-intensive) [#praxis]_ ;
    - :data:`neldermead`: Nelder-Mead (not recommended);
    - :data:`cobyla`: Cobyla (not recommended);
    - :data:`bobyqa`: Bobyqa (not recommended);
    - :data:`newuoa`: Newuoa (not recommended).
    - :data:`lloyd[(<fact>]`: Lloyd's algorithm using a specified factor (default :data:`1.9`, only for :data:`-morpho centroidal`).
    - :data:`random(<seednb>,<dimnb>,<min>,<max>,<id>)`: random perturbations (use only if you know what you are doing). At each odd iteration, for each of the :data:`seednb` seeds, :data:`dimnb` of its attributes (among those specified by option :data:`-morphooptidof`) are randomly perturbed, the norm of the total perturbation vector ranging from :data:`<min>` to :data:`<max>`; :data:`id` is the identifier of the distribution (similarly to option :option:`-id`).  Variables can be any mathematical expression based on :data:`seednb` (the total number of seeds), :data:`dim` (the tessellation dimension), :data:`avdiameq` (the average equivalent cell diameter) and :data:`inistep` (the value of :option:`-morphooptiinistep`); at each next (even) iteration, the attributes of the seeds are reverted to their original values.

  In several algorithms are provided, the second etc. are used if the previous ones fails.

  **Default value**: :data:`subplex,praxis` (:data:`lloyd` for :data:`-morpho centroidal`).

.. option:: -morphooptigrid <var1>:<grid1>,<var2>:<grid2>,... (secondary option)

  Specify the grids used to discretize the variable distributions. The variables are those defined in :option:`-morpho`, and the grid must be :data:`regular(<min>,<max>,<bin_nb>)`, where :data:`<min>` and :data:`<max>` are the minimum and maximum values of the grid interval, respectively, and :data:`<bin_nb>` is the number of bins.

  **Default value**: :data:`diameq:regular(-1,10,1100),size:regular(-1,10,1100),sphericity:regular(-0.1,1.1,1200),1-sphericity:regular(-0.1,1.1,1200)`.

.. option:: -morphooptismooth <var1>:<val1>,<var2>:<val2>,... (secondary option)

  Specify the standard deviations of the Gaussian distributions which are assigned to each cell data to compute the distributions.  The variables are those defined in :option:`-morpho`.

  It is also possible to specify how the convolution functions should be treated, using :data:`analytical` for analytical functions or :data:`numerical` for numerical functions (the default, recommended).

  **Default value**: :data:`diameq:0.05,size:0.05,sphericity:0.005,numerical`.

.. option:: -morphooptideltamax <deltamax> (secondary option)

  Specify the maximal value by which each variable is allowed to change during optimization.

  Possible values: any (:math:`\geq 0`).

  **Default value**: :data:`HUGE_VAL`.

.. option:: -morphooptiinistep <inistep> (secondary option)

  Specify the step used to perturb the seed positions and weights when optimization begins.  The argument can be a real value of a mathematical expression based on :data:`avdiameq`, the average equivalent cell diameter.

  **Default value**: :data:`avdiameq/10`.

.. option:: -morphooptialgomaxiter <iter_number> (secondary option)

  Specify the maximum number of iterations allowed to the optimization algorithm to run without decreasing the objective function.  The expression can be any mathematical expression based on variable :data:`varnb` (the total number of optimization variables).

  **Default value**: :data:`max(varnb,1000)`.

.. option:: -morphooptilogtime <variables> (secondary option)

  Log the time taken during the optimization process.  The variables can be among those provided in :ref:`time_keys`.

  **Default value**: -.

  **File extension**: :file:`.logtime`.

.. option:: -morphooptilogvar <variables> (secondary option)

  Log the variables (seed attributes) during the optimization process.  The variables can be among those provided in :ref:`variable_keys`.

  **Default value**: -.

  **File extension**: :file:`.logvar`.

.. option:: -morphooptilogval <variables> (secondary option)

  Log the value of the objective function during the optimization process.  The variables can be among those provided in :ref:`objective_function_value_keys`.

  **Default value**: -.

  **File extension**: :file:`.logval`.

.. option:: -morphooptilogdis <variables> (secondary option)

  Log the distributions during the optimization process.  The variables can be among those provided in :ref:`statistical_distribution_keys`.

  **Default value**: -.

  **File extension**: :file:`.logdis#`.

.. option:: -morphooptilogtesr <variables> (secondary option)

  Log the raster tessellation voxel data during the optimization process.  The variables can be among those provided in :ref:`tessellation_optimization_keys`.

  **Default value**: -.

  **File extension**: :file:`.logtesr`.

Group Options
~~~~~~~~~~~~~

The following option can be used to define cell groups (each cell is assigned to a group).  Groups are computed after tessellation and so can be defined from the cell properties (if scalar and raster tessellations are written in output, groups are computed independently for each of them).

.. option:: -group <group_expression>

  Specify the groups of the cells.  The expression can be:

  - an integer number or an expression based on the variables defined in :ref:`tessellation_keys` or :ref:`raster_tessellation_keys`, for example :data:`"vol<0.1?1:2"`;
  - :data:`file(<file_name>)`: values to load from a :ref:`data_file`.

  **Default value**: -.

Crystal Orientation Options
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. option:: -ori <ori_distrib>

  Specify the type of crystal orientation distribution. It can be:

  - :data:`random`: randomly-distributed orientations in 3D space;

  - :data:`uniform`: uniformly-distributed orientations in 3D space [#uniform-crysym]_;

  - :data:`orientation[:<distribution>]`: a :ref:`discrete orientation <rotations_and_orientations>`, with an optional disorientation distribution (see below);

  - :data:`fiber(<dirc_x>,<dirc_y>,<dirc_z>,<dirs_x>,<dirs_y>,<dirs_z>)[:normal(<var>=<val>)]`: randomly-distributed orientations along the fiber, see :ref:`orientation_fibers`, with an optional disorientation distribution (see below);

  - :data:`parent[:normal(<var>=<val>)]`: orientations inherited from the ones of the parent cells, with an optional disorientation distribution (see below);

  - :data:`file(<file_name>[,des=<descriptor>])`: orientations to be read from a :ref:`data_file` written using a specific descriptor (see :ref:`rotations_and_orientations`, default :data:`rodrigues`).

  The optional distributions are:

    - :data:`normal(<var>=<val>)`: a 3-variate normal distribution, where :data:`<var>` can be:

      - :data:`theta`: the 1D standard deviation;
      - :data:`theta1`: the standard deviation about direction 1 (:math:`x`);
      - :data:`theta2`: the standard deviation about direction 2 (:math:`y`);
      - :data:`theta3`: the standard deviation about direction 3 (:math:`z`);
      - :data:`thetam`: the average angle of the distribution;

      and the value is provided in degrees.

    - :data:`flat(theta=<val>)`: a flat (orientation) distribution, where :data:`theta` is the maximum disorientation angle (in degrees).

  An example is :data:`cube:normal(thetam=5)`;

  Finally, a sum of distributions can be provided; e.g., :data:`0.5*random+0.1*cube`.

  **Default value**: :data:`random`.

.. option:: -orispread <spread>

  Specify the type of (in-cell) orientation spreads.  It can be:

  - :data:`normal(<thetam>)`: a 3-variate normal distribution corresponding to an average misorientation angle (with respect to the average orientation) of :data:`<thetam>` (expressed in degree), to be applied to all cells.
  - :data:`file(<file_name>)`: different cell distributions (of the type :data:`normal...`), to load from a :ref:`data_file`.
  - :data:`none`: none.

  **Default value**: :data:`none`.

.. option:: -oricrysym <crysym>

  Specify the :ref:`Crystal Symmetry <crystal_symmetries>`.

  This is used by option :data:`-ori uniform` and to reduce the domain of definition of the orientation descriptors.

  **Default value**: :data:`triclinic`.

.. option:: -orioptiini <ori_distrib> (secondary option)

  Specify the initial crystal orientations, which can be:

  - :data:`random`: random orientations;
  - :data:`file(<file_name>[,des=<descriptor>])`: orientations to be read from a :ref:`data_file` written using a specific descriptor (see :ref:`rotations_and_orientations`, default :data:`rodrigues`).

  **Default value**: :data:`random`.

.. option:: -orioptifix <orientations> (secondary option)

  Specify some orientations to fix during optimization.  The argument can be:

  - :data:`file(<file_name>)`: logical values to load from a :ref:`data_file`;
  - :data:`none`: none.

  **Default value**: :data:`none`.

.. option:: -orioptistop <stopping_criterion> (secondary option)

  Specify the stopping criterion of the optimization process, as a logical expression based on the following variables:

  - :data:`reps`: relative error on the forces at orientations;
  - :data:`iter`: iteration number.

  **Default value**: :data:`"reps<1e-3||iter>=1e3"`.

.. option:: -orioptineigh <neighborhood_radius> (secondary option)

  Specify the radius of the neighborhood of orientations to be used to compute their forces (for :data:`-ori uniform`), which can be any mathematical or logical expression based on:

  - :data:`dr`: average radius of an orientation;
  - :data:`Nstar`: grand number of orientations (i.e., taking crystal symmetry into account).

  **Default value**: :data:`"Nstar<10000?pi:20*dr"`.

.. option:: -orioptilogvar <variables> (secondary option)

  Log the variables (the orientations) during the optimization process.  The variables can be among those provided in :ref:`orientation_optimization_keys`.

  **Default value**: -.

  **File extension**: :file:`.logorivar`.

Transformation Options
~~~~~~~~~~~~~~~~~~~~~~

.. option:: -transform <transformation1>,<transformation2>,...

  Apply successive transformations to a tessellation (if scalar and raster tessellations are written in output, they are transformed independently from each other).

  **For a scalar tessellation**, the transformations can be:

  -  |translate|;
  -  |rotate|;
  -  |scale|. For a 2D tessellation, :data:`<fact_z>` can be omitted.

  - :data:`cut(<primitive1>,<primitive2>,...)`: cut by a series of geometrical primitives (experimental).
    The region interior to the primitives is removed from the tessellation.  Append :data:`i` to a primitive name (as in :data:`spherei`, etc.) for the outer region.

    The primitives can be:

    - :data:`hspace[i](<d>,<a>,<b>,<c>)`: the half-space of equation :math:`a\,x+b\,y+c\,z \geq d`;

    - :data:`sphere[i](<center_x>,<center_y>,<center_z>,<rad>)`: a sphere of center (:data:`<center_x>`, :data:`<center_y>`, :data:`<center_z>`) and radius :data:`<rad>`;

    - :data:`cylinder[i](<basis_x>,<basis_y>,<basis_z>,<axis_x>,<axis_y>,<axis_z>,<rad>)`: a cylinder of basis point (:data:`<basis_x>`, :data:`<basis_y>`, :data:`<basis_z>`), axis (:data:`<axis_x>`, :data:`<axis_y>`, :data:`<axis_z>`) and radius :data:`<rad>`;

    - :data:`ecylinder[i](<basis_x>,<basis_y>,<basis_z>,<axis_x>,<axis_y>,<axis_z>,<esaxis1_x>,<esaxis1_y>,<esaxis1_z>,<esaxis2_x>,<esaxis2_y>,<esaxis2_z>,<srad1>,<srad2>)`: an elliptic cylinder of basis point (:data:`<basis_x>`, :data:`<basis_y>`, :data:`<basis_z>`), axis (:data:`<axis_x>`, :data:`<axis_y>`, :data:`<axis_z>`), ellipse section first axis (:data:`<esaxis1_x>`, :data:`<esaxis1_y>`, :data:`<esaxis1_z>`), ellipse section second axis (:data:`<esaxis2_x>`, :data:`<esaxis2_y>`, :data:`<esaxis2_z>`), ellipse section first radius :data:`<esrad1>` and ellipse section second radius :data:`<esrad2>`;

    - :data:`torus[i](<basis_x>,<basis_y>,<basis_z>,<axis_x>,<axis_y>,<axis_z>,<rad>,<srad>)`: a torus of basis point (:data:`<basis_x>`, :data:`<basis_y>`, :data:`<basis_z>`), axis (:data:`<axis_x>`, :data:`<axis_y>`, :data:`<axis_z>`), radius :data:`<rad>` and section radius :data:`<srad>`;

    - :data:`cube[i](<xmin>,<xmax>,<ymin>,<ymax>,<zmin>,<zmax>)`: a cube of :math:`x`, :math:`y` and :math:`z` coordinates in the specified ranges. (Only :data:`cubei` is available).

  - :data:`planecut(<d>,<a>,<b>,<c>)`: cut by the (oriented) plane of equation :math:`a\,x+b\,y+c\,z=d`.

  - :data:`crop(<primitive>)`: crop by a primitive. The primitive can be:

    - :data:`cube(<xmin>,<xmax>,<ymin>,<ymax>,<zmin>,<zmax>)` for a cube defined by its :math:`x`, :math:`y` and :math:`z` bounds.

  - :data:`slice(<d>,<a>,<b>,<c>)` for slicing a 3D tessellation by the (oriented) plane of equation :math:`a\,x+b\,y+c\,z=d` (yielding to a 2D tessellation).

  - :data:`mergecell(<expr1>,<expr2>,...)`: merge cells matching successive expressions :data:`<expr1>`, :data:`<expr2>`, etc., where expressions are based on the variables defined in :ref:`tessellation_keys`.

  - :data:`rmcell(<expr1>,<expr2>,...)` remove cells matching successive expressions :data:`<expr1>`, :data:`<expr2>`, etc., where expressions are based on the variables defined in :ref:`tessellation_keys`.

  - :data:`resetcellid`: reset cell ids to get a contiguous numbering starting from 1.

  - :data:`resetlabels`: reset the domain face, edge and vertex labels.

  - :data:`ori(<file_name>[,des=<descriptor>])`: override cell orientations with ones defined in a :ref:`data_file` written using a specific descriptor (see :ref:`rotations_and_orientations`, default :data:`rodrigues`).

  **For a raster tessellation**, the transformations can be:

  -  |translate|;
  -  |rotate|;
  -  |scale|. For a 2D tessellation, :data:`<fact_z>` can be omitted.

  -  :data:`renumber`: renumber cells to remove those that are empty or have a zero id.

  -  :data:`unindex`: assign a zero cell id to voxels of orientation :math:`(0,\,0,\,0)` (in Rodrigues vector).

  -  :data:`oriaverage`: set the cell orientations (field :data:`**cell/ori`) as the averages of the cell voxel orientations (field :data:`**oridata`).

  -  :data:`crop(<primitive>)`: crop the raster tessellation by a primitive.  The primitive can be:

    - :data:`cube(<xmin>,<xmax>,<ymin>,<ymax>,<zmin>,<zmax>)`: cube defined by its :math:`x`, :math:`y` and :math:`z` bounds;

    - :data:`cylinder(<center_x>,<center_y>,<diameter>)`: cylinder of center (:math:`<center_x>`, :math:`<center_y>`) of :math:`z` axis,

    - :data:`square(<xmin>,<xmax>,<ymin>,<ymax>)`: square defined by its :math:`x` and :math:`y` bounds (2D tessellation only),

    - :data:`circle(<center_x>,<center_y>,<diameter>)`: circle (2D tessellation only);

  -  :data:`autocrop`: reduce the raster to its minimal size.

  -  :data:`rasterscale(<fact_x>,<fact_y>,<fact_z>)`: scale the number of voxels of the raster by factors :data:`<fact_x>`, :data:`<fact_y>` and :data:`<fact_z>` along :data:`x`, :data:`y` and :data:`z`, respectively. For a 2D tessellation, :data:`<fact_z>` can be omitted.

  -  :data:`rmsat`: remove the cell *satellites*, i.e. parts disconnected from the cell bulk.

  -  :data:`grow`: grow the cells to fill the domain.

  -  :data:`tessinter(<tess_file>)`: intersect with tessellation :file:`tess_file`.

  -  :data:`addbuffer(<buff_x>,<buff_y>,<buff_z>)`: add a buffer of :data:`<buff_x>` void voxels on both sides in the x direction, :data:`<buff_y>` void voxels on both sides in the y direction and :data:`<buff_z>` void voxels on both sides in the z direction.

  -  :data:`2d`: transform a 3D tessellation with 1 voxel along z into a 2D tessellation.

  **Default value**: -.

.. option:: -sort <sort_expression> (secondary option)

  Sort the tessellation cells (typically to facilitate data post-processing) following a mathematical expression based on the tessellation variables (see :ref:`tessellation_keys`). Sorting is done in ascending order.

  **Default value**: -.

Regularization Options
~~~~~~~~~~~~~~~~~~~~~~

.. option:: -regularization <logical>

  Regularize a tessellation, that is, removes the small edges and (indirectly) faces. Regularization enables :ref:`meshing with higher-quality elements <neper_m>` and generates some slightly non-planar internal faces (in 3D).

  Control parameters can be set using options :option:`-fmax`, :option:`-sel` and :option:`-mloop`.

  **Default value**: :data:`0`.

.. option:: -fmax <maximum_angle>

  Specify the maximum allowed face flatness fault (in degrees). The flatness fault is the maximum angle between the normals at two locations on a face.

  **Default value**: :data:`20`.

.. option:: -sel <length> or -rsel <relative_length> [secondary option]

  Specify the absolute, :data:`sel`, or relative, :data:`rsel`, (maximum) small edge length.  :data:`rsel` is defined relative to the average cell size (volume in 3D and area in 2D), and a value of :data:`1` corresponds to a length of 0.25 for a unit volume cell in 3D and 0.125 for a unit area cell in 2D.
  The value can be:

  - a real value that applies to all cells;
  - an expression of the form :data:`<default_sel>,<cell_expr1>:<cell_sel1>,<cell_expr2>:<cell_sel2>...` to define different cell values, where :data:`<default_sel>` is the default small edge length, :data:`<cell_expr#>` is an expression defining the #th set of cells and :data:`<cell_sel#>` is the corresponding small edge length.  :data:`<cell_expr#>` can be any expression based on variables provided in :ref:`tessellation_keys`.  Expressions are processed successively.

  - :data:`file(<file_name>)` to load values from a :ref:`data_file`.

  The default value allows one to avoid mesh refinement with the default meshing parameters (see option :data:`-rcl`), but the value should typically be the same as the one of data :data:`-rcl`.

  **Default value**: :data:`-rsel 1`.

.. option:: -mloop <loop_number> [secondary option]

  Specify the maximum number of regularization loops.  During each loop, the small edges are considered for removal in turn from the shortest to the longest.  Regularization stops when the maximum number of loops is reached or no edges are deleted during a loop.

  **Default value**: :data:`2`.

Output Options
~~~~~~~~~~~~~~

.. option:: -o <file_name>

  Specify the output file name (no extension).

  **Default value**: :data:`n\<n\>-id\<id\>`.

.. option:: -format <format1>,<format2>,...

  Specify the format(s) of the output file(s), which can be:

    - tessellation: :data:`tess`, :data:`sim`, :data:`geo`, :data:`ply`, :data:`stl[:bycell]`, :data:`obj`, :data:`3dec`, :data:`fe`;
    - raster tessellation: :data:`tesr`, :data:`sim`, :data:`vtk`;
    - orientations: :data:`ori`.

  See :ref:`output_files` for details on the file formats.

  **Default value**: :data:`tess`.

.. option:: -tesrformat <format1>,<format2>,...

  Specify the format(s) of the raster output file(s), which can be:

    - :data:`ascii`: ASCII;
    - :data:`binary8`: 8-bit binary / unsigned char-type;
    - :data:`binary16` or :data:`binary16_big`: 16-bit binary / short-type;
    - :data:`binary32` or :data:`binary32_big`: 32-bit binary / int-type.

    :data:`binary16` and :data:`binary32` correspond to little endianness while :data:`binary16_big` and :data:`binary32_big` correspond to big endianness. [#endianness]_

  **Default value** : :data:`binary16` or :data:`binary_big` (depending on the system).

.. option:: -tesrsize <number_of_voxels>

  Specify the number of voxels of a raster tessellation along a direction of
  the domain, which can be:

  - :data:`<number>`: a single integer value (in the case of a domain of different lengths along the different directions, the value is considered as the geometrical average of the number of voxels along the different directions, so that the voxels are as cubic as possible);
  - :data:`<number_x>:\<number_y\>:\<number_z\>`: integer values along the :data:`x`, :data:`y` and :data:`z` directions.

  **Default value**: :data:`20`.

.. option:: -oridescriptor <descriptor[:convention]>

  Specify the orientation descriptor and (optionally) the orientation convention used in the :file:`.tess`, :file:`.tesr` and :file:`.ori` files.  See :ref:`rotations_and_orientations` for possible values.

  **Default value**: :data:`rodrigues:active`.

.. option:: -oriformat <format1>,<format2>,...

  Specify the format(s) of the :file:`.ori` output file(s), which can be:

  - :data:`plain`: plain format, for which descriptors are provided on successive lines;
  - :data:`geof`: Z-set format (Euler angles in Bunge convention are written).

  If several formats are specified, the format is appended to the file name as in :file:`<file_name>.ori-plain` and :file:`<file_name>.ori-geof`.

  **Default value**: :data:`plain`.

Post-Processing Options
~~~~~~~~~~~~~~~~~~~~~~~

The following two options provide general statistics on tessellations.

.. option:: -stattess <key1>,<key2>,...

  Provide statistics on the tessellation.  Available keys are described in :ref:`tessellation_keys`.

  **Default value**: -.

  **File extension**: :file:`.sttess`.

.. option:: -stattesr <key1>,<key2>,...

  Provide statistics on the raster tessellation.  Available keys are described in :ref:`raster_tessellation_keys`.

  **Default value**: -.

  **File extension**: :file:`.sttesr`.

The following options apply to the cells, seeds or cell groups of a tessellation or a raster tessellation, independently of its dimension.

.. option:: -statcell <key1>,<key2>,...

  Provide statistics on the tessellation cells.  Available keys are described in :ref:`tessellation_keys` and :ref:`raster_tessellation_keys`.

  **Default value**: -.

  **File extension**: :file:`.stcell`.

.. option:: -statseed <key1>,<key2>,...

  Provide statistics on the tessellation seeds.  Available keys are described in :ref:`tessellation_keys` and :ref:`raster_tessellation_keys`.

  **Default value**: -.

  **File extension**: :file:`.stseed`.

.. option:: -statgroup <key1>,<key2>,...

  Provide statistics on the tessellation cell groups.  Available keys are described in :ref:`tessellation_keys` and :ref:`raster_tessellation_keys`.

  **Default value**: -.

  **File extension**: :file:`.stgroup`.

  For a tessellation, it is also possible to get statistics on an per-entity basis using the following options.

.. option:: -statver <key1>,<key2>,...

  Provide statistics on the tessellation vertices.  Available keys are described in :ref:`tessellation_keys`.

  **Default value**: -.

  **File extension**: :file:`.stver`.

.. option:: -statedge <key1>,<key2>,...

  Provide statistics on the tessellation edges.  Available keys are described in :ref:`tessellation_keys`.

  **Default value**: -.

  **File extension**: :file:`.stedge`.

.. option:: -statface <key1>,<key2>,...

  Provide statistics on the tessellation faces.  Available keys are described in :ref:`tessellation_keys`.

  **Default value**: -.

  **File extension**: :file:`.stface`.

.. option:: -statpoly <key1>,<key2>,...

  Provide statistics on the tessellation polyhedra.  Available keys are described in :ref:`tessellation_keys`.

  **Default value**: -.

  **File extension**: :file:`.stpoly`.

For a raster tessellation, it is also possible to get statistics on an per-voxel basis,

.. option:: -statvox <key1>,<key2>,...

  Provide statistics on the tessellation voxels.  Available keys are described in :ref:`raster_tessellation_keys`.

  **Default value**: -.

  **File extension**: :file:`.stvox`.

Finally, it is possible to get statistics for a particular set of points.

.. option:: -statpoint <key1>,<key2>,...

  Provide statistics on the points loaded with option :option:`-loadpoint`. Available keys are described in :ref:`point_keys`.

  **Default value**: -.

  **File extension**: :file:`.stpoint`.

Debugging Options
~~~~~~~~~~~~~~~~~

.. option:: -checktess <file_name>

  Check a tessellation file.

  Use this option if a tessellation file fails to load using option :option:`-loadtess` or in other modules.

  **Default value**: -.

.. _output_files:

Output Files
------------

Tessellation
~~~~~~~~~~~~

- :file:`.tess`: Neper (scalar) tessellation file (see :ref:`tess_file`);

- :file:`.tesr`: Neper raster tessellation file (see :ref:`tesr_file`);

- :file:`.sim`: Neper simulation directory (see :ref:`simulation_directory`);

-  :file:`.geo`: Gmsh geometry file describing (under a minimal form) the tessellation and can be used for interactive visualization with Gmsh;

-  :file:`.ply`: Ply ("Polygon File Format") file describing the tessellation;


-  :file:`.stl`: STL ("STereoLithography") file describing the tessellation.  If :data:`-format stl:bycell` is used, a separate file is written for each cell, whose name ends in :file:`-<id>.stl`, where :data:`id` is the cell identifier written with leading zeros;

-  :file:`.obj`: Wavefront geometry file describing the tessellation;

-  :file:`.3dec`: Itasca 3DEC file describing the tessellation;

-  :file:`.vtk`: VTK file describing the raster tessellation and that is supported by Amitex_ffpt.  Binary data are always written using big endians;

-  :file:`.ori`: orientation file describing the crystal orientations of the tessellation cells.  The orientations are written on successive lines, using the descriptor specified by option :option:`-oridescriptor` (see also :ref:`rotations_and_orientations`) and the writing convention specified by option :option:`-oriformat`.

Statistics
~~~~~~~~~~

Statistics files are first provided for the tessellation and raster tessellation. Each file contains the data specified to the corresponding :data:`-stat` option and as described in :ref:`tessellation_keys` and :ref:`raster_tessellation_keys`.

- :file:`.sttess`: tessellation statistics file;
- :file:`.sttesr`: raster tessellation statistics file.

Statistics files are also provided for cells, seeds, vertices, edges, faces, polyhedra and points.  They are formatted with one line per entity.  Each line contains the data specified to the corresponding :data:`-stat` option and described in :ref:`tessellation_keys` and :ref:`raster_tessellation_keys`.

  - :file:`.stcell`:  tessellation cell statistics file;
  - :file:`.stseed`:  tessellation seed statistics file;
  - :file:`.stgroup`: tessellation cell group statistics file;
  - :file:`.stver`:   tessellation vertex statistics file;
  - :file:`.stedge`:  tessellation edge statistics file;
  - :file:`.stface`:  tessellation face statistics file;
  - :file:`.stpoly`:  tessellation polyhedron statistics file;
  - :file:`.stpoint`: point statistics file.

Tessellation Optimization Log Files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Log files are provided for the time, variables, statistical distributions and objective function value.  The files contain the data specified to the corresponding :data:`-morphooptilog` option and described in :ref:`tessellation_optimization_keys`.

  - :file:`.logtime`: time file;
  - :file:`.logvar` : variables (seed attributes) file;
  - :file:`.logdis#`: statistical distribution files;
  - :file:`.logval`:  objective function value file;
  - :file:`-obj.tesr`: target raster tessellation file.


Orientation Optimization Log Files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A log file is provided for the orientation variables.
The files contain the data specified to the
:option:`-orioptilogvar` option and described in :ref:`orientation_optimization_keys`.

  - :file:`.logorivar`: variables (orientations) file.


Examples
--------

Below are some examples of use of neper -T.

- Generate a Voronoi tessellation containing 100 cells:

  .. code-block:: console

    $ neper -T -n 100

- Generate a different Voronoi tessellation containing 100 cells (identifier = 2):

  .. code-block:: console

    $ neper -T -n 100 -id 2

- Use an elongated domain and generate a Voronoi tessellation containing 100 cells:

  .. code-block:: console

    $ neper -T -n 100 -domain "cube(3,1,0.33)"

- Generate a Voronoi tessellation containing 100 cells and apply regularization:

  .. code-block:: console

    $ neper -T -n 100 -reg 1

- Generate a 2D Voronoi tessellation containing 100 cells:

  .. code-block:: console

    $ neper -T -n 100 -dim 2

- Generate a tessellation containing 100 cells with an :data:`x` columnar axis:

  .. code-block:: console

    $ neper -T -n 100 -morpho "columnar(x)"

- Generate a tessellation containing 100 cells with a bamboo structure along :data:`x`:

  .. code-block:: console

    $ neper -T -n 100 -morpho "bamboo(x)"

- Generate a tessellation containing 100 cells with experimental grain-growth morphological properties:

  .. code-block:: console

    $ neper -T -n 100 -morpho gg

- Generate a tessellation containing 100 cells with experimental grain-growth morphological properties and define groups by splitting cells based on their ids:

  .. code-block:: console

    $ neper -T -n 100 -morpho gg -group "id<=50?1:2"

- Generate a tessellation containing 100 cells with experimental grain-growth morphological properties and an aspect ratio of 2:1:0.5:

  .. code-block:: console

    $ neper -T -n 100 -morpho "gg,aspratio(2,1,0.5)"

- Generate a tessellation containing 100 cells with experimental grain-growth morphological properties, and get the equivalent diameters and sphericities of the cells:

  .. code-block:: console

    $ neper -T -n 100 -morpho gg -statcell diameq,sphericity

- Generate a tessellation of specified absolute grain size distribution (the number of cells is determined accordingly):

  .. code-block:: console

    $ neper -T -n from_morpho -morpho "diameq:lognormal(0.1,0.03),1-sphericity:lognormal(0.145,0.03)"

- Generate a tessellation in a non-convex domain (by cutting the tessellation once generated):

  .. code-block:: console

    $ neper -T -n 100 -morpho gg -transform "cut(cylinder(1.2,0.5,0.5,0,1,0,0.4))"

- Generate a 2-scale Voronoi tessellation containing 100 x 10 cells:

  .. code-block:: console

    $ neper -T -n 100::10

- Generate a 2-scale Voronoi tessellation containing 100 x 10 cells, with different tessellations at scale 2 (identifier = 2) (identifier = 2):

  .. code-block:: console

    $ neper -T -n 100::10 -id 1::2

- Generate a 2-scale tessellation containing 10 primary cells with grain-growth morphological properties, each one divided into lamellae` of width 0.1:

  .. code-block:: console

    $ neper -T -n 10::from_morpho -morpho "gg::lamellar(w=0.1)"

- Generate a 2-scale Voronoi tessellation containing 10 primary cells with grain-growth morphological properties, each one divided into lamellae` of widths loaded from file :file:`lam_width` and plane normals loaded from file :file:`lam_normal`:

  .. code-block:: console

    $ neper -T -n 10::from_morpho -morpho "gg::lamellar(w=msfile(lam_width),v=msfile(lam_normal))"

    lam_width:
    1 0.05
    2 0.10
    3 0.05
    4 0.10
    5 0.05
    6 0.10
    7 0.05
    8 0.10
    9 0.05
    10 0.10

    lam_normal:
    1 1.000000 0.000000 0.000000
    2 0.000000 1.000000 0.000000
    3 1.000000 0.000000 0.000000
    4 0.000000 1.000000 0.000000
    5 1.000000 0.000000 0.000000
    6 0.000000 1.000000 0.000000
    7 1.000000 0.000000 0.000000
    8 0.000000 1.000000 0.000000
    9 1.000000 0.000000 0.000000
    10 0.000000 1.000000 0.000000

- Generate a 2-scale Voronoi tessellation containing 3 primary cells divided into 1, 10 and 100 secondary cells, respectively:

  .. code-block:: console

    $ neper -T -n "3::msfile(myfile)" -id 1::1

    myfile:
    1 1
    2 10
    3 100

- Generate a 2-scale Voronoi tessellation containing 2 x 3 cells with specific seed coordinates at both scales (files :file:`coo1` and :file:`coo2`):

  .. code-block:: console

    $ neper -T -n 2::3 -id 1::1 -morphooptiini "coo,file(coo1),weight,0::coo,msfile(coo2),weight,0" -morpho voronoi

    coo1:
    0.25 0.50 0.50
    0.75 0.50 0.50

    coo2:
    1 0.25 0.10 0.50
    1 0.25 0.50 0.50
    1 0.25 0.90 0.50
    2 0.75 0.50 0.10
    2 0.75 0.50 0.50
    2 0.75 0.50 0.90

  .. note:: :file:`coo1` is a simple position file (:ref:`position_file`) while :file:`coo2` is a multiscale cell file (:ref:`multiscale_cell_file`).

- Generate a Voronoi tessellation containing 100 cells  with uniformly distributed crystal orientations and cubic crystal symmetry:

  .. code-block:: console

    $ neper -T -n 100 -oricrysym cubic -ori uniform

- Generate 100 uniformly distributed crystal orientations with cubic crystal symmetry (no tessellation):

  .. code-block:: console

    $ neper -T -n 100 -oricrysym cubic -ori uniform -for ori

References
----------

.. [CMAME2011] R. Quey, P.R. Dawson and F. Barbe, Large-scale 3D random polycrystals for the finite element method: Generation, meshing and remeshing, Comput. Methods Appl. Mech. Engrg., vol. 200, pp. 1729-1745, 2011.

.. [CMAME2018] R. Quey and L. Renversade, Optimal polyhedral description of 3D polycrystals: Method and application to statistical and synchrotron X-ray diffraction data, Comput. Methods Appl. Mech. Engrg., vol. 330, pp. 308-333, 2018.

.. [JAC2018] R. Quey, A. Villani and C. Maurice, Nearly uniform sampling of crystal orientations. J. Appl. Crystallogr., vol. 51, pp. 1162-1173, 2018.

.. [LLLFP2011] A. Lyckegaard, E.M. Lauridsen, W. Ludwig, R.W. Fonda, and H.F. Poulsen. On the Use of Laguerre Tessellations for Representations of 3D Grain Structures. Advanced Engineering Materials, vol. 13, pp. 165--170, 2011.

.. [#cut] This capability is experimental and may fail as the local shape curvature approaches the cell size.

.. [#convex] Any normal tessellation, i.e. tessellation made of convex cells intersecting (one-to-one) along vertices, edges and faces, can be generated at a given *scale*.  There is no limitation owing to the use of the Laguerre parameterization.  For more complex structures, see the *multiscale* capability.

.. [#centroidal] :data:`centroidal` is not recommended as it does not correspond to a morphological property *per se*; size and/or sphericity properties should be used instead.

.. [#sphericity-circularity] Terms :data:`sphericity` and :data:`circularity` apply to 3D and 2D, respectively, but can be used interchangeably. The sphericity of a polyhedron corresponds to the ratio of the surface area of the sphere of equivalent volume to the surface area of the polyhedron. Similarly, the circularity of a polygon corresponds to the ratio of the perimeter of the circle of equivalent surface area to the perimeter of the polygon.

.. [#1-sphericity] The reason behind the :data:`1-sphericity` (or :data:`1-circularity`) variable is that, for a grain growth microstructure, 1 :math:`-` the sphericity follows a lognormal distribution [#CMAME2018]_.

.. [#endianness] Endianness is both written in the tesr file and tested on the system when reading the tesr file, so that the user normally does not have to care about it (even when transferring files across systems).

.. [#uniform-crysym] The crystal symmetry must be specified using :option:`-oricrysym`.

.. [#rotate] Cell orientations are rotated accordingly.

.. [#euler-bunge] The angle convention is determined automatically from the values.

.. [#praxis] The amount of memory needed approximately scales with the square of the number of seeds. :data:`-n 10000 -morpho gg` requires 12 Gb of RAM.

.. [#size_diameq] :data:`size` and :data:`diameq` should generally be considered as mutually exclusive.
