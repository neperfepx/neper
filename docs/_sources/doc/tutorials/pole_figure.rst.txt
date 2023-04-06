.. _pole_figure:

Plotting a Pole Figure
======================

.. important:: Read first: :ref:`visualize_tessellation`.

A pole figure can be generated using the :ref:`neper_v`, by switching to pole figure space, using :option:`-space` :data:`pf`.  Pole figures can be generated from one or several inputs, among those of the :ref:`neper_v`, such as the orientations of a tessellation cells, an orientation file and/or a direction file, making it possible to produce advanced figures.

.. note:: Inverse pole figures can be plotted just as pole figures, using :option:`-space` :data:`ipf` instead of :option:`-space` :data:`pf`.  Inverse pole figures are used in :ref:`orientation_trajectories`.

Visualization in pole figure space works in the same way as visualization in standard, real space: the entity properties can be specified using the :option:`-data*` options, the entities to show can be specified using the :option:`-show*` options, etc.  A difference is that the list of entities reduces to :data:`cell` if the input is a tessellation, and :data:`<input>` (or the default :data:`point`) if the input is a custom input (orientations or directions).

In the following, and unless mentionned otherwise, a simple tessellation is used:

.. code-block:: console

  # $ neper -T -n 100 -morpho gg -oricrysym cubic -o n100

The tessellation can be visualized as follows:

.. code-block:: console

  $ neper -V n100.tess -imagesize 800:400 -print img1

.. image:: pole_figure/img1.png

To reproduce *exactly* the images below, add the following line to your :file:`$HOME/.neperrc` file (or to a local configuration file to be loaded with :option:`--rcfile`)::

    neper -V -imagesize 500:500

Generalities
------------

The tessellation orientations can be visualized using :option:`-space` :data:`pf`:

.. code-block:: console

  $ neper -V n100.tess -space pf -print img2

.. image:: pole_figure/img2.png

This generates a PNG image named :file:`img2.png`.  By default, a pole figure is plotted from the (four) {111} poles, using a stereographic projection in the 1--2 reference plane, and all cell orientations/poles are represented using symbols.  Orientations are colored from their (cell) ids, so that colors match between visualizations in real space an pole figure space (the two images above).

.. note:: Even if only orientations/poles are represented in pole figure space, :data:`cell` is used to represent a cell orientation, which makes it possible to use the same options, :option:`-showcell`, :option:`-datacellcol`, etc. in all spaces.

For pole figures, it is also possible to write the output file at a vectorial PDF format:

.. code-block:: console

  $ neper -V n100.tess -space pf -imageformat pdf -print img2

:download:`img2.pdf <pole_figure/img2.pdf>`

.. note:: The PDF format, as a vectorial format, provides an optimal quality/weight ratio and should be used when possible (for example, in LaTeX documents).

The pole family can be modified using :option:`-pfpole`:

.. code-block:: console

  $ neper -V n100.tess -space pf -pfpole 1:0:0 -print img3

.. image:: pole_figure/img3.png


The projection type can be modified using :option:`-pfprojection`:

.. code-block:: console

  $ neper -V n100.tess -space pf -pfprojection equal-area -print img4

.. image:: pole_figure/img4.png

The sample directions can be modified using :option:`-pfdir`, so as the actual axis labels, using :option:`-datacsyslabel`:

.. code-block:: console

  $ neper -V n100.tess -space pf -pfdir y:z -datacsyslabel RD:TD:ND -print img5

.. image:: pole_figure/img5.png

The representation *mode* can be changed into :data:`density`:

.. code-block:: console

  $ neper -V n100.tess -space pf -pfmode density -print img6

.. image:: pole_figure/img6.png

It is even possible to superimpose the :data:`symbol` and :data:`density` representation modes:

.. code-block:: console
  :emphasize-lines: 2

  $ neper -V n100.tess -space pf        \
          -pfmode density,symbol        \
          -datacellcol orange           \
          -datacellrad 0.01             \
          -print img7

.. image:: pole_figure/img7.png

The visualization can be reduced to a selection of cells using :option:`-showcell` (as in real space).   For instance, to show only the cells larger than the average:

.. code-block:: console

  $ neper -V n100.tess -space pf -showcell "vol>0.01" -print img8

.. image:: pole_figure/img8.png

Finally, it is possible to change the character font from the default :data:`TimesRoman` to the LaTeX-style :data:`ComputerModern`:

.. code-block:: console

  $ neper -V n100.tess -space pf -pffont ComputerModern -print img9

.. image:: pole_figure/img9.png

Configuring a Symbol Plot
-------------------------

For a tessellation, the way data are represented can be specified using options :option:`-datacell*`. The properties include the color (options :option:`-datacellcol` and :option:`-datacellcolscheme`),
transparency (option :option:`-datacelltrs`), radius (or size) (option :option:`-datacellrad`) and symbol (option :option:`-datacellsymbol`).

The size of the poles can, for instance, be set to be proportional to the corresponding cell sizes, and their colors a function of the z cell positions:

.. code-block:: console
  :emphasize-lines: 2-5

  $ neper -V n100.tess -space pf              \
          -datacellrad real:0.1*diameq        \
          -datacellcol z                      \
          -datacellscale "0.0:1.0"            \
          -datacellscaletitle "z position"    \
          -print img10
  $ convert +append img10.png img10-scale3.png -alpha off img10.png

.. image:: pole_figure/img10.png

This originally produces a PNG file named :file:`img10.png` for the map and a PNG file named :file:`img10-scale3.png` for the scale bar, which is pasted to :file:`img10.png` thanks to :program:`convert`.

The apparence of the symbol "edges" can also be specified, using options :option:`-datacelledgerad` and :option:`-datacelledgecol`.  For example:

.. code-block:: console
  :emphasize-lines: 6,7

  $ neper -V n100.tess -space pf                  \
    -datacellrad real:0.1*diameq                  \
    -datacellcol z                                \
    -datacellscale "0.0:1.0"                      \
    -datacellscaletitle "z position"              \
    -datacelledgerad 0.02                         \
    -datacelledgecol orange                       \
    -print img11
  $ convert +append img11.png img11-scale3.png -alpha off img11.png

.. image:: pole_figure/img11.png

The type of symbols can be specified using :option:`-datacellsymbol`.  For example, to use squares:

.. code-block:: console
  :emphasize-lines: 3

  $ neper -V n100.tess -space pf                  \
    -datacellrad real:0.1*diameq                  \
    -datacellsymbol "square"                      \
    -print img12

.. image:: pole_figure/img12.png

When only the type of symbol is provided to :option:`-datacellsymbol`, the radius is taken from option :option:`-datacellrad` (either the provided value or the default value).  Alternatively, values can be provided in :option:`-datacellsymbol` itself, as well as an angle; different symbols can even be specified for the different cells, using a :ref:`data_file` (:download:`n100-symbols <pole_figure/n100-symbols>`), to produce a fully customized plot:

.. code-block:: console
  :emphasize-lines: 2

  $ neper -V n100.tess -space pf                 \
    -datacellsymbol "file(n100-symbols)"         \
    -print img13

.. image:: pole_figure/img13.png

Configuring a Density Field Plot
--------------------------------

A tessellation with typical rolling orientations is used in the following: :download:`n200-psc.tess <pole_figure/n200-psc.tess>`.

The (default) symbol plot can be turned into a density field plot using :option:`-pfmode` :data:`density`.

.. code-block:: console
  :emphasize-lines: 2

  $ neper -V n200-psc.tess -space pf            \
             -pfmode density                    \
             -print img14

.. image:: pole_figure/img14.png

A density plot is plotted with a color field and contour lines that correspond to the scale ticks.

The scale extent and ticks (and so the values of the contour lines) can be modified using :option:`-datacellscale`:

.. code-block:: console
  :emphasize-lines: 3

  $ neper -V n200-psc.tess -space pf            \
             -pfmode density                    \
             -datacellscale 0:2:4:6:8:10:12     \
             -print img15

.. image:: pole_figure/img15.png

A density field is computed by assigning a kernel to each pole, which is defined by :option:`-pfkernel` (default :data:`normal(theta=3)`).  A different value changes the intensity of the density field:

.. code-block:: console
  :emphasize-lines: 3

  $ neper -V n200-psc.tess -space pf            \
             -pfmode density                    \
             -pfkernel "normal(theta=5)"        \
             -datacellscale 0:1:2:3:4:5:6       \
             -print img16

.. image:: pole_figure/img16.png

Symmetry conditions in the reference coordinate system can be defined using :option:`-pfsym` (default :data:`monoclinic`).  It can be set to :data:`orthotropic`:

.. code-block:: console
  :emphasize-lines: 4

  $ neper -V n200-psc.tess -space pf            \
             -pfmode density                    \
             -pfkernel "normal(theta=5)"        \
             -pfsym orthotropic                 \
             -datacellscale 0:1:2:3:4:5:6       \
             -print img17

.. image:: pole_figure/img17.png

In the case of orthotropic symmetry, the pole figure can be reduced to one quarter using :option:`-pfsym` :data:`quarter` (default :data:`full`):

.. code-block:: console
  :emphasize-lines: 5

  $ neper -V n200-psc.tess -space pf            \
             -pfmode density                    \
             -pfkernel "normal(theta=5)"        \
             -pfsym orthotropic                 \
             -pfshape quarter                   \
             -datacellscale 0:1:2:3:4:5:6       \
             -print img18

.. image:: pole_figure/img18.png

The density field is defined on a regular grid, whose dimensions can be modified using :option:`-pfgridsize`.  While the default value of :data:`200` provides a good balance between quality and computation time, the option can be used to speed up generation or, at the opposite, increase quality:

.. code-block:: console
  :emphasize-lines: 6,8

  $ neper -V n200-psc.tess -space pf            \
             -pfmode density                    \
             -pfkernel "normal(theta=5)"        \
             -datacellscale 0:1:2:3:4:5:6       \
             -pfsym orthotropic                 \
             -pfgridsize 30                     \
             -print img19                       \
             -pfgridsize 300                    \
             -print img20

.. image:: pole_figure/img19.png
.. image:: pole_figure/img20.png

It is possible to specify which color scheme to use for the density plot, using :option:`-datacellcolscheme` (see :ref:`color_map_for_real_values`):

.. code-block:: console
  :emphasize-lines: 6

  $ neper -V n200-psc.tess -space pf            \
             -pfmode density                    \
             -pfkernel "normal(theta=5)"        \
             -pfsym orthotropic                 \
             -datacellscale 0.0:6               \
             -datacellcolscheme "viridis:fade"  \
             -print img21

.. image:: pole_figure/img21.png

Working From an Orientation Input
---------------------------------

A pole figure can be generated from an orientation file by loading it as a custom input (see :ref:`neper_v_input_data`).  For instance, consider an orientation file, :file:`n100.ori`, generated by :ref:`neper_t`:

.. code-block:: console

  $ neper -T -n 100 -for ori -o n100

The file can be loaded to the :ref:`neper_v` and plotted as a pole figure as follows:

.. code-block:: console

  $ neper -V n100.ori -space pf -print img22

.. image:: pole_figure/img22.png

.. note:: In :option:`-space` :data:`pf`, a custom input is interprated as an orientation file by default.
          The command produces the same image as if the orientations were read from a tessellation with the same orientations.

Since the orientations are not attached to cells, they are no longer called :data:`cell` but :data:`point` (by default) in options :option:`-data*` and :option:`-show*`.  For example, to specify symbol radii:

.. code-block:: console

  $ neper -V n100.ori -space pf -datapointrad 0.01+0.03*id/100 -print img23

.. image:: pole_figure/img23.png

Working From a Direction Input
------------------------------

A pole figure can be created from directions (instead of orientations) loaded as a custom input (see :ref:`neper_v_input_data`).  For instance, consider file :download:`dir_file <pole_figure/dir_file>`. The file can be loaded to the :ref:`neper_v`, as a direction file, and plotted as a pole figure as follows:

.. code-block:: console

  $ neper -V "mydir(type=vector):file(dir_file)" -space pf -print img24

.. image:: pole_figure/img24.png

The data are not called :data:`point` anymore but :data:`mydir` (or any other name defining the custom input), which defines new options, :option:`-datamydir*` and :option:`-showmydir`.

.. code-block:: console
  :emphasize-lines: 2

  $ neper -V "mydir(type=vector):file(dir_file)" -space pf \
          -datamydirrad 0.01+0.03*id/100                   \
          -print img25

.. image:: pole_figure/img25.png

Working From Multiple Inputs
----------------------------

A pole figure can be created from multiple inputs loaded as custom inputs (see :ref:`neper_v_input_data`).  This can, for example, be used to plot a (lattice) rotation path associated with plastic deformation. In the following, file :download:`ini <pole_figure/ini>` contains an initial orientation, :download:`all <pole_figure/all>` its locations during plastic deformation, and :download:`fin <pole_figure/fin>` the final orientation, all written as Rodrigues vectors.

.. important:: To plot lattice rotation paths, is it generally more efficient to use the method described in :ref:`orientation_trajectories` instead of what is described here.

The initial orientation, rotation path and final orientation can be visualized as follows:

.. code-block:: console
  :emphasize-lines: 1,3-5

  $ neper -V "final(type=ori):file(fin),initial(type=ori):file(ini),path(type=ori):file(all)" \
          -space pf                                     \
          -datainitialrad 0.030 -datainitialcol pink    \
          -datapathrad    0.005 -datapathcol    black   \
          -datafinalrad   0.060 -datafinalcol   red     \
          -print img26

.. image:: pole_figure/img26.png

It is also possible to plot in :data:`density` mode, in which case the mode applies only to the first input.  This can be used, for example, to mimic an orientation spread at final state:

.. code-block:: console
  :emphasize-lines: 6

  $ neper -V "final(type=ori):file(fin),initial(type=ori):file(ini),path(type=ori):file(all)" \
          -space pf                                     \
          -datainitialrad 0.030 -datainitialcol pink    \
          -datapathrad    0.005 -datapathcol    black   \
          -datafinalrad   0.000                         \
          -pfmode density,symbol                        \
          -datafinalscale 0.0:12                        \
          -datafinalcolscheme viridis:fade              \
          -pfgridsize 300 -pfkernel "normal(theta=8)"   \
          -print img27

.. image:: pole_figure/img27.png

Of course, it is also possible to represent final state using a file containing several orientations.
