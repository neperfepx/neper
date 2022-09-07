.. |translate| replace:: :data:`translate(<dist_x>,<dist_y>,<dist_z>)`: translate by distances :data:`<dist_x>`, :data:`<dist_y>` and :data:`<dist_z>` along :data:`x`, :data:`y` and :data:`z`, respectively
.. |rotate| replace:: :data:`rotate(<axis_x>,<axis_y>,<axis_z>,<angle>)`: rotate about the center and by an axis/angle pair (angle expressed in degrees)
.. |scale| replace:: :data:`scale(<fact_x>,<fact_y>,<fact_z>)`: scale by :data:`<fact_x>`, :data:`<fact_y>` and :data:`<fact_z>` along :data:`x`, :data:`y` and :data:`z`, respectively
.. |data_description| replace:: This can be done either directly, by providing one or several values (e.g. the RGB channel values for color) or indirectly, e.g. using scalar values that are converted in color using a given *color scheme*.  In this case (and for real-type data), a scale image is also generated, and its properties (minimum, maximum and tick values) can be specified.

.. index::
   single: -V

.. _neper_v:

Visualization Module (-V)
=========================

Module -V is the module for visualizing tessellations, meshes, simulation results and custom data, either as publication-quality (raster) PNG or (vectorial) PDF images, or VTK files, for interactive visualization.  Visualization can be done in real (physical) space, but also on pole figures, which is managed by option :option:`-space`.

Virtually any detail of the visualization can be set.  For example, all entities (tessellation polyhedra, faces, edges, vertices, seeds and crystals, 3D, 2D, 1D and 0D mesh elements and nodes, and points) can be assigned a particular color, size, transparency, etc. (options :data:`-data*`), the visibility of the different entities can be adjusted (options :option:`-show*`), or slice views can be generated (option :option:`-slicemesh`).  This different capabilities make it possible to carry out standard or advanced visualizations but also post-processing.

.. note:: The specification of colors, etc. via options :option:`-data*` and visibility via options :option:`-show*` is common to all spaces.

Standard, real space visualizations (default :option:`-space` :data:`real`) are achieved using the POV-Ray ray tracing renderer to produce high-quality (raster) PNG images.  The parameters of the "scene" are assigned default values, but can also be fine-tuned, such as the light positions, camera position and angle, projection type, etc. (options :data:`-camera*` and :data:`-light*`).

Pole figure visualizations (:option:`-space` :data:`pf`) are achieved using the Asymptote vector graphics rendered to produce high-quality (raster) PNG images (by default), but also high-quality (vectorial) PDF images.  Data can be represented as symbols or a density field, and can be superimposed.

In contrast to other modules, module -V processes the command arguments one after the other.

Here is what a typical run of module -V looks like:

.. code-block:: console

  $ neper -V n10-id1.tess,n10-id1.msh -dataelsetcol id -print img

  ========================    N   e   p   e   r    =======================
  Info   : A software package for polycrystal generation and meshing.
  Info   : Version 4.0.0
  Info   : Built with: gsl|muparser|opengjk|openmp|nlopt|libscotch (full)
  Info   : Running on 8 threads.
  Info   : <https://neper.info>
  Info   : Copyright (C) 2003-2021, and GNU GPL'd, by Romain Quey.
  Info   : No initialization file found (`/home/rquey/.neperrc').
  Info   : ---------------------------------------------------------------
  Info   : MODULE  -V loaded with arguments:
  Info   : [ini file] (none)
  Info   : [com line] n10-id1.tess,n10-id1.msh -dataelsetcol id -print img
  Info   : ---------------------------------------------------------------
  Info   : Loading tessellation...
  Info   :     [i] Parsing file `n10-id1.tess'...
  Info   :     [i] Parsed file `n10-id1.tess'.
  Info   : Loading mesh...
  Info   :     [i] Parsing file `n10-id1.msh'...
  Info   :     [i] Parsed file `n10-id1.msh'.
  Info   : Reconstructing mesh...
  Info   : Reading data (elset3d, col)...
  Info   : Printing image...
  Info   :     [o] Writing file `img.pov'...
  Info   :   - Printing mesh...
  Info   :     > Reducing data...
  Info   :       . 3D...
  Info   :       . 2D...
  Info   :       . Number of 3D elt faces reduced by  90% (to 418).
  Info   :       . Number of 3D elt edges reduced by  50% (to 627).
  Info   :       . Number of 0D elts      reduced by 100% (to 0).
  Info   :     [o] Wrote file `img.pov'.
  Info   :   - Generating png file (1200x900 pixels)...
  Info   :     [o] Writing file `img.png'...
  Info   :     [o] Wrote file `img.png'.
  Info   : Printing scale...
  Info   : Elapsed time: 1.546 secs.
  ========================================================================

Arguments
---------

Prerequisites
~~~~~~~~~~~~~

.. option:: -povray <binary>

  Specify the POV-Ray command or the path to the POV-Ray binary (for generating standard PNG images).

  **Default value**: :data:`povray`.

.. option:: -asymptote <binary>

  Specify the Asymptote command or the path to the Asymptote binary (for generating pole figures).

  **Default value**: :data:`asy`.

.. _neper_v_input_data:

Input Data
~~~~~~~~~~

.. option:: <file_name1>,<file_name2>,...

  Specify the input file(s) or directory. The argument can be:

  - a :ref:`tess_file`;
  - a :ref:`tesr_file`;
  - a :ref:`mesh_file`;
  - a :ref:`simulation_directory`;
  - :data:`[<input>[(type=<type>)]:]file(<file_name>[,des=<descriptor>])`: a custom input (points, vectors, ...) to load from a :ref:`data_file`, given a custom name, :data:`<input>` (default :data:`point`), and of a specified type, which can be:

    - :data:`point`: points (default in :option:`-space` :data:`real`);
    - :data:`ori`: orientations (default in :option:`-space` :data:`pf`);
    - :data:`vector`: vectors.

  For :data:`ori`, the descriptor can be specified (see :ref:`rotations_and_orientations`, default :data:`rodrigues`).

  .. note:: As a custom input is loaded, new options such as :option:`-data<input>col`, :option:`-show\<input\>`, etc. are defined, as described in the following.  It is possible to load several custom inputs.

  **Default value**: -.

When a simulation directory is loaded as input, it is possible to specify the simulation step to consider.

.. option:: -step <step>

  Specify the simulation step (:data:`0` for the initial state).,

  **Default value**: :data:`0`.

The data internal to the simulation directory have the same status as internal data (such as, for instance, the nominal element orientations stored in the mesh file) and can therefore be loaded (with options :data:`-data*`) more simply than with standard external files.  For example, coloring elements from orientations can be done using :data:`-step 2 -dataeltcol ori`.

Space Options
~~~~~~~~~~~~~

The following option enables the definition of the space in which data (simulation, tessellation, mesh, point, etc.) are represented.  The space is most generally real (physical) space, :data:`real`, but can be defined as pole figure space, :data:`pf`, or tree space, :data:`tree`.  Tree space shows a tree-like structure of a simulation directory.

.. option::  -space <space>

  Specify the space in which data are represented, which can be:

  - :data:`real`: real (physical) space;
  - :data:`pf`: pole figure space;
  - :data:`tree`: tree space.

  **Default value**: :data:`real`.

.. - :data:`rodrigues` for Rodrigues orientation space;

Tessellation Data Loading and Rendering Options
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The following option enables the definition of the cell data itself (pole figure space only):

.. index::
   single: -datacell

.. option:: -datacell <type>:<value>

  Specify the cell data type and value, which can be:

  - :data:`ori`: orientations;
  - :data:`vector:\<res>`: a vectorial result from the :ref:`simulation_directory`.

  **Default value**: :data:`ori`.


The following options enable the definition of the properties (color and size) of the tessellation cells or entities (polyhedra, faces, edges and vertices), seeds and crystals.  *Crystals* are plotted at the centers of their respective cells, shaped according to the `Crystal Symmetry <crystal_symmetries>`_ and have the same volumes as their respective cells.  |data_description|

For each entity, all attributes can be set, although the may not apply in certain spaces (see option :option:`-space`). Specifically,

  - :data:`-data{cell,poly,face}rad` applies only in PF space;
  - :data:`-data*trs` does not apply in PF space.

.. index::
   single: -datacellcol
   single: -datapolycol
   single: -datafacecol
   single: -dataedgecol
   single: -datavercol
   single: -dataseedcol
   single: -datavoxcol
   single: -datavoidvoxcol
   single: -datacrystaledgecol
   single: -datacelledgecol
   single: -datavoxedgecol
   single: -datacrystalcol
   single: -data...col

.. option:: -data{cell,poly,face,edge,ver,seed,crystal,vox,voidvox,{crystal,cell,vox}edge}col <color>

  Specify the colors, which can be done in two main different ways.

  **Colors can be specified directly**:

  - a color (see :ref:`colors_and_color_maps`);
  - :data:`file(<file_name>)`: individual colors to load from a :ref:`data_file` (see :ref:`colors_and_color_maps`).

  **Colors can be set from data using an argument of the general form** :data:`<data_type>:\<data_value\>`, which can be shortened to :data:`<data_value>` in the frequent case where the data type can be unambiguously determined from the data value.  The possibilities are:

  - :data:`int:\<data_value\>`: integer values represented using a color palette,  which can be:

    - an integer value;
    - an expression based on the variables defined in :ref:`tessellation_keys`, such as :data:`id` or :data:`mode`, or in a :ref:`simulation_directory`, which allows to define individual values;
    - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  - :data:`real:\<data_value\>`: real values represented using a smooth color scale, which can be:

    - a real value;
    - an expression based on the variables described in :ref:`tessellation_keys`, such as :data:`x` or :data:`vol`, or in a :ref:`simulation_directory`, which allows to define individual values;
    - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  - :data:`ori:\<data_value\>`: crystal orientations (when applicable), which can be:

    - :data:`orientation`: an orientation (see :ref:`rotations_and_orientations`);
    - :data:`internal`: internal orientations;
    - :data:`file(<file_name>,[des=<descriptor>])`: individual values to load from a :ref:`data_file` (see :ref:`rotations_and_orientations`).

  - :data:`disori:\<data_value\>` (only for :data:`vox`): crystal disorientations (the rotation with respect to the nominal cell orientation, when applicable), which can be:

    - :data:`<disorientation>`: a disorientation (see :ref:`rotations_and_orientations`);
    - :data:`internal`: internal disorientations;
    - :data:`file(<file_name>,[des=<descriptor>])`: individual values to load from a :ref:`data_file` (see :ref:`rotations_and_orientations`).

  See also option :data:`-data...colscheme`.

  .. note:: :option:`-datacelledgecol` applied only in PF space.

  **Default value**: :data:`id` for cells and crystals, :data:`white` for faces, :data:`black` for edges and vertices, :data:`gray` for seeds, voxels and void voxels, :data:`black` for voxel edges.


.. index::
   single: -datacellcolscheme
   single: -datapolycolscheme
   single: -datafacecolscheme
   single: -dataedgecolscheme
   single: -datavercolscheme
   single: -dataseedcolscheme
   single: -datavoxcolscheme
   single: -datavoidvoxcolscheme
   single: -datavoxedgecolscheme
   single: -data...colscheme

.. option:: -data{cell,poly,face,edge,ver,seed,crystal,vox}colscheme <col_scheme>

  Specify the color scheme used to get colors from the data loaded with option :data:`-data...col`.
  The type of color scheme depends on the type of data.

  - For integer values, the color scheme is :data:`palette`, as described in :ref:`color_map_for_integer_values`;

  - For real values, the available color schemes are described in :ref:`color_map_for_real_values`;

  - For crystal orientations (cubic symmetry is assumed), the color scheme can be:

    - :data:`ipf[(<dir>)]`: IPF coloring using direction :data:`<dir>`, which can be :data:`x`, :data:`y` or :data:`z` (default :data:`z`);

    - :data:`rodrigues[(<max>)]`: Rodrigues vector coloring, where :data:`<max>` is the half-length along a direction (default is the fundamental region);

    - :data:`axis`: rotation axis coloring;
    - :data:`angle[(<max>)]`: rotation angle coloring, where :data:`<max>` is the maximum angle (in radian (default is the fundamental region);
    - :data:`axis-angle[(<max>[,absolute])]` for rotation axis / angle coloring, where :data:`<max>` is the maximum angle (in radian) and :data:`absolute` makes it so that the absolute values of the axis components are used.

  **Default value**: :data:`palette` for integer values, :data:`viridis` for real values, and :data:`rodrigues` for crystal orientations.

.. index::
   single: -datacelltrs
   single: -datapolytrs
   single: -datafacetrs
   single: -dataedgetrs
   single: -datavertrs
   single: -dataseedtrs
   single: -data...trs

.. option:: -data{cell,poly,face,edge,ver,seed,crystal}trs <transparency>

  Specify the transparencies (values from 0 to 1), which can be:

  - a real value;
  - an expression based on the variables described in :ref:`tessellation_keys`, such as :data:`x` or :data:`vol`, or in a :ref:`simulation_directory`, which allows to define individual values;
  - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

.. option:: -data{cell,poly,face,edge,ver,seed,crystal,{crystal,cell,vox}edge}rad <rad>

  Specify the radii, which can be:

  - a real value;
  - an expression based on the variables described in :ref:`tessellation_keys`, such as :data:`x` or :data:`vol`, or in a :ref:`simulation_directory`, which allows to define individual values;
  - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  .. note:: :option:`-datacelledgerad` applied only in PF space.

  **Default value**: tessellation dependent.

.. option:: -data{cell,poly,face,edge,ver,seed,crystal,vox}scale <scale>

  Specify the scale relative to the :data:`-data...col real` data, which can be:

  - :data:`<min>:\<max\>`: minimal and maximal values;
  - :data:`<min>:\<inter1\>:...:\<max\>`: minimal, intermediate and maximal values.

  When only minimal and maximal values are specified, the scale ticks are set automatically.  When intermediate values are specified, the scale ticks are the specified values.

  The scale ticks have the same format as the minimal value.

  **Default value**: :data:`<data_minimum>:\<data_maximum\>`.

.. option:: -data{cell,poly,face,edge,ver,seed,crystal,vox}scaletitle <title>

  Specify the title of the scale relative to the :data:`-data...col real` data.

  **Default value**: -.

.. option:: -datacellsymbol <symbol>

  Specify the symbol representing the cells (only for :option:`-space` :data:`pf`), which can be:

  - a symbol to apply to all cells (see below);
  - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  A symbol can be (all arguments are optional, and the radius is taken from option :option:`-data<entity>rad` if defined):

  - :data:`circle([rad=<rad>)`: a circle/disc of specified radius;
  - :data:`square[(rad=<rad>,theta=<angle>)]`: a square of specified properties;
  - :data:`triangle[(rad=<rad>,theta=<angle>)]`: a triangle of specified properties;
  - :data:`diamond[(rad=<rad>,theta=<angle>)]`: a diamond (45° rotated square) of specified properties.
  - :data:`star[(rad=<rad>,theta=<angle>)]`: a R-branch star of specified properties.

  **Default value**: :data:`circle`.

Mesh Data Loading and Rendering Options
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The following options enable the definition of the properties (color, size, etc.) of the mesh entities (3D, 2D, 1D and 0D elements and elsets, nodes, and full mesh).  :data:`elt` and :data:`elset` refer to the elements and elsets of higher dimensions.  The dimension can be also be specified explicitly, as in :data:`elt2d` or :data:`elset2d`.  :data:`node` represents all nodes, and :data:`mesh` the full mesh.  |data_description|

.. option:: -data{elt,elset,node,elt{0-3}d,elset{0-3}d,elt{2,3}dedge,mesh}col <color>

  Specify the colors, which can be done in two main different ways.

  **Colors can be specified directly**:

  - :data:`<value>`: a color (see :ref:`colors_and_color_maps`);
  - :data:`file(<file_name>)`: individual colors to load from a :ref:`data_file` (see :ref:`colors_and_color_maps`);
  - :data:`from_nodes`: colors interpolated from the node colors (defined with :data:`-datanodecol`).

  **Colors can be set from data using an argument of the general form** :data:`<data_type>:\<data_value\>`, which can be shortened to :data:`<data_value>` in the frequent case where the data type can be unambiguously determined from the data value.  The possibilities are:

  - :data:`int:\<data_value\>`: integer values represented using a color palette,  which can be:

    - an integer value;
    - an expression based on the variables defined in :ref:`tessellation_keys`, such as :data:`id` or :data:`mode`, or in a :ref:`simulation_directory`, which allows to define individual values;
    - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  - :data:`real:\<data_value\>`: real values represented using a smooth color scale, which can be:

    - a real value;
    - an expression based on the variables described in :ref:`tessellation_keys`, such as :data:`x` or :data:`vol`, or in a :ref:`simulation_directory`, which allows to define individual values;
    - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  - :data:`vector:\<data_value\>`: vectorial values (only for VTK output), which can be:

    - a simulation result (see :ref:`simulation_results`);
    - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  - :data:`tensor:\<data_value\>`: tensorial values (only for VTK output), which can be:

    - a simulation result (see :ref:`simulation_results`);
    - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.  The file can contain either 9 components or only 6 components, in which case Voigt notation is assumed.

  - :data:`ori:\<data_value\>`: crystal orientations (when applicable), which can be:

    - an orientation (see :ref:`rotations_and_orientations`);
    - :data:`internal`: internal orientations;
    - :data:`file(<file_name>,[des=<descriptor>])`: individual orientations to load from a :ref:`data_file` (see :ref:`rotations_and_orientations`).

  The color schemes used to determine the colors from the data can be fine-tuned using options :data:`-dataeltcolscheme` or :data:`-dataelsetcolscheme`.

  **Default value**: :data:`-dataelsetcol id` (elsets of higher dimension colored, other elsets white, element edges black)


.. option:: -data{elt,elset,node,elt{0-3}d,elset{0-3}d}colscheme <col_scheme>

  Specify the color scheme used to get colors from the data loaded with option :data:`-data...col`.
  The type of color scheme depends on the type of data.

  - For integer values, the color scheme is :data:`palette`, as described in :ref:`color_map_for_integer_values`;

  - For real values, the available color schemes are described in :ref:`color_map_for_real_values`;

  - For crystal orientations (cubic symmetry is assumed), the color scheme can be:

    - :data:`ipf[(<dir>)]`: IPF coloring using direction :data:`<dir>`, which can be :data:`x`, :data:`y` or :data:`z` (default :data:`z`);

    - :data:`rodrigues[(<max>)]`: Rodrigues vector coloring, where :data:`<max>` is the half-length along a direction (default is the fundamental region);

    - :data:`axis`: rotation axis coloring;
    - :data:`angle[(<max>)]`: rotation angle coloring, where :data:`<max>` is the maximum angle (in radian (default is the fundamental region);
    - :data:`axis-angle[(<max>[,absolute])]` for rotation axis / angle coloring, where :data:`<max>` is the maximum angle (in radian) and :data:`absolute` makes it so that the absolute values of the axis components are used.

  **Default value**: :data:`palette` for integer values, :data:`viridis` for real values, and :data:`rodrigues` for crystal orientations.

.. option:: -data{elt,elset,node,elt{0-3}d,elset{0-3}d}scale <scale>

  Specify the scale relative to the :data:`-data...col real` data, which can be:

  - :data:`<min>:\<max\>`: minimal and maximal values;
  - :data:`<min>:\<inter1\>:...:\<max\>`: minimal, intermediate and maximal values.

  When only minimal and maximal values are specified, the scale ticks are set automatically.  When intermediate values are specified, the scale ticks are the specified values.

  The scale ticks have the same format as the minimal value.

  **Default value**: :data:`<data_minimum>:\<data_maximum\>`.

.. option:: -data{elt,elset,node,elt{0-3}d,elset{0-3}d}scaletitle <title>

  Specify the title of the scale relative to the :data:`-data...col real` data.

  **Default value**: -.

.. option:: -data{elt{0,1}d,node,elt{2,3}dedge,elset{0,1}d}rad <rad>

  Specify the radii, which can be:

  - a real value;
  - :data:`file(<file_name>)`: individual values to load values from a :ref:`data_file` (not for :data:`*edge*`).

  **Default value**: mesh dependent.


The following options enable the loading of node positions.

.. option:: -datanodecoo <coo>

  Specify the coordinates of the nodes, which can be done in two main different ways.

  **Coordinates can be specified directly as follows:**

  - :data:`file(<file_name>)`: individual coordinates to load from a :ref:`position_file`.

  **Coordinates can be set from data using an argument of the general form** :data:`<data_type>:\<data_value\>`:

  - :data:`disp:file(<file_name>)`: individual displacements to load from a :ref:`data_file` (formatted as a :ref:`position_file`);

  - :data:`coo`: load coordinates from the simulation directory and step specified in input.

  **Default value**: -.

.. option:: -datanodecoofact <fact>

  Specify the value of the scaling factor to apply to the displacements of the nodes.

  **Default value**: :data:`1`.

Custom Input Data Loading and Rendering Options
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The following options enable the definition of the properties (color, shape, size, etc.) of custom inputs. |data_description|

.. option:: -data<input>col <color>

  Specify the colors of the input points, which can be done in two main different ways.

  **Colors can be specified directly**:

  - a color (see :ref:`colors_and_color_maps`);
  - :data:`file(<file_name>)`: individual colors to load from a :ref:`data_file` (see :ref:`colors_and_color_maps`).

  **Colors can be set from data using an argument of the general form** :data:`<data_type>:\<data_value\>`, which can be shortened to :data:`<data_value>` in the frequent case where the data type can be unambiguously determined from the data value.  The possibilities are:

  - :data:`int:\<data_value\>`: integer values represented using a color palette,  which can be:

    - an integer value;
    - an expression based on the variables defined in :ref:`point_keys`, such as :data:`id` or :data:`cell`, which allows to define individual values;
    - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  - :data:`real:\<data_value\>`: real values represented using a smooth color scale, which can be:

    - a real value;
    - an expression based on the variables described in :ref:`point_keys`, such as :data:`x` or :data:`y`, which allows to define individual values;
    - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  See also option :option:`-data<input>colscheme`.

  **Default value**: :data:`id`.


.. option:: -data<input>colscheme <col_scheme>

  Specify the color scheme used to get colors from the data loaded with option :option:`-data<input>col`.
  The type of color scheme depends on the type of data.

  - For integer values, the color scheme is :data:`palette`, as described in :ref:`color_map_for_integer_values`;

  - For real values, the available color schemes are described in :ref:`color_map_for_real_values`;

  **Default value**: :data:`palette` for integer values and :data:`viridis` for real values.

.. option:: -data<input>trs <transparency>

  Specify the transparency of the input points (a value from 0 to 1), which can be:

  - a real value;
  - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  **Default value**: :data:`0`.

.. option:: -data<input>scale <scale>

  Specify the scale relative to the :data:`-data<input>col real` data, which can be:

  - :data:`<min>:\<max\>`: minimal and maximal values;
  - :data:`<min>:\<inter1\>:...:\<max\>`: minimal, intermediate and maximal values.

  When only minimal and maximal values are specified, the scale ticks are set automatically.  When intermediate values are specified, the scale ticks are the specified values.

  The scale ticks have the same format as the minimal value.

  **Default value**: :data:`<data_minimum>:\<data_maximum\>`.

.. option:: -data<input>scaletitle <title>

  Specify the title of the scale relative to the :data:`-data<input>col real` data.

  **Default value**: -.

.. option:: -data<input>coo <coo>

  Specify the coordinates of the input points, which can be done in two main different ways.

  **Coordinates can be specified directly as follows:**

  - :data:`file(<file_name>)`: individual coordinates to load from a :ref:`position_file`.

  **Coordinates can be set from data using an argument of the general form** :data:`<data_type>:\<data_value\>`:

  - :data:`disp:file(<file_name>)`: individual displacements to load from a :ref:`data_file` (formatted as a :ref:`position_file`);

  - :data:`coo`: load coordinates from the simulation directory and step specified in input.

  **Default value**: -.

.. option:: -data<input>coofact <fact>

  Specify the value of the scaling factor to apply to the displacements of the input points.

  **Default value**: :data:`1`.

.. option:: -data<input>rad <rad>

  Specify the radii of the input points, which can be:

  - a real value;
  - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file` (not for :data:`*edge*`).

  **Default value**: :data:`0.02`.

.. option:: -data<input>symbol <symbol>

  Specify the symbol representing the input points, which can be:

  - a symbol to apply to all input points (see below);
  - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  A symbol can be (all arguments are optional, and the radius is taken from option :option:`-data<entity>rad` if defined):

  - :data:`sphere([rad=<rad>])`: a sphere of radius :data:`<rad>`;
  - :data:`cube[([rad=<rad>,[ori=<ori>)]`: a cube of radius (half edge length) :data:`<rad>` and orientation :data:`<ori>` (default identity);
  - :data:`cylinder[(rad=<rad>,length=<length>,axis=(<axis_x>,<axis_y>,<axis_z>))]`: a cylinder of specified properties;
  - :data:`arrow[(rad=<rad>,length=<length>,axis=(<axis_x>,<axis_y>,<axis_z>))]`: an arrow of specified properties;
  - :data:`disc[(rad=<rad>,axis=(<axis_x>,<axis_y>,<axis_z>))]`: a disc of specified properties;
  - :data:`ellipsoid[(rad1=<rad>,rad2=<rad>,rad3=<rad>,axis1=(<axis_x>,<axis_y>,<axis_z>)],axis2=(<axis_x>,<axis_y>,<axis_z>),axis3=(<axis_x>,<axis_y>,<axis_z>))]`: an ellipsoid of specified properties;
  - :data:`torus[(rad1=<rad>,rad2=<rad>,axis=(<axis_x>,<axis_y>,<axis_z>)]`: a torus of specified properties.

  **Default value**: sphere.

.. option:: -data<input>edgerad <rad>

  Specify the radius of the input point edges (represented as symbols), which can be any real value (:math:`\geq0`).

  **Default value**: :data:`0`.

.. option:: -data<input>edgecol <color>

  Specify the color of the input point edges (represented as symbols), which can be a color as described in :ref:`colors_and_color_maps`.

  **Default value**: :data:`black`.

Coordinate System Rendering Options
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. option:: -datacsyscoo <coo_x>:<coo_y>:<coo_z>

  Specify the coordinates of the origin of the coordinate system.

  **Default value**: :data:`0:0:0`.

.. option:: -datacsyslength <length>

  Specify the length of the coordinate system axes.

  **Default value**: :data:`0.2`.

.. option:: -datacsysrad <rad>

  Specify the radius of the coordinate system axes.

  **Default value**: :data:`0.01`.

.. option:: -datacsyslabel <label_x>:<label_y>:<label_z>

  Specify the labels of the coordinate system axes.

  .. note:: A LaTeX (mathematical) syntax can be used, but is interprated as such only by the Asymptote renderer.

  **Default value**: :data:`$X_1$:$X_2$:$X_3$`.

.. option:: -datacsyscol <color>

  Specify the color of the coordinate system, which can be a color as described in :ref:`colors_and_color_maps`.

  **Default value**: :data:`32:32:32`.

.. option:: -datacsysfontsize <size>

  Set the font size of the labels of the coordinate system axes.

  **Default value**: :data:`1`.

Show Options
~~~~~~~~~~~~~

The following options apply to the full tessellations, mesh, etc.

.. index::
   single: -showtess
   single: -showtesr
   single: -showmesh
   single: -showmeshslice
   single: -showcsys
   single: -showscale

.. option:: -show{tess,tesr,mesh,meshslice,csys,scale} <logical>

  Show or hide a full tessellation, mesh, etc.

  **Default value**: :data:`1` for the more informative data (mesh slice :math:`>` mesh :math:`>` tessellation) and :data:`0` for others.

The following option applies to a tessellation or a raster tessellation.

.. index::
   single: -showcell
   single: -showpoly
   single: -showface
   single: -showedge
   single: -showver
   single: -showseed
   single: -showcrystal
   single: -showfaceinter
   single: -showvox

.. option:: -show{cell,poly,face,edge,ver,seed,crystal,faceinter,vox} <logical>

  Specify the entities to show. The argument can be:

  - :data:`1` or :data:`all`: all;
  - an expression based on the :ref:`tessellation_keys` or :ref:`raster_tessellation_keys`, which allows to define individual values;
  - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`;
  - :data:`0` or :data:`none`: none.

  For faces, etc., :data:`cell_shown` and :data:`poly_shown` can also be used.

  **Default value**: :data:`all` for cells and edges, :data:`none` for others.

The following options apply to the entities of the mesh.  The options apply to 3D elements (:data:`elt3d`) and element sets (:data:`elset3d`), 2D elements (:data:`elt2d`) and element sets (:data:`elset2d`), 1D elements (:data:`elt1d`) and element sets (:data:`elset1d`), 0D elements (:data:`elt0d`) and element sets (:data:`elset0d`), and nodes (:data:`nodes`). Also note that the :data:`elt` and :data:`elset` can be used to denote the highest-dimension elements and elsets.

.. index::
   single: -showelt
   single: -showelset
   single: -showelt0d
   single: -showelt1d
   single: -showelt2d
   single: -showelt3d
   single: -showelset0d
   single: -showelset1d
   single: -showelset2d
   single: -showelset3d
   single: -shownode

.. option:: -show{elt,elset,elt{0-3}d,elset{0-3}d,node} <logical>

  Specify the entities to show. The argument can be:

  - :data:`1` or :data:`all`: all;
  - an expression based on the :ref:`mesh_keys`, which allows to define individual values;
  - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`;
  - :data:`0` or :data:`none`: none.

  **Default value**: :data:`1` if highest mesh dimension is 3 and :data:`0` otherwise.

.. option:: -show<input> <logical>

  Show or hide the points of a custom input.  The argument can be:

  - :data:`1` or :data:`all`: all;
  - an expression based on the :ref:`point_keys`, which allows to define individual values;
  - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`;
  - :data:`0` or :data:`none`: none.

  **Default value**: :data:`1`.

Slice Options
~~~~~~~~~~~~~~

.. option:: -slicemesh <plane1>,<plane2>,...

  Plot one (or several) slice(s) of the mesh.  A slicing plane is defined by its equation, such as :data:`x=0` or :data:`0.12*x+0.34*y+0.56*z=0.78`.

  **Default value**: -.

Camera Options
~~~~~~~~~~~~~~~

.. option:: -cameracoo <coo_x>:<coo_y>:<coo_z>

  Specify the coordinates of the camera. The coordinates can be defined as expressions based on the following variables:

  - :data:`x`, :data:`y` and :data:`z`: the center of the tessellation or mesh (if both a tessellation and a mesh have been loaded, the mesh is considered);
  - :data:`length`: the average length of the domain (:data:`1` for a unit cube);
  - :data:`vx`, :data:`vy` and :data:`vz`: the coordinates of the shift vector, which are equal to :data:`3.462`, :data:`-5.770` and :data:`4.327`, respectively in 3D, and :data:`0`, :data:`0` and :data:`8`, respectively, in 2D.

  **Default value**: :data:`x+length*vx:y+length*vy:z+length*vz`.

.. option:: -cameralookat <coo_x>:<coo_y>:<coo_z>

  Specify the coordinates of the camera look-at point.  The coordinates can be defined as expressions based on the following variables:

  - :data:`x`, :data:`y` and :data:`z`: the center of the tessellation or mesh (if both a tessellation and a mesh have been loaded, the mesh is considered).

  **Default value**: :data:`x:y:z`.

.. option:: -cameraangle <angle>

  Specify the opening angle of the camera along the horizontal direction (in degrees).

  The opening angle along the vertical direction is determined from the opening along the horizontal direction and the image size ratio.

  **Default value**: :data:`25`.

.. option:: -camerasky <dir_x>:<dir_y>:<dir_z>

  Specify the sky vector of the camera (vertical direction of the image).

  **Default value**: :data:`0:1:0` in 2D and :data:`0:0:1` in 3D.

.. option:: -cameraprojection <projection>

  Specify the type of projection of the camera, which can be :data:`perspective` or :data:`orthographic`.

  **Default value**: :data:`orthographic` for 2D and :data:`perspective` for 3D.

Light Options
~~~~~~~~~~~~~~

These options set the lights (only for the POV-Ray renderer).

.. option:: -lightambient <ambient>

  Specify the amount of ambient light.  Ambient light is light that is scattered everywhere in the scene, and it ranges from :data:`0` to :data:`1`.

  **Default value**: :data:`0.6` in 3D and :data:`1` in 2D.

.. option:: -lightsource <light1>:<light2>:...

  Specify the light sources. The argument can be:

  - :data:`point(coo=<coo>[,color=<color>][,shadow={0,1}])`: a point light source, where:

    - :data:`<coo>` can be either :data:`camera` (for the camera coordinates) or :data:`\<coo_x\>:\<coo_y\>:\<coo_z\>` for actual coordinates.

    - :data:`<color>` can be any color as described in :ref:`colors_and_color_maps`.

    - :data:`<shadow>` can be :data:`0` for no shadows or :data:`1` for shadows.

  - :data:`none`: none.

  **Default value**: :data:`"point(coo=camera,shadow=0)"` in 3D and :data:`none` in 2D.

.. option:: -lightdiffuse <diffuse>

  Specify the amount of diffuse reflection, i.e how much of the light coming directly from any light sources is reflected by diffuse reflection.  It ranges from :data:`0` to :data:`1`.

  **Default value**: :data:`0.6`.

.. option:: -lightreflection <reflection>

  Specify the amount of specular reflection, ranging from :data:`0` to :data:`1`.

  **Default value**: :data:`0.0`.

.. note:: To disable all light effects, use :data:`-lightambient 1 -lightsource none`.

Scene Options
~~~~~~~~~~~~~~

.. option:: -scenebackground <color>

  Specify the color of the background, which can be any color as described in :ref:`colors_and_color_maps`.

  **Default value**: :data:`white`.

Pole Figure Options
~~~~~~~~~~~~~~~~~~~~

.. option::  -pfdir <dir1>:<dir2>

  Specify the 2 reference coordinate system directions aligned with the horizontal and vertical directions of the pole figure, respectively, which can be :data:`x`, :data:`y`, :data:`z`, :data:`-x`, :data:`-y` or :data:`-z`.

  .. note:: The resulting normal direction must be pointing upwards.  To set the actual direction labels, see option :option:`-datacsyslabel`.

  **Default value**: :data:`x:-y`.

.. option::  -pfpole <h>:<k>:<l>

  Specify the pole family (for orientation input).

  **Default value**: :data:`1:1:1`.

.. option::  -pfprojection <projection>

  Specify the projection, which can be :data:`stereographic` or :data:`equal-area`.

  **Default value**: :data:`stereographic`.

.. option::  -pfsym <symmetry>

  Specify the symmetry, which can be :data:`monoclinic` or :data:`orthotropic`.

  **Default value**: :data:`monoclinic`.

.. option::  -pfshape <shape>

  Specify the shape, which can be :data:`full` (full circle) or :data:`quarter` (quarter-circle, for orthotropic symmetry).

  **Default value**: :data:`full`.

.. option::  -pfmode <mode1>,<mode2>,...

  Specify the representation mode, which can be:

    - :data:`symbol`: data represented as symbols;
    - :data:`density`: data represented as a distribution density field.

  .. note:: Modes are processed successively, so that the last one(s) are printed on top of the first  one(s).  In the case of multiple inputs, :data:`density` is applied only to the first input.

  **Default value**: :data:`point`.

.. option::  -pfkernel <kernel>

  Specify the kernel used to smooth pole directions when computing a pole density field, which can be:

  - :data:`normal(theta=<theta>)`: a (2-variate) normal distribution of (1-D) standard deviation :data:`<theta>` (in degrees).

  **Default value**: :data:`normal(theta=3)`.

.. option::  -pfgridsize <size> (secondary option)

  Specify the size of the density grid (in pixels).

  **Default value**: :data:`200`.

.. option::  -pfclustering <logical>

  Specify whether data clustering (which speeds up density generation) should be used.

  .. note:: Clustering applies to all representation modes and slightly alters the point positions.  It should be disable for absolute accuracy.

  **Default value**: :data:`1`.

.. option::  -pffont <font> (secondary option)

  Specify the character font, which can be :data:`TimesRoman` or :data:`ComputerModern`.

  **Default value**: :data:`TimesRoman`.

Output Image Options
~~~~~~~~~~~~~~~~~~~~~

.. option:: -imageformat <format1>,<format2>,...

  Specify the format(s) of the output image(s), which can be:

  - Usual , real space images:

    - :data:`png`: the PNG format;
    - :data:`pov`: the POV-Ray format;
    - :data:`pov:objects`: the POV-Ray format, but containing only the objects (the resulting file can be loaded with :option:`-includepov`, not compatible with :data:`png`);
    - :data:`vtk`: the VTK format.

  - Pole figure images:

    - :data:`pdf`: the PDF format (scalar format, recommended);
    - :data:`png`: the PNG format;
    - :data:`asy`: the Asymptote format.

  **Default value**: :data:`png`.

.. option:: -imagesize <size_x>:<size_y>

  Specify the size of the PNG image (in pixels).

  **Default value**: :data:`1200:900`.

Renderer Options
~~~~~~~~~~~~~~~~

.. option:: -povrayantialiasing <logical>

  Specify whether antialiasing (which produces a smoother PNG image) should be used.

  **Default value**: :data:`1`.

Scripting Options
~~~~~~~~~~~~~~~~~

.. option:: -loop <variable> <initial_value> <increment_value> <final_value> <commands> -endloop

  Create a loop of commands, as in :data:`-loop Z 0.1 0.1 0.9 -slicemesh "z=Z" -print zZ -endloop`.

  **Default value**: -.

Output Options
~~~~~~~~~~~~~~

.. option:: -outdir <dir1>,<dir2>,...

  Specify the output directories (the first valid one is used), which can be:

  - a custom directory;
  - :data:`sim_dir`: the standard location of the simulation directory, :data:`<dir.sim>/images/<format>`, where :data:`<format>` is the image format (see option :option:`-imageformat`). :data:`sim_dir` is only valid if a simulation directory is loaded as input.

  For example, to write to a simulation directory when loaded as input and to the local directory in other cases, :data:`sim_dir,.` (which may be placed in the initialization file).

  **Default value**: :option:`.`.

Print Options
~~~~~~~~~~~~~

.. option:: -print <file_name>

  Print the image.  The file name should not include the extension.

  **Default value**: -.

Advanced Options
~~~~~~~~~~~~~~~~

.. option:: -includepov <file_name1>[:<transformation1>:<transformation2>:...],<file_name2>,...

  Include objects to the image under the form of POV-Ray files and optionally apply it a transformation.  A transformation can be:

  - |translate|;
  - |scale|;
  - :data:`rotate(<theta_x>,<theta_y>,<theta_z>)`: rotate by angles :data:`thetax`, :data:`thetay` and :data:`thetaz` about :data:`x`, :data:`y` and :data:`z`.

    .. important:

    The rotation is read in POV-Ray style; i.e., according to the POV-Ray documentation:

    .. code-block:: plain

      Note that the order of the rotations does matter.  Rotations occur about the x-axis
      first, then the y-axis, then the z-axis.  If you are not sure if this is what you
      want, then you should only rotate on one axis at a time using multiple rotation
      statements to get a correct rotation.  Rotation is always performed relative to the
      axis. Thus, if an object is some distance from the axis of rotation, it will not only
      rotate but it will orbit about the axis as though it was swinging around on an
      invisible string.  POV-Ray uses a left-handed rotation system. Using the famous
      "Computer Graphics Aerobics" exercise, you hold up your left hand and point your thumb
      in the positive direction of the axis of rotation.  Your fingers will curl in the
      positive direction of rotation.  Similarly, if you point your thumb in the negative
      direction of the axis, your fingers will curl in the negative direction of
      rotation.

  **Default value**: -.

Output Files
------------

- :file:`.png`: PNG file, a bitmapped image (the alpha channel is off);
- :file:`.pdf`: PDF file, a scalar image;
- :file:`.pov`: POV-Ray file, a POV-Ray script file;
- :file:`.asy`: Asymptote file, an Asymptote script file;
- :file:`.vtk`: a VTK file, for interactive visualization.

A PNG image can be obtained from a POV-Ray file by invoking POV-Ray as follows (see the POV-Ray documentation for details and further commands):

.. code-block:: console

  $ povray <file.pov> +W<image_width>+H<image_height> -D +A0.2

Images can be obtained from an Asymptote file by invoking Asymptote as follows (see the Asymptote documentation for details and further commands):

.. code-block:: console

  $ asy -f <format> <file.asy>

where :data:`<format>` can be :data:`png`, :data:`pdf`, :data:`eps`, :data:`jpg`, :data:`tiff`, etc.

Examples
--------

Below are some examples of use of neper -V.

- Print out tessellation :file:`n10-id1.tess` with cells colored from their identifiers and an image size of 900 x 450 pixels:

  .. code-block:: console

    $ neper -V n10-id1.tess -datacellcol id -imagesize 900:450 -print img

- Print out tessellation :file:`n10-id1.tess` with cells colored from crystal orientations and semi-transparency:

  .. code-block:: console

    $ neper -V n10-id1.tess -datacellcol ori -datacelltrs 0.5 -print img

- Print out mesh :file:`n10-id1.msh` with elements colored from scalar values written in file :file:`v` and a scale ranging from 0 to 100:

  .. code-block:: console

    $ neper -V n10-id1.msh -dataeltcol "real:file(v)" -dataeltscale 0:100 -print img

- Print out mesh :file:`n10-id1.msh` with elements colored from nodal scalar values written in file :file:`v` and a scale ranging from 0 to 100:

  .. code-block:: console

    $ neper -V n10-id1.msh -datanodecol "real:file(v)" -dataeltcol from_nodes -dataeltscale 0:100 -print img

- Print out the 10 first cells of a 100-cell tessellation, colored from their identifiers and semi-transparency, and with edges shown in red and vertices shown as green spheres of radius 0.01:

  .. code-block:: console

    $ neper -V n100-id1.tess -showcell "id<=10" -datacellcol id -datacelltrs 0.5 -showedge cell_shown -showver cell_shown -dataverrad 0.01 -dataedgecol red -datavercol green -print img

- Print out the interior element sets of mesh :file:`n100-id1.msh` and show the 1D elements:

  .. code-block:: console

    $ neper -V n100-id1.tess,n100-id1.msh -dataelsetcol id -showelset 'body>0' -showelt1d elt3d_shown -print img

- Print out 3 slices of mesh :file:`n100-id1.msh`:

  .. code-block:: console

    $ neper -V n100-id1.msh -dataelsetcol id -slicemesh "x=0.5:y=0.5:z=0.5" -print img

- Print out slices of mesh :file:`n100-id1.msh`, at z coordinates ranging from 0.1 to 0.9 by step of 0.1, each slice being printed in a separate file:

  .. code-block:: console

    $ neper -V n100-id1.msh -dataelsetcol id -loop Z 0.1 0.1 0.9 -slicemesh "z=Z" -print imgZ -endloop

- Print out a deformed mesh colored by orientations, defined from a simulation directory:

  .. code-block:: console

    $ neper -V mysim.sim -step 10 -datanodecoo coo -dataeltcol ori -print img

- Print out a deformed mesh colored by orientations, defined from a simulation directory; generate an image at each deformation step:

  .. code-block:: console

    $ neper -V mysim.sim -loop STEP 0 1 10 -step STEP -datanodecoo coo -dataeltcol ori -print imgSTEP -endloop
