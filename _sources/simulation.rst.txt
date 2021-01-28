.. _simulation_link:

Simulation
==========

Module -S is dedicated to processing simulation results.  It is able to rewrite
FEPX raw simulation results into a more human-friendly *simulation directory*, and
post-process the results further.  The simulation directory can be taken as
input by module -V for advanced visualization.

.. contents::
   :depth: 2
   :local:

Generalities
------------

The first primary capability is to convert FEPX raw simulation results into a simulation directory, which is as simple as running
::
  $ neper -S jobdir

The resulting simulation directory (jobdir.sim) contains both the simulation input and results, and is structured as follows.  Result files are ASCII files and are provided at each simulation step.
::

  jobdir.sim
  ├── inputs
  │   ├── simulation.config
  │   ├── simulation.msh
  │   └── simulation.tess
  ├── report
  └── results
      ├── elements
      │   ├── strain-eq
      │   │   ├── strain-eq.step0
      │   │   ├── strain-eq.step1
      │   │   ├── strain-eq.step2
      │   │   ...
      │   ├── stress
      │   │   ├── stress.step0
      │   │   ├── stress.step1
      │   │   ├── stress.step2
      │   │   ...
      │   ├── ...
      │   ...
      └── nodes
          ├── coo
          │   ├── coo.step0
          │   ├── coo.step1
          │   ├── coo.step2
          │   ...
          ├── ...
          ...

Computing new nodal or elemental results
----------------------------------------

Nodal or elemental results can be added to the simulation directory.  The data can be mesh results (as also available through Neper -M's -stat* options), simulation subresults (such as vector or tensor components), or new results defined by mathematical expressions based on mesh or simulation results. The results can be assigned names. Existing results at a specific step can be accessed using (step=...).

::

  $ neper -S jobdir.sim -eltres vol,stress33,energy:0.1234*(crss-crss(step=0))^2"

The results add to the simulation directory, which becomes
::

  jobdir.sim
  ├── inputs
  │   ├── simulation.config
  │   ├── simulation.msh
  │   └── simulation.tess
  ├── report
  └── results
      ├── elements
      │   ├── energy
      │   │   ├── energy.step0
      │   │   ├── energy.step1
      │   │   ├── energy.step2
      │   │   ...
      │   ├── strain-eq
      │   │   ├── strain-eq.step0
      │   │   ├── strain-eq.step1
      │   │   ├── strain-eq.step2
      │   │   ...
      │   ├── stress
      │   │   ├── stress.step0
      │   │   ├── stress.step1
      │   │   ├── stress.step2
      │   │   ...
      │   ├── stress33
      │   │   ├── stress33.step0
      │   │   ├── stress33.step1
      │   │   ├── stress33.step2
      │   │   ...
      │   ├── vol
      │   │   ├── vol.step0
      │   │   ├── vol.step1
      │   │   ├── vol.step2
      │   │   ...
      │   ├── ...
      │   ...
      └── nodes
          ├── coo
          │   ├── coo.step0
          │   ├── coo.step1
          │   ├── coo.step2
          │   ...
          ├── ...
          ...

Computing elset results
-----------------------

Elset results can be computed from the elemental results of the simulation directory.  This is simply done by volume-weighted averaging (orientations are also properly averaged, using quaternions). The results are written to directory "elsets".

::

  $ neper -S jobdir.sim -elsetres stress,stress33

The results add to the simulation directory, which becomes
::

  jobdir.sim
  ├── inputs
  │   ├── simulation.config
  │   ├── simulation.msh
  │   └── simulation.tess
  ├── report
  └── results
      ├── elements
      │   ├── energy
      │   │   ├── energy.step0
      │   │   ├── energy.step1
      │   │   ├── energy.step2
      │   │   ...
      │   ├── strain-eq
      │   │   ├── strain-eq.step0
      │   │   ├── strain-eq.step1
      │   │   ├── strain-eq.step2
      │   │   ...
      │   ├── stress
      │   │   ├── stress.step0
      │   │   ├── stress.step1
      │   │   ├── stress.step2
      │   │   ...
      │   ├── stress33
      │   │   ├── stress33.step0
      │   │   ├── stress33.step1
      │   │   ├── stress33.step2
      │   │   ...
      │   ├── vol
      │   │   ├── vol.step0
      │   │   ├── vol.step1
      │   │   ├── vol.step2
      │   │   ...
      │   ├── ...
      │   ...
      ├── elsets
      │   ├── stress
      │   │   ├── stress.step0
      │   │   ├── stress.step1
      │   │   ├── stress.step2
      │   │   ...
      │   ├── stress33
      │   │   ├── stress33.step0
      │   │   ├── stress33.step1
      │   │   ├── stress33.step2
      │   │   ...
      └── nodes
          ├── coo
          │   ├── coo.step0
          │   ├── coo.step1
          │   ├── coo.step2
          │   ...
          ├── ...
          ...

Removing results
----------------

Results can be removed by preceding them by "!" (note the single quotes, below).

::

  $ neper -S jobdir.sim -eltres '!stress,!stress33,!vol,!strain-eq' -elsetres '!stress,!stress33'

The simulation directory becomes
::

  jobdir.sim
  ├── inputs
  │   ├── simulation.config
  │   ├── simulation.msh
  │   └── simulation.tess
  ├── report
  └── results
      ├── elements
      │   ├── energy
      │   │   ├── energy.step0
      │   │   ├── energy.step1
      │   │   ├── energy.step2
      │   │   ...
      │   ...
      └── nodes
          ├── coo
          │   ├── coo.step0
          │   ├── coo.step1
          │   ├── coo.step2
          │   ...
          ├── ...
          ...


